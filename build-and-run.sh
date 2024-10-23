#!/usr/bin/bash
set -x

docker build --progress plain --network host -t ati-netft-cpp:latest .
docker compose up