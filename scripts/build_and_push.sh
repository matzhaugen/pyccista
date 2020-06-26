system=$(uname -a)
#!/bin/bash
if [[ ${system} == *"Darwin"* ]]; then
	./scripts/build_wheels.sh
	python3 -m twine upload --skip-existing --repository-url ${REPOSITORY_URL} dist/*
else
	docker build -t matzhaugen/concord:$TAG -f Dockerfile_concord . &&\
	docker build -t matzhaugen/twine -f Dockerfile_twine  . >/dev/null 2>&1 &&\
	docker run -e TWINE_USERNAME=$TWINE_USERNAME -e TWINE_PASSWORD=$TWINE_PASSWORD -e REPOSITORY_URL=$REPOSITORY_URL matzhaugen/twine scripts/upload_package.sh
fi