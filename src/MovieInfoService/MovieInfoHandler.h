#ifndef MOVIES_MICROSERVICES_MOVIEINFOHANDLER_H
#define MOVIES_MICROSERVICES_MOVIEINFOHANDLER_H

#include <iostream>
#include <string>
#include <regex>
#include <future>
#include <mongoc.h>
#include <bson/bson.h>

#include "../../gen-cpp/MovieInfoService.h"
#include "../../gen-cpp/UserLikesService.h"

#include "../ClientPool.h"
#include "../ThriftClient.h"
#include "../logger.h"

namespace movies{

class MovieInfoServiceHandler : public MovieInfoServiceIf {
 public:
  MovieInfoServiceHandler(mongoc_client_pool_t *,mongoc_client_t *,ClientPool<ThriftClient<UserLikesServiceClient>> *);
  ~MovieInfoServiceHandler() override=default;

  void GetMoviesByIds(std::vector<std::string>& _return, const std::vector<std::string> & movie_ids) override;
  void GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string, const int64_t user_id) override;
  void UploadMovies(std::string& _return, const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links) override;
  void GetMovieLink(std::string& _return, const std::string& movie_name, const int64_t user_id) override;
 private:
    mongoc_client_pool_t *_mongodb_client_pool;
    mongoc_client_t *mongodb_client;
    ClientPool<ThriftClient<UserLikesServiceClient>> *_user_likes_client_pool;
};
    // Constructor
MovieInfoServiceHandler::MovieInfoServiceHandler(mongoc_client_pool_t *mongodb_client_pool, mongoc_client_t *_mongodb_client, ClientPool<ThriftClient<UserLikesServiceClient>> *user_likes_client_pool) {
 // Storing the clientpool
       _mongodb_client_pool = mongodb_client_pool;
       mongodb_client = _mongodb_client;
       _user_likes_client_pool = user_likes_client_pool;
}
	
 void MovieInfoServiceHandler::GetMovieLink(std::string& _return, const std::string& movie_name, const int64_t user_id){
    std::cout << "************** Inside Get Movie Link *************** !!!!!!!! ..." << std::endl;
    std::cout << "************** Inside Get : "<< user_id << std::endl;
 
    auto collection = mongoc_client_get_collection(
         mongodb_client, "movies", "movie-info");

          if (!collection) {
          ServiceException se;
          se.errorCode = ErrorCode::SE_MONGODB_ERROR;
          se.message = "Failed to create collection user from DB recommender";
          mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
          throw se;
           }
	 // Get the user likes service client pool
        auto user_likes_client_wrapper = _user_likes_client_pool->Pop();
        if (!user_likes_client_wrapper) {
            ServiceException se;
            se.errorCode = ErrorCode::SE_THRIFT_CONN_ERROR;
            se.message = "Failed to connect to user-likes-service";
            throw se;
        }
        auto user_likes_client = user_likes_client_wrapper->GetClient();

	  bson_t *query = bson_new();
          BSON_APPEND_UTF8 (query, "movie_title", movie_name.c_str());

          mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
          const bson_t *doc;
          bool found = mongoc_cursor_next(cursor, &doc);

          if (found) {
              auto movietitle_json_char = bson_as_json(doc, nullptr);
              json movietitle_json = json::parse(movietitle_json_char);
               for (auto &titlev : movietitle_json["movie_link"]) {
                       std::cout << "movie link is ------>>>>  !!!!!!! ..."<< titlev <<"  " << std::endl;
                         _return = titlev;
                     }
	       for (auto &movie_id : movietitle_json["movie_id"]) {
                   std::cout << "movie id is ------>>>>  !!!!!!! ..."<< movie_id <<"  " << std::endl;
               	   try {
            	      user_likes_client->UserWatchMovie(user_id, movie_id);
           	   } catch (...) {
            	      _user_likes_client_pool->Push(user_likes_client_wrapper);
            	      LOG(error) << "Failed to send call UserWatchMovie to user-likes-client";
            	      throw;
       		   }
                _user_likes_client_pool->Push(user_likes_client_wrapper);
                std::cout << " After calling UserWatchMovie "<< std::endl;
	        }
               mongoc_cursor_destroy(cursor);
               mongoc_collection_destroy(collection);
               mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
             }else {
        	_return = "No Movie link found ";         
	 }
   }
	
 void MovieInfoServiceHandler::UploadMovies(std::string& _return, const std::vector<std::string> & movie_ids, const std::vector<std::string> & movie_titles, const std::vector<std::string> & movie_links){
         std::cout << "************** Inside upload movies *************** !!!!!!!! ..." << std::endl;
         std::cout << "************** Size is *********** " << movie_ids.size() << std::endl;
	 auto collection = mongoc_client_get_collection(
         mongodb_client, "movies", "movie-info");
	  if (!collection) {
 	          ServiceException se;
  	          se.errorCode = ErrorCode::SE_MONGODB_ERROR;
 	          se.message = "Failed to create collection user from DB recommender";
 	          mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
  	          throw se;
 		 }

	mongoc_bulk_operation_t *bulk;
 	 bson_error_t error;
	 bool ret;
	 int i;

	 bulk = mongoc_collection_create_bulk_operation_with_opts (collection, NULL);

	 for (i = 0; i < movie_ids.size(); i++) {
	  bson_t *movie_doc = bson_new();
	  std::string idv = movie_ids[i];
          std::string titlev = movie_titles[i];
	  std::string linkv = movie_links[i];
	 
	  BSON_APPEND_UTF8(movie_doc, "movie_id", idv.c_str());
          BSON_APPEND_UTF8(movie_doc, "movie_title", titlev.c_str());
	  BSON_APPEND_UTF8(movie_doc, "movie_link", linkv.c_str());

	  mongoc_bulk_operation_insert (bulk, movie_doc);
	  std::cout << "INSERT **************************** "<< titlev.c_str() << " done" <<std::endl;
	  bson_destroy(movie_doc);
	 }
	     std::cout << "BEFORE BULK DONE IN UPLOAD !!!!!!! ..." << std::endl;

	  ret = mongoc_bulk_operation_execute (bulk, NULL, &error);
	    if (!ret) {
                  std::cout << "Movies data Insert failed ..." << std::endl;
		  ServiceException se;
                  se.errorCode = ErrorCode::SE_MONGODB_ERROR;
                  se.message = error.message;
		  throw se;
                  }
              std::cout << "DATA BULK INSERT DONE0 !!!!!!!! ..." << std::endl;

              mongoc_collection_destroy(collection);
              mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
              mongoc_cleanup ();

  _return = "Movies Uploaded Successfully"; 
 }
 
 void MovieInfoServiceHandler::GetMoviesByTitle(std::vector<std::string> & _return, const std::string& movie_string, const int64_t user_id){
	 std::cout << "************** Inside get movies by title *************** !!!!!!!! ..." << movie_string <<" end "<< std::endl;
	 std::cout << "************** User_id is  *******" << user_id <<" end "<< std::endl;
	 
	 auto collection = mongoc_client_get_collection(
		         mongodb_client, "movies", "movie-info");

          if (!collection) {
           ServiceException se;
           se.errorCode = ErrorCode::SE_MONGODB_ERROR;
           se.message = "Failed to create collection user from DB recommender";
            mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
           throw se;
                  }

          bson_t *query = bson_new();
	  const std::string& i = "i"; 
     	  bson_append_regex(query,"movie_title", -1, movie_string.c_str(),i.c_str());
	  mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
	  
       	  const bson_t *doc;
	  while (mongoc_cursor_next (cursor, &doc)) {

            auto movietitle_json_char = bson_as_json(doc, nullptr);
	    json movietitle_json = json::parse(movietitle_json_char);
	      for (auto &titlev : movietitle_json["movie_title"]) {
	     std::cout << "movie found is ------>>>>  !!!!!!! ..."<< titlev <<"  " << std::endl;
	    _return.push_back(titlev);
	       }
	      doc = NULL;
	  }
	    mongoc_cursor_destroy(cursor);
	   mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);	
}

