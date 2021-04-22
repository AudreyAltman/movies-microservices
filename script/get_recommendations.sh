for i in {1..20}
do
  curl -d "user_id="$i localhost:8080/recommendations/get
done
