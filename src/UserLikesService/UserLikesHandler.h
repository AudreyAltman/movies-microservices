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
    static int64_t id_counter;
};

// Constructor
UserLikesServiceHandler::UserLikesServiceHandler(
    mongoc_client_pool_t *mongodb_client_pool) {

        // Storing the clientpool
         _mongodb_client_pool = mongodb_client_pool;
}

// Remote Procedure "UserRateMovie"
void UserLikesServiceHandler::UserRateMovie(const int64_t user_id, const std::string& movie_id, const int64_t likeDislike) {
	if (likeDislike > 0) {
		// Get mongo client
		mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);
		if (!mongodb_client) {
		  ServiceException se;
		  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
		  se.message = "Failed to pop a client from MongoDB pool";
		  throw se;
		}

		// Get mongo collection
		auto collection_users = mongoc_client_get_collection(mongodb_client, "user-likes", "user-likes");
		if (!collection_users) {
		  ServiceException se;
		  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
		  se.message = "Failed to create collection user from DB likes";
		  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		  throw se;
		}

		// Only bother updating movies record if rating has changed
		int64_t current_rating = -1;
		
		// Check if record exists already for user and movie
		bson_t *query_user = bson_new();
		BSON_APPEND_INT64(query_user, "user_id", user_id);
		BSON_APPEND_UTF8(query_user, "movie_id", movie_id.c_str());
		mongoc_cursor_t *cursor_user = mongoc_collection_find_with_opts(collection_users, query_user, nullptr, nullptr);
		
		const bson_t *doc_user;
		bool found_user_movie = mongoc_cursor_next(cursor_user, &doc_user);
		
		if (found_user_movie) {
			// Check to see if user has rated movie
			auto user_movies_json_char = bson_as_json(doc_user, nullptr);
			json user_movies_json = json::parse(user_movies_json_char);
			
			// Get current rating
			current_rating = user_movies_json["rating"].json::get<int>();
			
			// If user rating is not set appropriately,
			// Update current rating to new rating in user likes
			if (current_rating != likeDislike) {
				bson_t *update_user = bson_new();
				BSON_APPEND_INT64(update_user, "user_id", user_id);
				BSON_APPEND_UTF8(update_user, "movie_id", movie_id.c_str());
				BSON_APPEND_INT64(update_user, "rating", likeDislike);				
				bson_error_t error;
				
				bool updateUser = mongoc_collection_replace_one(collection_users, doc_user, update_user, nullptr, nullptr, &error);	
				
				if (!updateUser) {
					LOG(error) << "Failed to update user-likes record for user "
						<< std::to_string(user_id) << " movie " << movie_id << " to MongoDB: " << error.message;
					ServiceException se;
					se.errorCode = ErrorCode::SE_MONGODB_ERROR;
					se.message = error.message;
					// Cleanup mongo
					bson_destroy(update_user);
					bson_destroy(query_user);
					mongoc_cursor_destroy(cursor_user);
					mongoc_collection_destroy(collection_users);
					mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
					throw se;
				}
				bson_destroy(update_user);
			}
		} else {
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
				mongoc_cursor_destroy(cursor_user);
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
		mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		mongoc_cleanup();
	} else {
		ServiceException se;
		se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
		se.message = "Invalid rating in function call to UserRateMovie.";
		throw se;
	}
	return;
}

// Remote Procedure "GetUsersLikedMovies"
// Returns movies if rating is higher than 2
void UserLikesServiceHandler::GetUsersLikedMovies(std::vector<std::string>& _return, const int64_t user_id) {
	// Initialize empty movie_id list
    std::vector<std::string> _return_movie_ids;

	// Get mongo client
	mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);
	if (!mongodb_client) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to pop a client from MongoDB pool";
	  throw se;
	}

	// Get mongo collection
	auto collection = mongoc_client_get_collection(mongodb_client, "user-likes", "user-likes");
	if (!collection) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to create collection user from DB likes";
	  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	  throw se;
	}

	// Get users rated movies from database
	bson_t *query = bson_new();
	BSON_APPEND_INT64(query, "user_id", user_id);

	mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
	const bson_t *doc;
	bool found = mongoc_cursor_next(cursor, &doc);

	while (found) {
		auto ratings_json_char = bson_as_json(doc, nullptr);
		json ratings_json = json::parse(ratings_json_char);

		// If rating is higher than 2, add to return vector
		if (ratings_json["rating"].json::get<int>() > 2) {
			_return_movie_ids.push_back(ratings_json["movie_id"]);
		}
		// Check next record
		found = mongoc_cursor_next(cursor, &doc);
	}

	_return = _return_movie_ids;

	// Cleanup mongo
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
    mongoc_cleanup ();
}

// Remote Procedure "GetMovieRating": does nothing for now, future implementation
int64_t UserLikesServiceHandler::GetMovieRating(const std::string& movie_id) {
	// TO DO: implement function
	int64_t rating_return = 0;
	return rating_return;
}

// Remote Procedure "UserWatchMovie"
void UserLikesServiceHandler::UserWatchMovie(const int64_t user_id, const std::string& movie_id) {
	// Get mongo client
	mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);
	if (!mongodb_client) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to pop a client from MongoDB pool";
	  throw se;
	}

	// Get mongo collection
	auto collection = mongoc_client_get_collection(mongodb_client, "user-likes", "user-likes");
	if (!collection) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to create collection user from DB likes";
	  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	  throw se;
	}

	// Find out if user has already watched or rated movie
	bson_t *query = bson_new();
	BSON_APPEND_INT64(query, "user_id", user_id);
	BSON_APPEND_UTF8(query, "movie_id", movie_id.c_str());

	mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
	const bson_t *doc;
	bool found = mongoc_cursor_next(cursor, &doc);

	// Only if user has not already rated/watched movie do we add a record
	if (!found) {
		bson_t *add_movie = bson_new();
		BSON_APPEND_INT64(add_movie, "user_id", user_id);
		BSON_APPEND_UTF8(add_movie, "movie_id", movie_id.c_str());
		BSON_APPEND_INT64(add_movie, "rating", 0);

		bson_error_t error;
		bool addMovie = mongoc_collection_insert_one(collection, add_movie, nullptr, nullptr, &error);

		if (!addMovie) {
			LOG(error) << "Failed to add user-likes record for user " << std::to_string(user_id) <<
			" and movie " << movie_id << " to MongoDB: " << error.message;
			ServiceException se;
			se.errorCode = ErrorCode::SE_MONGODB_ERROR;
			se.message = error.message;
			// Cleanup mongo
			bson_destroy(add_movie);
			bson_destroy(query);
			mongoc_cursor_destroy(cursor);
			mongoc_collection_destroy(collection);
			mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
			throw se;
		}
		bson_destroy(add_movie);
	}

	// Cleanup mongo
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	mongoc_cleanup();
}

// Remote Procedure "AddUser"
void UserLikesServiceHandler::AddUser(const std::string& user_name) {
	// DO NOTHING: future implementation
}

// Remote Procedure "GetUserID"
int64_t UserLikesServiceHandler::GetUserID(const std::string& user_name) {
	// DO NOTHING: future implementation
	return 0;
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_USERLIKESHANDLER_H
