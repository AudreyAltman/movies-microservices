namespace cpp movies
namespace py movies
namespace lua movies


enum ErrorCode {
  SE_CONNPOOL_TIMEOUT,
  SE_THRIFT_CONN_ERROR,
  SE_UNAUTHORIZED,
  SE_MEMCACHED_ERROR,
  SE_MONGODB_ERROR,
  SE_REDIS_ERROR,
  SE_THRIFT_HANDLER_ERROR,
  SE_RABBITMQ_CONN_ERROR
}

exception ServiceException {
    1: ErrorCode errorCode;
    2: string message;
}

service RecommenderService{
  void UploadRecommendations(
    1: i64 user_id;
    2: list<string> movie_id;
  )throws (1: ServiceException se)

  list<string> GetRecommendations(
    1: i64 user
  )throws (1: ServiceException se)
}

service MovieInfoService{
  list<string> GetMoviesByIds(
    1: list<string> movie_ids
  )

  list<string> GetMoviesByTitle(
    1: string movie_string
  )throws (1: ServiceException se)
}

service UserLikesService{
   void UserRateMovie(
     1: i64 user_id
     2: string movie_id
     3: i64 likeDislike
   ) throws (1: ServiceException se)

   list<string> GetUsersLikedMovies(
     1: i64 user_id
   ) throws (1: ServiceException se)

   i64 GetMovieRating(
     1: string movie_id
   ) throws (1: ServiceException se)

   void UserWatchMovie(
     1: i64 user_id
     2: string movie_id
   ) throws (1: ServiceException se)

   void AddUser(
     1: string user_name
   ) throws (1: ServiceException se)

   i64 GetUserID(
     1: string user_name
   ) throws (1: ServiceException se)
}
