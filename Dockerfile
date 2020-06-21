FROM quay.io/pypa/manylinux2014_x86_64 as builder
COPY . /io
WORKDIR /io
ENV PLAT=manylinux1_x86_64
# Install concord
RUN travis/install_libraries.sh
RUN travis/build_wheels.sh

FROM python:3.8-slim
COPY --from=builder /io/wheelhouse .
