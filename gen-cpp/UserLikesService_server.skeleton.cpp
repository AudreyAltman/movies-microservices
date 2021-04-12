// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "UserLikesService.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::movies;

class UserLikesServiceHandler : virtual public UserLikesServiceIf {
 public:
  UserLikesServiceHandler() {
    // Your initialization goes here
  }

  void UserRateMovie(const int64_t user_id, const std::string& movie_id, const int64_t likeDislike) {
    // Your implementation goes here
    printf("UserRateMovie\n");
  }

  void GetUsersLikedMovies(std::vector<std::string> & _return, const int64_t user_id) {
    // Your implementation goes here
    printf("GetUsersLikedMovies\n");
  }

  int64_t GetMovieRating(const std::string& movie_id) {
    // Your implementation goes here
    printf("GetMovieRating\n");
  }

  void UserWatchMovie(const int64_t user_id, const std::string& movie_id) {
    // Your implementation goes here
    printf("UserWatchMovie\n");
  }

  void AddUser(const std::string& user_name) {
    // Your implementation goes here
    printf("AddUser\n");
  }

  int64_t GetUserID(const std::string& user_name) {
    // Your implementation goes here
    printf("GetUserID\n");
  }

};

int main(int argc, char **argv) {
  int port = 9090;
  ::std::shared_ptr<UserLikesServiceHandler> handler(new UserLikesServiceHandler());
  ::std::shared_ptr<TProcessor> processor(new UserLikesServiceProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

