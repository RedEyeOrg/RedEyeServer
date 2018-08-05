git clone https://github.com/babelouest/ulfius
cd ulfius
git submodule update --init
sudo apt-get install libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev
cd ulfius
cd lib/orcania
make && sudo make install
cd ../yder
make && sudo make install
cd ../../
sudo make install
