#!/bin/bash
mkdir -p sources
mkdir -p usr/bin
mkdir scripts
mkdir etc
git archive develop -o sources/develop.tar
cd sources
tar -xf develop.tar
rm develop.tar
make mediaServer
mv mediaServer ../usr/bin/
cp -r config/mms/ ../etc/
cp config/config_psql.sh ../scripts
cp config/initdb.sql ../scripts
cp config/install_deps.sh ../scripts
cp config/create_user_group.sh ../scripts
