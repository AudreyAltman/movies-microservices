for i in {1..20}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=aaa&rating="$r localhost:8080/user_rating/upload
done

for i in {1..21}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=bbb&rating="$r localhost:8080/user_rating/upload
done

for i in {1..11}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=ccc&rating="$r localhost:8080/user_rating/upload
done

for i in {11..21}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=ddd&rating="$r localhost:8080/user_rating/upload
done

for i in {1..6}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=eee&rating="$r localhost:8080/user_rating/upload
done

for i in {6..16}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=fff&rating="$r localhost:8080/user_rating/upload
done

for i in {16..21}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=ggg&rating="$r localhost:8080/user_rating/upload
done

for i in {1..5}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=hhh&rating="$r localhost:8080/user_rating/upload
done

for i in {17..21}
do
  r=$((1 + $RANDOM % 4))
  curl -d "user_id="$i"&movie_id=iii&rating="$r localhost:8080/user_rating/upload
done

for i in {1..21}
do
  rem=$(($i % 2))
  if [ "$rem" -ne "0" ]; then
    r=$((1 + $RANDOM % 4))
    curl -d "user_id="$i"&movie_id=jjj&rating="$r localhost:8080/user_rating/upload
  fi
done

for i in {1..21}
do
  rem=$(($i % 2))
  if [ "$rem" -eq "0" ]; then
    r=$((1 + $RANDOM % 4))
    curl -d "user_id="$i"&movie_id=kkk&rating="$r localhost:8080/user_rating/upload
  fi
done

for i in {1..21}
do
  rem=$(($i % 3))
  if [ "$rem" -ne "0" ]; then
    r=$((1 + $RANDOM % 4))
    curl -d "user_id="$i"&movie_id=lll&rating="$r localhost:8080/user_rating/upload
  fi
done

for i in {1..21}
do
  rem=$(($i % 3))
  if [ "$rem" -eq "0" ]; then
    r=$((1 + $RANDOM % 4))
    curl -d "user_id="$i"&movie_id=mmm&rating="$r localhost:8080/user_rating/upload
  fi
done