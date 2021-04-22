for i in {1..20}
do
  curl -d "user_id="$i"&movie_id=MOV00004&movie_id=MOV00008" localhost:8080/recommendations/upload
done
