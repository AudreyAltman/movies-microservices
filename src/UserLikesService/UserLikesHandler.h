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
// NOTE: Audrey simplified this in order to get it working with the nginx endpoint and validate it against the batch job.
//       Nicole can add complexity back in once the batch job is confirmed to work.
void UserLikesServiceHandler::UserRateMovie(const int64_t user_id, const std::string& movie_id, const int64_t likeDislike) {
	// TO DO: update database with user like or dislike (false == dislike, true == like)

  // Get mongo client
    mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);

    if (!mongodb_client) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_MONGODB_ERROR;
      se.message = "Failed to pop a client from MongoDB pool";
      throw se;
    }

    // Get mongo collection
    auto collection = mongoc_client_get_collection(
        mongodb_client, "user-likes", "user-likes");

    if (!collection) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_MONGODB_ERROR;
      se.message = "Failed to create collection user from DB recommender";
      mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
      throw se;
    }

    // TODO: Check if the rating for this user and movie already exist in the database
    //  If it does exists, set found to true
    bool found = false;

   if (found) {
      ServiceException se;
      se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
      se.message = ""; // TODO
//          mongoc_cursor_destroy(cursor);
      mongoc_collection_destroy(collection);
      mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
      throw se;
   } else {
      bson_t *new_doc = bson_new();
      BSON_APPEND_INT64(new_doc, "user_id", user_id);
      BSON_APPEND_UTF8(new_doc, "movie_id", movie_id.c_str());
      BSON_APPEND_INT64(new_doc, "rating", likeDislike);

      bson_error_t error;
      bool plotinsert = mongoc_collection_insert_one (collection, new_doc, nullptr, nullptr, &error);

      if (!plotinsert) {
         LOG(error) << "Failed to insert rating for " << user_id << " to MongoDB: " << error.message;
         ServiceException se;
         se.errorCode = ErrorCode::SE_MONGODB_ERROR;
         se.message = error.message;
         bson_destroy(new_doc);
//             mongoc_cursor_destroy(cursor);
         mongoc_collection_destroy(collection);
         mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
         throw se;
      }
      bson_destroy(new_doc);
   }

    // Cleanup mongo
//        bson_destroy(query);
//        mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
    mongoc_cleanup ();
}

// Remote Procedure "GetUsersLikedMovies"
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
	auto collection = mongoc_client_get_collection(mongodb_client, "likes", "user-likes");
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

		// If rating is a like, add to return vector
		if (ratings_json["rating"].json::get<int>() == 1) {
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

// Remote Procedure "GetMovieRating"
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

	  rating_return = likes_json["likes"].json::get<int>() - likes_json["dislikes"].json::get<int>();
	}

	// Cleanup mongo
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	mongoc_cleanup();

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
	auto collection = mongoc_client_get_collection(mongodb_client, "likes", "user-likes");
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
	// Get mongo client
	mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);
	if (!mongodb_client) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to pop a client from MongoDB pool";
	  throw se;
	}

	// Get mongo collection
	auto collection = mongoc_client_get_collection(mongodb_client, "likes", "users");
	if (!collection) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to create collection user from DB likes";
	  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	  throw se;
	}

	// Check if user_name already exists
	bson_t *query = bson_new();
	BSON_APPEND_UTF8(query, "user_name", user_name.c_str());

	mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);

	const bson_t *doc;
	bool found_user = mongoc_cursor_next(cursor, &doc);

	if (found_user) {
		ServiceException se;
        se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
        se.message = "User name " + user_name + " already existed in MongoDB Users";
		bson_destroy(query);
		mongoc_cursor_destroy(cursor);
		mongoc_collection_destroy(collection);
		mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		throw se;
	}

	bson_t *add_user = bson_new();
	BSON_APPEND_INT64(add_user, "user_id", id_counter++);
	BSON_APPEND_UTF8(add_user, "user_name", user_name.c_str());

	bson_error_t error;
	bool addUser = mongoc_collection_insert_one(collection, add_user, nullptr, nullptr, &error);

	if (!addUser) {
		LOG(error) << "Failed to add users record for user name " << user_name <<" to MongoDB: " << error.message;
		ServiceException se;
		se.errorCode = ErrorCode::SE_MONGODB_ERROR;
		se.message = error.message;
		// Cleanup mongo
		bson_destroy(add_user);
		bson_destroy(query);
		mongoc_cursor_destroy(cursor);
		mongoc_collection_destroy(collection);
		mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		throw se;
	}

	// Cleanup mongo
	bson_destroy(add_user);
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
}

// Remote Procedure "GetUserID"
int64_t UserLikesServiceHandler::GetUserID(const std::string& user_name) {
	// Get mongo client
	mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);
	if (!mongodb_client) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to pop a client from MongoDB pool";
	  throw se;
	}

	// Get mongo collection
	auto collection = mongoc_client_get_collection(mongodb_client, "likes", "users");
	if (!collection) {
	  ServiceException se;
	  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
	  se.message = "Failed to create collection user from DB likes";
	  mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
	  throw se;
	}

	// Make sure user_name exists
	bson_t *query = bson_new();
	BSON_APPEND_UTF8(query, "user_name", user_name.c_str());

	mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);

	const bson_t *doc;
	bool found_user = mongoc_cursor_next(cursor, &doc);

	if (!found_user) {
		ServiceException se;
        se.errorCode = ErrorCode::SE_THRIFT_HANDLER_ERROR;
        se.message = "User name " + user_name + " does not exist in MongoDB Users";
		bson_destroy(query);
		mongoc_cursor_destroy(cursor);
		mongoc_collection_destroy(collection);
		mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
		throw se;
	}

	auto user_json_char = bson_as_json(doc, nullptr);
	json user_json = json::parse(user_json_char);

	int64_t _return_id = user_json["user_id"].json::get<int>();

	// Cleanup mongo
	bson_destroy(query);
	mongoc_cursor_destroy(cursor);
	mongoc_collection_destroy(collection);
	mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);

	return _return_id;
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_USERLIKESHANDLER_H
