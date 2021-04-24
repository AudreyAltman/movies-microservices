start=$SECONDS

for i in {1..5000}
do
  r=$((1 + $RANDOM % 4))
  u=$((1 + $RANDOM % 1000))
  m=$((1 + $RANDOM % 9))
  curl -d "user_id="$u"&movie_id=MOV0000"$m"&rating="$r localhost:8080/user_rating/rate_movie
done

for i in {1..5000}
do
  r=$((1 + $RANDOM % 4))
  u=$((1 + $RANDOM % 1000))
  m=$((11 + $RANDOM % 9))
  curl -d "user_id="$u"&movie_id=MOV000"$m"&rating="$r localhost:8080/user_rating/rate_movie
done

end=$SECONDS
duration=$(( end - start ))
echo "Process took $duration seconds to complete."
