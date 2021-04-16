for i in {1..21}
do
  curl -d "user_id="$i"&movie_id=aaa&movie_id=ddd" localhost:8080/recommendations/upload
done
