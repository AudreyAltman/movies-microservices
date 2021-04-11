for i in {0..20}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=aaa&rating="$r localhost:8080/user_rating/upload
done
