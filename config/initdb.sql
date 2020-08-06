-- TABLES CREATION

CREATE TABLE IF NOT EXISTS mms_user (
    uid serial,
    mail text NOT NULL UNIQUE,
    passwd text NOT NULL,
    usertype integer NOT NULL,
    name text NOT NULL,
    birth date NOT NULL,
    is_online bool NOT NULL,
    PRIMARY KEY (uid)
  );

CREATE TABLE IF NOT EXISTS mms_cart (
	cid serial,
  uid integer REFERENCES mms_user(uid),
	last_updated date NOT NULL,
	cart_closed boolean NOT NULL,
	PRIMARY KEY (cid)
    );

CREATE TABLE IF NOT EXISTS mms_retailer (
	rid serial,
	mail text NOT NULL UNIQUE,
	PRIMARY KEY (rid)
);

CREATE TABLE IF NOT EXISTS mms_format (
	fid serial,
	physical boolean NOT NULL,
	PRIMARY KEY (fid)
);

CREATE TABLE IF NOT EXISTS mms_genre (
	gid serial,
	genre text NOT NULL,
	PRIMARY KEY (gid)
    );

 CREATE TABLE IF NOT EXISTS mms_article (
	aid serial,
	name text NOT NULL,
	description text NOT NULL,
	author text NOT NULL,
	fid integer NOT NULL REFERENCES mms_format(fid),
	article_id text NOT NULL,
	retailer_id integer NOT NULL REFERENCES mms_retailer(rid),
	price float NOT NULL,
	stock integer NOT NULL,
	release_date date NOT NULL,
	PRIMARY KEY (aid)
    );

 CREATE TABLE IF NOT EXISTS mms_product (
	pid serial,
	name text NOT NULL,
	description text NOT NULL,
	genre integer NOT NULL REFERENCES mms_genre(gid),
	type integer NOT NULL,
	discount float NOT NULL,
	PRIMARY KEY (pid)
    );

 CREATE TABLE IF NOT EXISTS mms_article_product (
	pid integer REFERENCES mms_product(pid),
	aid integer REFERENCES mms_article(aid),
	PRIMARY KEY (pid,aid)
    );

  CREATE TABLE IF NOT EXISTS mms_product_cart (
	pid integer REFERENCES mms_product(pid),
	cid integer REFERENCES mms_cart(cid),
	quantity integer NOT NULL,
	price float NOT NULL,
	PRIMARY KEY (pid,cid)
    );

 -- FUNCTIONS CREATION

 CREATE OR REPLACE FUNCTION encrypt_password(a text) RETURNS text AS $$
  DECLARE
  pass ALIAS FOR $1;
  encrypted text;
  BEGIN
   encrypted = digest(pass, 'sha512');
   RETURN encrypted; END; $$LANGUAGE 'plpgsql';

 CREATE OR REPLACE FUNCTION login(text, text) RETURNS boolean AS $$
DECLARE
my_mail ALIAS FOR $1; my_pass ALIAS FOR $2;
encrypted text;
row mms_user%ROWTYPE;
ret boolean;
BEGIN
 SELECT INTO row * from mms_user where mail = my_mail;
 IF row IS NOT NULL THEN
	 encrypted = digest(my_pass, 'sha512');
	 ret = encrypted = row.passwd;
 ELSE
	ret = false;
 END IF;
 RETURN ret; END; $$LANGUAGE 'plpgsql';

 CREATE OR REPLACE FUNCTION set_type() RETURNS int4 AS $$
DECLARE
type int4;
count int4;
BEGIN
 SELECT INTO count count(uid) from mms_user;
 IF count = 0 THEN
	type = 2;
ELSE
	type =0;
END IF;
 RETURN type; END; $$LANGUAGE 'plpgsql';


CREATE OR REPLACE FUNCTION check_update() RETURNS TRIGGER AS '
  DECLARE
  BEGIN
  IF NEW.usertype !=1 AND OLD.usertype != 2 THEN
  	NEW.usertype = OLD.usertype;
  END IF;
  RETURN NEW; END;'LANGUAGE 'plpgsql';
CREATE OR REPLACE FUNCTION check_values() RETURNS TRIGGER AS '
  DECLARE
  count int4;
  BEGIN
  SELECT INTO count count(uid) from mms_user;
  IF count = 0 AND NEW.usertype != 2 THEN
  	NEW.usertype = 2;
  ELSE IF count != 0 AND NEW.usertype != 0 THEN
  	NEW.usertype = 0;
  END IF;
  END IF;
  RETURN NEW; END;'LANGUAGE 'plpgsql';

-- TRIGGER CREATION
CREATE TRIGGER t_check_values BEFORE INSERT ON mms_user FOR EACH ROW EXECUTE PROCEDURE check_values();
CREATE TRIGGER t_check_update BEFORE UPDATE ON mms_user FOR EACH ROW EXECUTE PROCEDURE check_update();
