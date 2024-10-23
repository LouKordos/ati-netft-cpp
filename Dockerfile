FROM ubuntu:24.04

RUN echo "nameserver 8.8.8.8" > /etc/resolv.conf
RUN dig google.com
RUN apt-get update -y
RUN apt-get install -y gcc-14 cmake

RUN cmake --version