#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <signal.h>

#include "../utils.h"
#include "UserLikesHandler.h"
#include "../utils_mongodb.h"

using json = nlohmann::json;
using apache::thrift::server::TThreadedServer;
using apache::thrift::transport::TServerSocket;
using apache::thrift::transport::TFramedTransportFactory;
using apache::thrift::protocol::TBinaryProtocolFactory;

using namespace movies;

int64_t UserLikesServiceHandler::id_counter = 0;

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
  int my_port = config_json["user-likes-service"]["port"];

  // Get mongodb client pool
    mongoc_client_pool_t* mongodb_client_pool =
       init_mongodb_client_pool(config_json, "user-likes", 128);

    if (mongodb_client_pool == nullptr) {
       return EXIT_FAILURE;
    }

    mongoc_client_t *mongodb_client = mongoc_client_pool_pop(mongodb_client_pool);
      if (!mongodb_client) {
        LOG(fatal) << "Failed to pop mongoc client";
        return EXIT_FAILURE;
      }

      bool r = false;
      while (!r) {
        r = CreateIndex(mongodb_client, "user-likes", "user_id", true);
        if (!r) {
          LOG(error) << "Failed to create mongodb index, try again";
          sleep(1);
        }
      }

      r = false;
      while (!r) {
        r = CreateIndex(mongodb_client, "user-likes", "movie_id", true);
        if (!r) {
          LOG(error) << "Failed to create mongodb index for movie-likes db, try again";
          sleep(1);
        }
      }

      // Note from Audrey - this isn't causing any terminal errors, but I'm not sure it's creating an index.
      r = false;
      while (!r) {
        r = CreateIndex(mongodb_client, "users", "user_id", true);
        if (!r) {
          LOG(error) << "Failed to create mongodb index for users db, try again";
          sleep(1);
        }
      }

      mongoc_client_pool_push(mongodb_client_pool, mongodb_client);

  // 4: configure this server
  TThreadedServer server(
      std::make_shared<UserLikesServiceProcessor>(
          std::make_shared<UserLikesServiceHandler>(
          mongodb_client_pool)),
      std::make_shared<TServerSocket>("0.0.0.0", my_port),
      std::make_shared<TFramedTransportFactory>(),
      std::make_shared<TBinaryProtocolFactory>()
  );

  // 5: start the server
  std::cout << "Starting the user likes server ..." << std::endl;
  server.serve();
  return 0;
}
