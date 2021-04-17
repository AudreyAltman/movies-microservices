curl -d "user_id=1&movie_id=abc" localhost:8080/user_rating/watch_movie
curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=1&movie_id=abc&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=1" localhost:8080/user_rating/get_user_likes