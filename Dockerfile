FROM ubuntu:24.04

RUN apt-get update -y
RUN apt-get install -y g++-14 gcc-14 cmake build-essential

# Install any further deps here to allow for caching

RUN mkdir -p /app/src
RUN mkdir /app/src/build
COPY ./src /app/src/
WORKDIR /app/src

ENV CC=gcc-14
ENV CXX=g++-14

RUN cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
RUN cmake --build build -j $(nproc) -v
RUN g++-14 --version

ENTRYPOINT [ "build/tests/sensor_test" ]