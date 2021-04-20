for i in `seq 20`
do
  curl -d "movie_name=Sherlock Holmes&user_id="$i http://127.0.0.1:8080/movies/watch
done
