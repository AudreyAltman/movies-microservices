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

}

// Remote Procedure "GetMovieRating"
int64_t UserLikesServiceHandler::GetMovieRating(const std::string& movie_id) {

}


// Remote Procedure "UserWatchMovie"
void UserLikesServiceHandler::UserWatchMovie(const int64_t user_id, const std::string& movie_id) {

}

// Remote Procedure "AddUser"
void UserLikesServiceHandler::AddUser(const std::string& user_name) {

}

// Remote Procedure "GetUserID"
int64_t UserLikesServiceHandler::GetUserID(const std::string& user_name) {

}

} // namespace movies


#endif //MOVIES_MICROSERVICES_USERLIKESHANDLER_H

