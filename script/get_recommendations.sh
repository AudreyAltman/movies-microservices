for i in {1..21}
do
  curl -d "user_id="$i localhost:8080/recommendations/get
done
