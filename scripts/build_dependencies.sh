
docker pull $1:$2
if [[ "$(docker images -q $1:$2 2> /dev/null)" == "" ]]; then
  docker build -t $1:$2 -f Dockerfile_builder .
fi