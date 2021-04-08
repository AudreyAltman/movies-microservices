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
	// TO DO: update database with user like or dislike (false == dislike, true == like)
	if (user_id != 0) {
		// adjust like or dislike
	}
	else {
		return;
	}
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
	  se.message = "Failed to create collection user from DB recommender";
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

