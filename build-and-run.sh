#!/usr/bin/bash
set -x

docker compose build --progress plain
docker compose up