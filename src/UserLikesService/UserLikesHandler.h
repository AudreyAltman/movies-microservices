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

namespace movies{

class UserLikesServiceHandler : public UserLikesServiceIf {
 public:
  UserLikesServiceHandler();
  ~UserLikesServiceHandler() override=default;

  void GetMovieLikesByIds(std::vector<int64_t>& _return, const std::vector<std::string> & movie_ids) override;
  void UserRateMovie(const std::string& user_id, const std::string& movie_id, const int64_t likeDislike) override;
  void GetUsersLikedMovies(std::vector<std::string>& _return, const std::string& user_id) override;
  int64_t GetMovieRating(const std::string& movie_id) override;
  void UserWatchMovie(const std::string& user_id, const std::string& movie_id) override;
  void AddUser(const std::string& user_name) override;
  void GetUserID(std::string& _return, const std::string& user_name) override;
};

// Constructor
UserLikesServiceHandler::UserLikesServiceHandler() {

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
		// adjust like or dislike
	}
	else {
		return;
	}
}

void UserLikesServiceHandler::GetUsersLikedMovies(std::vector<std::string>& _return, const std::string& user_id) {
	// TO DO: look up movies with value 1 in like position
	_return.push_back("123");
	_return.push_back("abc");
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

