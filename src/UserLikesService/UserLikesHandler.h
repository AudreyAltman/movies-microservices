#ifndef MOVIES_MICROSERVICES_USERLIKESHANDLER_H
#define MOVIES_MICROSERVICES_USERLIKESHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>
#include <mongoc.h>
#include <bson/bson.h>

#include "../../gen-cpp/UserLikesService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class UserLikesServiceHandler : public UserLikesServiceIf {
 public:
  UserLikesServiceHandler(mongoc_client_pool_t *);
  ~UserLikesServiceHandler() override=default;

  void UserRateMovie(const int64_t user_id, const std::string& movie_id, const int64_t likeDislike) override;
  void GetUsersLikedMovies(std::vector<std::string>& _return, const int64_t user_id) override;
  int64_t GetMovieRating(const std::string& movie_id) override;
  void UserWatchMovie(const int64_t user_id, const std::string& movie_id) override;
  void AddUser(const std::string& user_name) override;
  int64_t GetUserID(const std::string& user_name) override;
 private:
  mongoc_client_pool_t *_mongodb_client_pool;
};

// Constructor
UserLikesServiceHandler::UserLikesServiceHandler(mongoc_client_pool_t *mongodb_client_pool) {
  // Storing the clientpool
       _mongodb_client_pool = mongodb_client_pool;
}

