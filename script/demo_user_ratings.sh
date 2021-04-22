curl -d "user_id=1&movie_id=MOV00001" localhost:8080/user_rating/watch_movie
curl -d "user_id=1&movie_id=MOV00020" localhost:8080/user_rating/watch_movie
curl -d "user_id=1&movie_id=MOV00354" localhost:8080/user_rating/watch_movie

curl -d "user_id=3&movie_id=MOV00020" localhost:8080/user_rating/watch_movie
curl -d "user_id=3&movie_id=MOV00354" localhost:8080/user_rating/watch_movie

curl -d "user_id=1&movie_id=MOV00001&rating=1" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=MOV00020&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=3&movie_id=MOV00354&rating=3" localhost:8080/user_rating/rate_movie

curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=3" localhost:8080/user_rating/get_user_likes

curl -d "user_id=1&movie_id=MOV00001&rating=3" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=MOV00354&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=3&movie_id=MOV00020&rating=2" localhost:8080/user_rating/rate_movie

curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=3" localhost:8080/user_rating/get_user_likes

curl -d "user_id=3&movie_id=MOV00020&rating=3" localhost:8080/user_rating/rate_movie

curl -d "user_id=1" localhost:8080/user_rating/get_user_likes
curl -d "user_id=3" localhost:8080/user_rating/get_user_likes

