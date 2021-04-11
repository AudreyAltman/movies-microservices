// Adapted from:
// https://github.com/mongodb/mongo-hadoop/wiki/Spark-Usage
// https://spark.apache.org/docs/latest/ml-collaborative-filtering.html

package movies

import org.apache.hadoop.conf.Configuration
import org.apache.spark.{SparkContext, SparkConf}
import org.apache.spark.rdd.RDD

import org.bson.BSONObject
import com.mongodb.hadoop.{MongoInputFormat, MongoOutputFormat,BSONFileInputFormat, BSONFileOutputFormat}
import com.mongodb.hadoop.io.MongoUpdateWritable

import org.apache.spark.ml.evaluation.RegressionEvaluator
import org.apache.spark.ml.recommendation.ALS

object Recommend {
  def main(args: Array[String]): Unit = {

    // Set up the configuration for reading from MongoDB.
    val mongoConfig = new Configuration()
    mongoConfig.set("mongo.input.uri", "mongodb://localhost:27017/user-likes.user-likes")

    val sparkConf = new SparkConf()
    val sc = new SparkContext("local", "MoviesBatchJob", sparkConf)

    // Create an RDD backed by the MongoDB collection.
    val documents = sc.newAPIHadoopRDD(
      mongoConfig,                // Configuration
      classOf[MongoInputFormat],  // InputFormat
      classOf[Object],            // Key type
      classOf[BSONObject])        // Value type

    // Convert RDD of BSONObject into Dataframe
    val docDF = documents.mapValues{v => (v.get("user_id"), v.get("movie_id"), v.get("rating"))}
      .map(_._2)
      .map(o => (o._1.toString, o._2.toString, o._3.toString))
      .map(s => (s._1.toInt, s._2, s._3.toFloat))
      .toDF("user_id", "movie_id", "rating")

    // Build the recommendation model using ALS on the training data
    val als = new ALS()
      .setMaxIter(5)
      .setRegParam(0.01)
      .setUserCol("user_id")
      .setItemCol("movie_id")
      .setRatingCol("rating")
    val model = als.fit(docDF)

//    // Create a separate Configuration for saving data back to MongoDB.
//    val outputConfig = new Configuration()
//    outputConfig.set("mongo.output.uri",
//      "mongodb://localhost:27017/recommender-mongodb")
//
//    // Save this RDD as a Hadoop "file".
//    // The path argument is unused; all documents will go to "mongo.output.uri".
//    documents.saveAsNewAPIHadoopFile(
//      "file:///this-is-completely-unused",
//      classOf[Object],
//      classOf[BSONObject],
//      classOf[MongoOutputFormat[Object, BSONObject]],
//      outputConfig)
  }
}