// Remote Procedure "UserRateMovie"
void UserLikesServiceHandler::UserRateMovie(const int64_t user_id, const std::string& movie_id, const int64_t likeDislike) {
	if (user_id == 1 || user_id == -1) {
		// Get mongo client
		mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);
		if (!mongodb_client) {
		  ServiceException se;
		  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
		  se.message = "Failed to pop a client from MongoDB pool";
		  throw se;
		}

		// Get mongo collection
		auto collection_users = mongoc_client_get_collection(mongodb_client, "likes", "user-likes");
		if (!collection_users) {
		  ServiceException se;
		  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
		  se.message = "Failed to create collection user from DB likes";
		  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		  throw se;
		}
		
		// Only bother updating movies record if rating has changed
		int64_t current_rating = 0;

		// Check if record exists already for user and movie
		bson_t *query_user = bson_new();
		BSON_APPEND_INT64(query_user, "user_id", user_id);
		mongoc_cursor_t *cursor_user = mongoc_collection_find_with_opts(collection_users, query_user, nullptr, nullptr);
		
		const bson_t *doc_user;
		bool found_user = mongoc_cursor_next(cursor_user, &doc_user);
		
		bool found_user_movie = false;
		while (found_user && !found_user_movie) {
			// Check to see if user has rated movie
			auto user_movies_json_char = bson_as_json(doc_user, nullptr);
			json user_movies_json = json::parse(user_movies_json_char);
			
			if (user_movies_json["movie_id"] == movie_id) {
				found_user_movie = true;
				
				// Get current rating
				current_rating = user_movies_json["rating"].json::get<int>();
				// If user rating is already set appropriately,
				// break from loop (do nothing)
				if (current_rating == likeDislike) {
					break;
				}
				// If not set to correct value
				// Update current rating to new rating in user likes
				bson_t *update_user = bson_new();
				BSON_APPEND_INT64(update_user, "user_id", user_id);
				BSON_APPEND_UTF8(update_user, "movie_id", movie_id.c_str());
				BSON_APPEND_INT64(update_user, "rating", likeDislike);				
				bson_error_t error;
				
				bool updateUser = mongoc_collection_update_one(collection_users, doc_user, update_user, nullptr, nullptr, &error);	
				
				if (!updateUser) {
					LOG(error) << "Failed to update user-likes record for user "
						<< std::to_string(user_id) << " movie " << movie_id << " to MongoDB: " << error.message;
					ServiceException se;
					se.errorCode = ErrorCode::SE_MONGODB_ERROR;
					se.message = error.message;
					// Cleanup mongo
					bson_destroy(update_user);
					bson_destroy(query_user);
					mongoc_collection_destroy(collection_users);
					mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
					throw se;
				}
				bson_destroy(update_user);
			}				
		}
		// If user has not previously rated movie
		if (!found_user_movie) {
			// Add record with user id, movie id, and rating
			bson_t *add_user = bson_new();
			BSON_APPEND_INT64(add_user, "user_id", user_id);
			BSON_APPEND_UTF8(add_user, "movie_id", movie_id.c_str());
			BSON_APPEND_INT64(add_user, "rating", likeDislike);				
			bson_error_t error;
			
			bool addUser = mongoc_collection_insert_one(collection_users, add_user, nullptr, nullptr, &error);	
			
			if (!addUser) {
				LOG(error) << "Failed to add user-likes record for user "
					<< std::to_string(user_id) << " movie " << movie_id << " to MongoDB: " << error.message;
				ServiceException se;
				se.errorCode = ErrorCode::SE_MONGODB_ERROR;
				se.message = error.message;
				// Cleanup mongo
				bson_destroy(add_user);
				bson_destroy(query_user);
				mongoc_collection_destroy(collection_users);
				mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
				throw se;
			}
			bson_destroy(add_user);
		}
		
		// Cleanup mongo from users
		bson_destroy(query_user);
		mongoc_cursor_destroy(cursor_user);
		mongoc_collection_destroy(collection_users);
		
		// Update movie record if needed
		if (current_rating != likeDislike) {
			// Get mongo collection
			auto collection_movies = mongoc_client_get_collection(mongodb_client, "likes", "movie-likes");
			if (!collection_movies) {
				ServiceException se;
				se.errorCode = ErrorCode::SE_MONGODB_ERROR;
				se.message = "Failed to create collection user from DB likes";
				mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
			  throw se;
			}
			
			// Check if movie likes record exists
			bson_t *query_movie = bson_new();
			BSON_APPEND_UTF8(query_movie, "movie_id", movie_id.c_str());
			mongoc_cursor_t *cursor_movie = mongoc_collection_find_with_opts(collection_movies, query_movie, nullptr, nullptr);
			
			const bson_t *doc_movie;
			bool found_movie = mongoc_cursor_next(cursor_movie, &doc_movie);
			
			if (found_movie) {
				auto movie_json_char = bson_as_json(doc_movie, nullptr);
				json movie_json = json::parse(movie_json_char);
				
				// Update movie ratings
				bson_t *update_movie = bson_new();
				BSON_APPEND_UTF8(update_movie, "movie_id", movie_id.c_str());
				// Check if user rating existed
				if (current_rating == 0) {
					// If user liked, add to likes column
					if (likeDislike == 1) {
						int64_t upd_likes = movie_json["likes"].json::get<int>() + 1;
						BSON_APPEND_INT64(update_movie, "likes", upd_likes);
					} 
					// Else add to dislikes column
					else {
						int64_t upd_dislikes = movie_json["dislikes"].json::get<int>() + 1;
						BSON_APPEND_INT64(update_movie, "dislikes", upd_dislikes);
					}
				} else {
					// Update both movie rating columns if rating changed
					// If user liked, add to likes column and subtract from dislikes column
					if (likeDislike == 1) {
						int64_t upd_likes = movie_json["likes"].json::get<int>() + 1;
						int64_t upd_dislikes = movie_json["dislikes"].json::get<int>() - 1;
						BSON_APPEND_INT64(update_movie, "likes", upd_likes);
						BSON_APPEND_INT64(update_movie, "dislikes", upd_dislikes);
					} 
					// Else add to dislikes column and subtract from likes column
					else {
						int64_t upd_likes = movie_json["likes"].json::get<int>() - 1;
						int64_t upd_dislikes = movie_json["dislikes"].json::get<int>() + 1;
						BSON_APPEND_INT64(update_movie, "likes", upd_likes);
						BSON_APPEND_INT64(update_movie, "dislikes", upd_dislikes);
					}
				}
				bson_error_t error;
				
				bool updateMovie = mongoc_collection_update_one(collection_movies, doc_movie, update_movie, nullptr, nullptr, &error);	
				
				if (!updateMovie) {
					LOG(error) << "Failed to update movie-likes record " << movie_id << " to MongoDB: " << error.message;
					ServiceException se;
					se.errorCode = ErrorCode::SE_MONGODB_ERROR;
					se.message = error.message;
					// Cleanup mongo
					bson_destroy(update_movie);
					bson_destroy(query_movie);
					mongoc_cursor_destroy(cursor_movie);
					mongoc_collection_destroy(collection_movies);
					mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
					throw se;
				}
				bson_destroy(update_movie);
				
			} else {
				// Add movie rating
				bson_t *add_movie = bson_new();
				BSON_APPEND_UTF8(add_movie, "movie_id", movie_id.c_str());
				if (likeDislike == 1) {
					BSON_APPEND_INT64(add_movie, "likes", 1);
					BSON_APPEND_INT64(add_movie, "dislikes", 0);
				} else {
					BSON_APPEND_INT64(add_movie, "likes", 0);
					BSON_APPEND_INT64(add_movie, "dislikes", 1);
				}
				
				bson_error_t error;
				bool addMovie = mongoc_collection_insert_one(collection_movies, add_movie, nullptr, nullptr, &error);
				
				if (!addMovie) {
					LOG(error) << "Failed to add movie-likes record " << movie_id << " to MongoDB: " << error.message;
					ServiceException se;
					se.errorCode = ErrorCode::SE_MONGODB_ERROR;
					se.message = error.message;
					// Cleanup mongo
					bson_destroy(add_movie);
					bson_destroy(query_movie);
					mongoc_cursor_destroy(cursor_movie);
					mongoc_collection_destroy(collection_movies);
					mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
					throw se;
				}
				
				bson_destroy(add_movie);
			}
			
			// Cleanup mongo
			bson_destroy(query_movie);
			mongoc_cursor_destroy(cursor_movie);
			mongoc_collection_destroy(collection_movies);
		}
		
		// Cleanup mongo
		mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		mongoc_cleanup();
	}
	else {
		ServiceException se;
		se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
		se.message = "Invalid likeDislike input in function call to UserRateMovie.";
		throw se;
	}
	return;
}

