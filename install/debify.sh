#!/bin/bash
cd ..
make mediaServer
BINSIZE=$(ls -lh   mediaServer | cut -d " " -f5)
mkdir -p deb/DEBIAN
mkdir -p deb/etc/mms/db/
mkdir deb/etc/mms/network
mkdir -p deb/usr/bin/
mv mediaServer deb/usr/bin/
touch deb/DEBIAN/control
touch deb/DEBIAN/conffiles
touch deb/DEBIAN/md5sums
touch deb/etc/mms/network/config.conf
touch deb/etc/mms/db/data.conf

echo "Package: mediaServer" >> deb/DEBIAN/control
echo "Version: "$1 >> deb/DEBIAN/control
echo "Architecture: all" >> deb/DEBIAN/control
echo "Essential: no" >> deb/DEBIAN/control
echo "Priority: optional" >> deb/DEBIAN/control
#echo "Depends: database (>=0.1)" >> deb/DEBIAN/control
echo "Maintainer: Roberto Sánchez" >> deb/DEBIAN/control
#echo "Size: "$BINSIZE  >> deb/DEBIAN/control
echo "Description: Main mediaServer daemon" >> deb/DEBIAN/control
md5sum deb/usr/bin/mediaServer/mediaServer > deb/DEBIAN/md5sums

echo "###" > deb/etc/mms/network/config.conf
echo "#Configuration file for networking parameters" >> deb/etc/mms/network/config.conf
echo "#PORT 2420" >> deb/etc/mms/network/config.conf
echo "#nLISTEN_QUEUE 50" >> deb/etc/mms/network/config.conf
echo "#SSL related network parameters" >> deb/etc/mms/network/config.conf
echo "#HTTPS yes" >> deb/etc/mms/network/config.conf
echo "###" >> deb/etc/mms/network/config.conf


echo "#DB User Data" >> deb/etc/mms/db/data.conf
echo "NAME=posgres" >> deb/etc/mms/db/data.conf
echo "PASSWD=posgres" >> deb/etc/mms/db/data.conf

dpkg -b deb/ mediaServer_$1_$(dpkg --print-architecture).deb
rm -rf deb
make clean
