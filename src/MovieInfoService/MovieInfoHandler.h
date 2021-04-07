#ifndef MOVIES_MICROSERVICES_MOVIEINFOHANDLER_H
#define MOVIES_MICROSERVICES_MOVIEINFOHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>
#include <mongoc.h>
#include <bson/bson.h>

#include "../../gen-cpp/MovieInfoService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class MovieInfoServiceHandler : public MovieInfoServiceIf {
 public:
  MovieInfoServiceHandler(mongoc_client_pool_t *);
  ~MovieInfoServiceHandler() override=default;

  void GetMoviesByIds(std::vector<std::string>& _return, const std::vector<std::string> & movie_ids) override;
  void GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string) override;
 private:
    mongoc_client_pool_t *_mongodb_client_pool;
};

// Constructor
MovieInfoServiceHandler::MovieInfoServiceHandler(mongoc_client_pool_t *mongodb_client_pool) {
 // Storing the clientpool
       _mongodb_client_pool = mongodb_client_pool;
}
	
 void MovieInfoServiceHandler::GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string){

         _return.push_back("Wonder Woman");	
}

// Remote Procedure "GetMoviesById"

void MovieInfoServiceHandler::GetMoviesByIds(std::vector<std::string>& _return, const std::vector<std::string> & movie_ids) {
    // This is a placeholder, used to confirm that RecommenderService can communicate properly with MovieInfoService
    // Once the MongoDB is up and running, this should return movie titles that match the given ids.
 for (auto &movie_id : movie_ids) {
		 _return.push_back("MovieInfoService name for: " + movie_id);
		 }
     }

} // namespace movies


#endif //MOVIES_MICROSERVICES_MOVIEINFOHANDLER_H

