#!/bin/bash

if command -v apt-get >/dev/null; then
  apt-get update && apt-get install -y libeigen3-dev build-essential
elif command -v yum >/dev/null; then
  yum update && yum install -y eigen3-devel
else
  echo "I have no Idea what im doing here"
fi
