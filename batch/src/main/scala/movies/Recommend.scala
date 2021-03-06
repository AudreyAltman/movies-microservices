// Adapted from:
// https://github.com/mongodb/mongo-hadoop/wiki/Spark-Usage
// https://spark.apache.org/docs/latest/ml-collaborative-filtering.html

package movies

import org.apache.spark.rdd.RDD
import org.apache.spark.sql.SparkSession
import org.apache.spark.sql.functions._

import org.apache.hadoop.conf.Configuration

import org.bson.BSONObject
import org.bson.types.ObjectId
import com.mongodb.hadoop.{MongoInputFormat, MongoOutputFormat,BSONFileInputFormat, BSONFileOutputFormat}
import com.mongodb.hadoop.io.MongoUpdateWritable
import com.mongodb.{BasicDBList,BasicDBObject}

import org.apache.spark.ml.evaluation.RegressionEvaluator
import org.apache.spark.ml.recommendation.ALS

object Recommend {
  def main(args: Array[String]): Unit = {

    val spark = SparkSession.builder.appName("MoviesBatchJob").master("local").getOrCreate()
    val sc = spark.sparkContext
    import spark.implicits._

    // Set up the configuration for reading from MongoDB.
    val mongoConfig = new Configuration()
    mongoConfig.set("mongo.input.uri", "mongodb://localhost:27017/user-likes.user-likes")

    // Create a separate Configuration for saving data back to MongoDB.
    val outputConfig = new Configuration()
    outputConfig.set("mongo.output.uri", "mongodb://localhost:27018/recommender.recommender")

    // Create an RDD backed by the MongoDB collection.
    val documents = sc.newAPIHadoopRDD(
      mongoConfig,                // Configuration
      classOf[MongoInputFormat],  // InputFormat
      classOf[Object],            // Key type
      classOf[BSONObject])        // Value type

    // UDF that will convert movie_id String to Int
    val hashMovieId = udf((input: String) => input.hashCode())

    // Format data for ALS operation
    val docDF = documents.mapValues{v => (v.get("user_id"), v.get("movie_id"), v.get("rating"), v.get("_id"))}
      .map{ case(k,v) => (v._1.toString, v._2.toString, v._3.toString, k.toString)}
      .map(s => (s._1.toInt, s._2, s._3.toFloat, s._4))
      .toDF("user_id", "movie_id", "rating", "oid")
      .withColumn("movie_num", hashMovieId(col("movie_id")))

    docDF.cache

    // Build the recommendation model using ALS on the training data
    val als = new ALS()
      .setMaxIter(5)
      .setRegParam(0.01)
      .setUserCol("user_id")
      .setItemCol("movie_num")
      .setRatingCol("rating")
    val model = als.fit(docDF)

    // Get recommendations for each user
    val userRecs = model.recommendForAllUsers(5)

    userRecs.localCheckpoint

    val movie_map = docDF.select("movie_num", "movie_id").distinct.rdd.map(row => (row.getAs[Int](0), row.getAs[String](1)))
    val collected = movie_map.collect.toMap

    // Format for output
    val recs = userRecs.select(col("user_id"), explode(col("recommendations")).as("rec"))
      .withColumn("movie_num",col("rec.movie_num"))
      .drop("rec")
    val strIds = recs.rdd
      .map(row => (row.getAs[Int](0), row.getAs[Int](1))).collect
      .map(x => (x._1, collected.get(x._2).get))
    val grouped = strIds.groupBy(_._1).mapValues(_.map(_._2)).toSeq
    val movies =  sc.parallelize(grouped).toDF("uid", "movie_ids")
    val distinctIds = docDF.select("user_id", "oid").distinct
    val joined = movies.join(distinctIds, movies("uid")===distinctIds("user_id"), "inner")
      .select("oid", "user_id", "movie_ids")
    // Form of outputRDD: (oid, (user_id, movie_ids))
    val outputRDD = joined.rdd
      .map(row => (row.getString(0),(row.getAs[Int](1), row.getAs[scala.collection.mutable.WrappedArray[String]](2).toArray, row.getString(0))))

    // Create BSON output
    val updates = outputRDD.mapValues(row => {
      var blist = new BasicDBList()

      for (movie_id <- row._2) {
        blist.add(movie_id)
      }

      new MongoUpdateWritable(
        new BasicDBObject("user_id", row._1),  // Query
        new BasicDBObject("user_id", row._1).append("movie_ids", blist),  // Update operation
        true,  // Upsert
        false, // Update multiple documents
        true   // Replace
      )
    })

    // Save this RDD as a Hadoop "file".
    // The path argument is unused; all documents will go to "mongo.output.uri".
    updates.saveAsNewAPIHadoopFile(
      "file:///this-is-completely-unused",
      classOf[Object],
      classOf[MongoUpdateWritable],
      classOf[MongoOutputFormat[Object, MongoUpdateWritable]],
      outputConfig)

  }
}
