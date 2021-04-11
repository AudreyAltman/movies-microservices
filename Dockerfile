FROM yg397/thrift-microservice-deps:xenial

ARG NUM_CPUS=4

COPY ./ /movies-microservices

RUN apt-get autoclean;

# Install Java
RUN apt-get update && \
    apt-get install -y default-jdk && \
    apt-get clean;

# Install Scala
# RUN wget www.scala-lang.org/files/archive/scala-2.13.0.deb && \
#     dpkg -i scala*.deb;
RUN apt-get update && \
    apt-get install -y scala && \
    apt-get clean;

# Install apt-transport-https (dependency for SBT install)
RUN apt-get update && \
    apt-get install apt-transport-https ca-certificates && \
    apt-get clean;

# Install SBT
RUN echo "deb https://dl.bintray.com/sbt/debian /" | tee -a /etc/apt/sources.list.d/sbt.list && \
    apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 2EE0EA64E40A89B84B2DF73499E82A75642AC823 && \
    apt-get update && \
    apt-get install -y sbt && \
    apt-get clean;

# Build microservices
RUN cd /movies-microservices \
    && mkdir -p build \
    && cd build \
    && cmake .. \
    && make -j${NUM_CPUS} \
    && make install

WORKDIR /movies-microservices
