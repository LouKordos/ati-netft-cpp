FROM ubuntu:24.04

RUN apt-get update -y && \
	apt-get install -y g++-14 gcc-14 cmake build-essential gdb git vim libtbb-dev && \
	rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY ./ /app

ENV CC=gcc-14
ENV CXX=g++-14

RUN cmake -DCMAKE_BUILD_TYPE=Release -S . -B build && \
	cmake --build build -j $(nproc) -v

ENTRYPOINT ["build/src/tests/sensor_test"]