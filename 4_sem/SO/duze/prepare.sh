cd /usr/src
patch -p0 < ws374078.patch
cd /usr/src/minix/
make && make install
