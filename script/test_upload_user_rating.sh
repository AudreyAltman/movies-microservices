curl -d "user_id=1&movie_id=abc&rating=1" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=def&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=1&movie_id=xyz&rating=3" localhost:8080/user_rating/rate_movie
curl -d "user_id=2&movie_id=abc&rating=4" localhost:8080/user_rating/rate_movie
curl -d "user_id=3&movie_id=def&rating=1" localhost:8080/user_rating/rate_movie
curl -d "user_id=4&movie_id=xyz&rating=2" localhost:8080/user_rating/rate_movie
curl -d "user_id=5&movie_id=def&rating=3" localhost:8080/user_rating/rate_movie
curl -d "user_id=6&movie_id=abc&rating=4" localhost:8080/user_rating/rate_movie