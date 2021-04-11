#ifndef MOVIES_MICROSERVICES_USERLIKESHANDLER_H
#define MOVIES_MICROSERVICES_USERLIKESHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>

#include "../../gen-cpp/UserLikesService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"
#include <mongoc.h>
#include <bson/bson.h>

namespace movies{

class UserLikesServiceHandler : public UserLikesServiceIf {
 public:
  UserLikesServiceHandler(mongoc_client_pool_t *);
  ~UserLikesServiceHandler() override=default;

  void GetMovieLikesByIds(std::vector<int64_t>& _return, const std::vector<std::string> & movie_ids) override;
  void UserRateMovie(const std::string& user_id, const std::string& movie_id, const int64_t likeDislike) override;
  void GetUsersLikedMovies(std::vector<std::string>& _return, const std::string& user_id) override;
  int64_t GetMovieRating(const std::string& movie_id) override;
  void UserWatchMovie(const std::string& user_id, const std::string& movie_id) override;
  void AddUser(const std::string& user_name) override;
  void GetUserID(std::string& _return, const std::string& user_name) override;
  private:
    mongoc_client_pool_t *_mongodb_client_pool;
};

// Constructor
UserLikesServiceHandler::UserLikesServiceHandler(
    mongoc_client_pool_t *mongodb_client_pool) {

        // Storing the clientpool
         _mongodb_client_pool = mongodb_client_pool;
}

// Remote Procedure "GetMoviesLikesByIds"
void UserLikesServiceHandler::GetMovieLikesByIds(std::vector<int64_t>& _return, const std::vector<std::string> & movie_ids) {
	_return.push_back(-5);
	_return.push_back(8);
}
// Remote Procedure "UserRateMovie"
void UserLikesServiceHandler::UserRateMovie(const std::string& user_id, const std::string& movie_id, const int64_t likeDislike) {
	// TO DO: update database with user like or dislike (false == dislike, true == like)
	if (user_id != "") {

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
          BSON_APPEND_UTF8(new_doc, "user_id", user_id.c_str());
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

		// adjust like or dislike
	}
	else {
		return;
	}
}

void UserLikesServiceHandler::GetUsersLikedMovies(std::vector<std::string>& _return, const std::string& user_id) {

    // Get mongo client
    mongoc_client_t *mongodb_client = mongoc_client_pool_pop(_mongodb_client_pool);

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

    // Get movies rated by this user from database
    bson_t *query = bson_new();
    BSON_APPEND_UTF8(query, "user_id", user_id.c_str());

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
    const bson_t *doc;
    bool found = mongoc_cursor_next(cursor, &doc);

    if (found) {
       auto recommendations_json_char = bson_as_json(doc, nullptr);
       json recommendations_json = json::parse(recommendations_json_char);

       _return.push_back(recommendations_json["movie_id"]);
    }
    // Cleanup mongo
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
    mongoc_cleanup ();

	// TO DO: look up movies with value 1 in like position
//	_return.push_back("123");
//	_return.push_back("abc");
}

int64_t UserLikesServiceHandler::GetMovieRating(const std::string& movie_id) {
	// TO DO: look up movie's overall rating (likes - dislikes)
	return 5;
}

void UserLikesServiceHandler::UserWatchMovie(const std::string& user_id, const std::string& movie_id) {
	// TO DO: put 0 in rating position for user and movie
	if (user_id != "" && movie_id != "") {
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

void UserLikesServiceHandler::GetUserID(std::string& _return, const std::string& user_name) {
	// TO DO: get user ID based on username
	_return = "8473";
}

} // namespace movies


#endif //MOVIES_MICROSERVICES_USERLIKESHANDLER_H

