PLAT=manylinux1_x86_64
DOCKER_IMAGE=quay.io/pypa/manylinux2014_x86_64
TAG=1
.PHONY: build
build:
	docker build -t matzhaugen/concord:$(TAG) .
run:
	docker build -t matzhaugen/concord:$(TAG) . &&\
	docker run -it matzhaugen/concord:$(TAG) bash
install:
	docker run -it --rm -e PLAT=${PLAT} -v `pwd`:/io ${DOCKER_IMAGE} bash #/io/travis/build-wheels.sh