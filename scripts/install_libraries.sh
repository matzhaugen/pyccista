
mkdir tarfiles #&& yum install -y pcre-devel
curl -L https://gitlab.com/libeigen/eigen/-/archive/3.3.7/eigen-3.3.7.tar.gz -o tarfiles/eigen-3.3.7.tar.gz &&\
tar -xzvf tarfiles/eigen-3.3.7.tar.gz > /dev/null 2>&1

curl -L https://sourceforge.net/projects/swig/files/swig/swig-3.0.12/swig-3.0.12.tar.gz/download?use_mirror=iweb -o tarfiles/swig-3.0.12.tar.gz  &&\
tar -xzvf tarfiles/swig-3.0.12.tar.gz > /dev/null 2>&1 &&\
# cd swig-3.0.12 && ./configure > /dev/null 2>&1 && make -j4 > /dev/null 2>&1 && make install -j4 > /dev/null 2>&1 && cd ..
cd swig-3.0.12 && ./configure --without-pcre --prefix=$HOME/.local && make -j4 && make install -j4 && cd ..

# tar -xzvf tarfiles/libffi-3.3.tar.gz &&\
# cd libffi-3.3 && ./configure && make -j4 && make install -j4 && cd ..
