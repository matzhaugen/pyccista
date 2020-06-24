PLAT=manylinux1_x86_64
DOCKER_IMAGE=quay.io/pypa/manylinux2014_x86_64
TAG=1
.PHONY: build concord
install:
	docker build -t matzhaugen/concord-builder:$(TAG) -f Dockerfile_builder .
concord:
	docker build -t matzhaugen/concord:$(TAG) -f Dockerfile_concord .
run:
	docker build -t matzhaugen/concord:$(TAG) . &&\
	docker run -it matzhaugen/concord:$(TAG) bash
upload:
	docker build -f Dockerfile_twine -t twine . 
	docker run -e TWINE_PASSWORD=${TWINE_PASSWORD} twine travis/upload_package.sh
test:
	docker run -it twine bash
	