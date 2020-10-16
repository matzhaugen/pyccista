PLAT=manylinux1_x86_64
DOCKER_IMAGE=quay.io/pypa/manylinux2014_x86_64
REPOSITORY_URL=https://test.pypi.org/legacy/
TAG=1
.PHONY: build concord test
develop:
	pip3.8 uninstall -y pyconcord &&\
	pip3.8 install -r dev_requirements.txt &&\
	python3.8 setup.py develop build_ext -I eigen-3.3.7 bdist_wheel &&\
	pip3.8 install pyconcord --no-index -f dist/pyconcord-0.3-cp38-cp38-macosx_10_15_x86_64.whl
build:
	python setup.py build_ext -I eigen-3.3.7 bdist_wheel
install:
	docker build -t matzhaugen/concord-builder:$(TAG) -f Dockerfile_builder .
concord:
	docker build -t matzhaugen/concord:$(TAG) -f Dockerfile_concord .
run:
	docker build -t matzhaugen/concord:$(TAG) . &&\
	docker run -it matzhaugen/concord:$(TAG) bash
upload:
	docker build -f Dockerfile_twine -t twine . 
	docker run -e TWINE_TOKEN_TEST=${TWINE_TOKEN_TEST} -e REPOSITORY_URL=${REPOSITORY_URL} twine scripts/upload_package.sh