// Remote Procedure "GetMoviesById"

void MovieInfoServiceHandler::GetMoviesByIds(std::vector<std::string>& _return, const std::vector<std::string> & movie_ids) {
    // This is a placeholder, used to confirm that RecommenderService can communicate properly with MovieInfoService
    // Once the MongoDB is up and running, this should return movie titles that match the given ids.
	std::cout << " INSIDE get movies by IDS !!!!!!!! ..." << std::endl; 
	  
	auto collection = mongoc_client_get_collection(
         mongodb_client, "movies", "movie-info");

	  if (!collection) {
          ServiceException se;
          se.errorCode = ErrorCode::SE_MONGODB_ERROR;
          se.message = "Failed to create collection user from DB recommender";
          mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);
          throw se;
		 }

  
	  for (auto &movie_id : movie_ids){
          bson_t *query = bson_new();
         
	  std::cout << "movie id inside is ------->> " << movie_id.c_str() << std::endl;
	  BSON_APPEND_UTF8 (query, "movie_id", movie_id.c_str());

          mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, nullptr, nullptr);
          const bson_t *doc;
          bool found = mongoc_cursor_next(cursor, &doc);

	  if (found) {
	      std::cout << "movies found !!!!!!!! ..." << std::endl;
	      auto movietitle_json_char = bson_as_json(doc, nullptr);
	      json movietitle_json = json::parse(movietitle_json_char);
	       for (auto &titlev : movietitle_json["movie_title"]) {
		       std::cout << "movie found is ------>>>>  !!!!!!! ..."<< titlev <<"  " << std::endl;
	                 _return.push_back(titlev);
		        }
	      mongoc_cursor_destroy(cursor);
	       }
        }
            mongoc_collection_destroy(collection);
            mongoc_client_pool_push(_mongodb_client_pool, mongodb_client);

	}
} // namespace movies


#endif //MOVIES_MICROSERVICES_MOVIEINFOHANDLER_H

