
system=$(uname -a)
# Compile wheels
if [[ ${system} == *"Darwin"* ]]; then
	./scripts/install_libraries.sh
else
	
	if [[ "$(docker images -q $1:$2 2> /dev/null)" == "" ]]; then
	  docker build -t $1:$2 -f Dockerfile_builder .
	fi
fi