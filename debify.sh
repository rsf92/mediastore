#!/bin/bash

if [ $# -ne 1 ]; then
	echo "Usage debufy.sh VERSION"
	exit 1
fi

#BINSIZE=$(ls -lh   usr/bin/mediaServer | cut -d " " -f5)
mkdir deb
cp -r files/DEBIAN deb/
cp -r etc deb/
cp -r usr deb/
cp -r scripts deb/


sed -i s,__version__,$1,g deb/DEBIAN/control

md5sum deb/usr/bin/mediaServer > deb/DEBIAN/md5sums

dpkg -b deb/ mediaServer_$1_$(dpkg --print-architecture).deb
rm -rf deb
