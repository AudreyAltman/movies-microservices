for i in `seq 25`
do
     rem=$(($i % 2))
        if [ "$rem" -ne "0" ]; then
                curl -d "movie_name=Sinister&user_id="$i http://127.0.0.1:8080/movies/watch
           else
		curl -d "movie_name=Conjuring&user_id="$i http://127.0.0.1:8080/movies/watch
        fi
done
