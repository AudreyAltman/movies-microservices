#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include "../utils.h"
#include "../utils_mongodb.h"
#include "MovieInfoHandler.h"
#include <mongoc.h>
#include <bson/bson.h>
#include <iostream>

using json = nlohmann::json;
using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace movies;


// signal handler code
void sigintHandler(int sig) {
	exit(EXIT_SUCCESS);
}

// entry of this service
int main(int argc, char **argv) {
  // 1: notify the singal handler if interrupted
  signal(SIGINT, sigintHandler);
  // 1.1: Initialize logging
  init_logger();


  // 2: read the config file for ports and addresses
  json config_json;
  if (load_config_file("config/service-config.json", &config_json) != 0) {
    exit(EXIT_FAILURE);
  }

  // 3: get my port
  int my_port = config_json["movie-info-service"]["port"];
   
  // Get the user likes service's port and address
     int user_likes_service_port = config_json["user-likes-service"]["port"];
     std::string user_likes_service_addr = config_json["user-likes-service"]["addr"];
 
  // Get the client of user-likes-service
     ClientPool<ThriftClient<UserLikesServiceClient>> user_likes_client_pool(
      "user-likes-service", user_likes_service_addr, user_likes_service_port, 0, 128, 1000);

  // Get mongodb client pool
   mongoc_client_pool_t* mongodb_client_pool =
   init_mongodb_client_pool(config_json, "movies", 128);
        	 
        std::cout << "Mongodb client pool done ..." << std::endl;
        if (mongodb_client_pool == nullptr) {
        return EXIT_FAILURE;
    	          }
   	 	            
    
   mongoc_client_t *mongodb_client = mongoc_client_pool_pop(mongodb_client_pool);
      if (!mongodb_client) {
          LOG(fatal) << "Failed to pop mongoc client";
	        return EXIT_FAILURE;
	    }
  mongoc_client_pool_push(mongodb_client_pool, mongodb_client);

   auto collection = mongoc_client_get_collection(
         mongodb_client, "movies", "movie-info");
	  if (!collection) {
 	          ServiceException se;
  	          se.errorCode = ErrorCode::SE_MONGODB_ERROR;
 	          se.message = "Failed to create collection user from DB recommender";
 	          mongoc_client_pool_push(mongodb_client_pool, mongodb_client);
  	          throw se;
 		 }
           
  // 4: configure this server
  TThreadedServer server(
      std::make_shared<MovieInfoServiceProcessor>(
      std::make_shared<MovieInfoServiceHandler>(mongodb_client_pool, mongodb_client, &user_likes_client_pool)),
      std::make_shared<TServerSocket>("0.0.0.0", my_port),
      std::make_shared<TFramedTransportFactory>(),
      std::make_shared<TBinaryProtocolFactory>()
  );
  
  // 5: start the server
  std::cout << "Starting the movie info server ..." << std::endl;
  server.serve();
  return 0;
}

