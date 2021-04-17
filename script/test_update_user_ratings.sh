curl -d "user_id=1&movie_id=abc&rating=1" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=def&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=xyz&rating=3" localhost:8080/user_rating/rate_movie
curl -d "user_id=3&movie_id=def&rating=1" localhost:8080/user_rating/rate_movie

curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=3" localhost:8080/user_rating/get_user_likes

curl -d "user_id=1&movie_id=abc&rating=3" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=xyz&rating=1" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=def&rating=1" localhost:8080/user_rating/rate_movie
curl -d "user_id=3&movie_id=def&rating=2" localhost:8080/user_rating/rate_movie

curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=3" localhost:8080/user_rating/get_user_likes

curl -d "user_id=1&movie_id=abc&rating=3" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=xyz&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=def&rating=2" localhost:8080/user_rating/rate_movie
curl -d "user_id=3&movie_id=def&rating=3" localhost:8080/user_rating/rate_movie

curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=3" localhost:8080/user_rating/get_user_likes