void UserLikesServiceHandler::GetUsersLikedMovies(std::vector<std::string>& _return, const int64_t user_id) {
	// TO DO: look up movies with value 1 in like position
	_return.push_back("123");
	_return.push_back("abc");
}

int64_t UserLikesServiceHandler::GetMovieRating(const std::string& movie_id) {
	int64_t rating_return = 0;

	// Get mongo client
	mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);

	if (!mongodb_client) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to pop a client from MongoDB pool";
	  throw se;
	}

	// Get mongo collection
	auto collection = mongoc_client_get_collection(mongodb_client, "likes", "movie-likes");

	if (!collection) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to create collection user from DB likes";
	  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	  throw se;
	}

	// Check if movie provided exists in database
	bson_t *query = bson_new();
	BSON_APPEND_UTF8(query, "movie_id", movie_id.c_str());

	mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
	const bson_t *doc;
	bool found = mongoc_cursor_next(cursor, &doc);

	if (found) {
	  // Calculate rating by subtracting dislikes from likes
	  auto likes_json_char = bson_as_json(doc, nullptr);
	  json likes_json = json::parse(likes_json_char);
	  
	  rating_return = likes_json["user_likes"].json::get<int>() - likes_json["user_dislikes"].json::get<int>();
	}

	// Cleanup mongo
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	mongoc_cleanup();

	return rating_return;
}

void UserLikesServiceHandler::UserWatchMovie(const int64_t user_id, const std::string& movie_id) {
	// TO DO: put 0 in rating position for user and movie
	if (user_id != 0 && movie_id != "") {
		// put 0 in rating for movie
	}
	else {
		return;
	}
}

void UserLikesServiceHandler::AddUser(const std::string& user_name) {
	// TO DO: add user to database and dynamically assign user_id
	if (user_name != "") {
		// validate that username does not already exist
		// add user to database
	}
	else {
		return;
	}
}

int64_t UserLikesServiceHandler::GetUserID(const std::string& user_name) {
	// TO DO: get user ID based on username
	return 8473;
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_USERLIKESHANDLER_H

