name := "movies"

version := "0.1"

scalaVersion := "2.11.10"

libraryDependencies ++= Seq(
  "org.apache.spark" %% "spark-core" % "2.3.2" % "provided",
  "org.apache.spark" %% "spark-sql" % "2.3.2" % "provided",
  "org.apache.spark" %% "spark-mllib" % "2.3.3" % "provided",
  "org.mongodb.mongo-hadoop" % "mongo-hadoop-core" % "2.0.2" % "provided"
)
