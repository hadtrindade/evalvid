#!/bin/sh
ROOTDIR=/home/evalvid

# Compile MP4Box
cd ${ROOTDIR}/gpac

./configure  --enable-sdl-static
sed -i 's/-Wl --warn-common//g' config.mak
make
make install
cp -R include/gpac /usr/include
cp bin/gcc/libgpac_static.a /usr/lib #for static linking
make clean

#link lib64 to lib folder
cd /usr/local/lib
ln -s  /usr/local/lib64/* .
ldconfig

#Complile Evalvid
cd ${ROOTDIR}
make clean
make install