#!/bin/sh

echo "----- Prebuilt ----- "
echo ""


echo "----- Libmicrohttpd ----- "

rm -rf libmicrohttpd
mkdir libmicrohttpd
cd libmicrohttpd
wget https://ftpmirror.gnu.org/libmicrohttpd/libmicrohttpd-latest.tar.gz
tar zxvf libmicrohttpd-latest.tar.gz
rm -f libmicrohttpd-latest.tar.gz
cd libmicrohttpd*
./configure
make -j16
cp ./src/microhttpd/.libs/libmicrohttpd.a ../../
mkdir ../../include
ln -s $(pwd)/src/include ../../include/microhttpd
cd ../../
rm -f libmicrohttpd-latest.tar.gz

echo "----- Libmicrohttpd End ----- "
