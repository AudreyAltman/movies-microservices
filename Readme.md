# Movie Microservices

This is a project for CIS 578.

Audrey Altman, Vishakha Kumar, and Nicole Stark

# Pre-requirements

- Docker
- Docker-compose
- Python 3.5+ (with asyncio and aiohttp)
- libssl-dev (apt-get install libssl-dev)
- libz-dev (apt-get install libz-dev)
- luarocks (apt-get install luarocks)
- luasocket (luarocks install luasocket)
- mongoc
-- (Installation instructions: http://mongoc.org/libmongoc/current/installing.html)
- Thrift 0.14.0 (./configure --with-lua --with-cpp --with-py)
-- (Thrift tutorial: https://www.youtube.com/watch?v=NK6hz2JM89w)

# Use
Run docker-compose script to run all the containers:
$ docker-compose up

Seed the database with default recommendations for each user:
$ sh script/upload_recommendation.sh

Check the default recommendations:
$ sh script/get_recommendations.sh

Add user ratings for various movies:
$ sh sh script/upload_user_rating.sh

Run a batch job to generate new recommedations based on user ratings:
$ sh script/run-batch-job.sh

Check the updated recommendations:
$ sh script/get_recommendations.sh

Stop docker containers that started from docker-compose script:
$ docker-compose down

# Sources

Parts of this codebase were adaped from:
- https://github.com/delimitrou/DeathStarBench/tree/master/mediaMicroservices
- https://github.com/proywm/vendingmachine_tutorial

# Cheatsheet on thrift 

To compile vending_machine.thrift:

thrift --gen cpp vending_machine.thrift

thrift --gen py vending_machine.thrift

thrift --gen lua vending_machine.thrift

# cheatsheet on docker commands

1. To create docker image from docker file:
$ docker build -t yourusername/repository-image-name .
2. List docker images:
$ docker images -a
3. Retag the image with a version number: 
$ docker tag yourusername/repository-image-name yourdockerhubusername/repository-image-name:version_1
4. Docker-Hub login:
$ docker login
5. Push the local image to docker-hub
$ docker push abiodunjames/example-node-app:v1 
6. List docker containers:
$ docker ps -a
7. Clean up old volumes
$ docker volume prune

Ref: 
1. https://stackify.com/docker-build-a-beginners-guide-to-building-docker-images/
2. https://www.digitalocean.com/community/tutorials/how-to-remove-docker-images-containers-and-volumes

# cheatsheet on docker-compose

1. Run docker-compose script to run all the containers:
$ docker-compose up
2. Stop docker containers that started from docker-compose script:
$ docker-compose down

# copy and paste functions for cmd
Build and run commands:
1. sudo docker build -t audreyaltman/movies-project .
2. sudo docker push audreyaltman/movies-project:latest
3. sudo docker-compose up
4. sh ./script/<script-name>
5. sudo docker-compose down
 
# Cleanup:
Remove exited docker images: 
$ sudo docker rm $(sudo docker ps --filter status=exited -q)

Force kill any containers which won't exit due to memory issues
$ sudo docker kill $(sudo docker ps -q)

Cleanup local docker: 
$ sudo docker volume prune

