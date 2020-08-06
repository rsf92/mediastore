#!/bin/bash

name=mediastore

sudo su postgres << EOF

psql -c "create user $name with password 'mediastore';"
psql -c "create database mms"
psql -c "create extension pgcrypto SCHEMA public"
psql -c "ALTER DATABASE mms OWNER TO $name"

psql -d mms -f initdb.sql

psql -d mms -c "ALTER TABLE mms_user OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_cart OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_retailer OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_format OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_genre OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_article OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_product OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_article_product OWNER TO $name"
psql -d mms -c  "ALTER TABLE mms_product_cart OWNER TO $name"

psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_user  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_cart  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_retailer  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_format  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_genre  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_article  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_product  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_article_product  TO $name"
psql -d mms -c  "GRANT ALL PRIVILEGES ON TABLE  mms_product_cart TO $name"
