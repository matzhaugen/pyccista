
system=$(uname -a)
# Compile wheels
if [[ ${system} == *"Darwin"* ]]; then
	./scripts/install_libraries.sh
else
	echo "$DOCKER_PASSWORD" | docker login -u "$DOCKER_USERNAME" --password-stdin
	if [[ "$(docker images -q $1:$2 2> /dev/null)" == "" ]]; then
		docker build -t $1:$2 -f Dockerfile_builder .
		docker push -$1:$2
	else
		docker pull matzhaugen/concord-builder:$TAG
	fi
fi