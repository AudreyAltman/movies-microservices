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



struct location{
	1: i64 location_id;
	2: i64 city;
}

struct Movie{
  1: i64 movie_id;
  2: string title;
}

enum WeatherType{
	WARM,
	COLD
}

enum BeverageType{
	HOT,
	COLD
}

service RecommenderService{
  list<Movie> GetRecommendations(
    1: i64 user
  )throws (1: ServiceException se)
}

service OrderBeverageService{
	string PlaceOrder(
		1: i64 city
	)throws (1: ServiceException se)
}

service WeatherService{
	WeatherType GetWeather(
		1: i64 city
	)
}
