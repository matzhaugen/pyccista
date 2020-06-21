yum install -y curl pcre-devel
curl https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz -o eigen-3.3.7.tar.gz && \
tar -xzvf eigen-3.3.7.tar.gz

curl -L https://sourceforge.net/projects/swig/files/swig/swig-3.0.12/swig-3.0.12.tar.gz/download -o swig-3.0.12.tar.gz&&\
tar -xzvf swig-3.0.12.tar.gz &&\
cd swig-3.0.12 && ./configure && make && make install && cd ..

curl -L https://github.com/libffi/libffi/releases/download/v3.3/libffi-3.3.tar.gz -o libffi-3.3.tar.gz &&\
tar -xzvf  libffi-3.3.tar.gz &&\
cd libffi-3.3 && ./configure && make && make install && cd ..
