start=$SECONDS

spark/spark-2.3.2-bin-hadoop2.7/bin/spark-submit --packages org.mongodb.mongo-hadoop:mongo-hadoop-core:2.0.2 batch/target/scala-2.11/movies_2.11-0.1.jar

end=$SECONDS
duration=$(( end - start ))
echo "Process took $duration seconds to complete."