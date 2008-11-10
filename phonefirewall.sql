-- *******************************************************************
--  phonefirewall: Script for creating black- and whitelist.
--  Usage:
--        $ sqlite3 db/phonefirewall.db < phonefirewall.sql
--
-- *******************************************************************
-- *******************************************************************

CREATE TABLE IF NOT EXISTS whitelist ( priority INTEGER,
			 countrycode INTEGER,
			 areacode INTEGER,
			 number BIGINT,
			 name varchar(64),
			 reason varchar(256),
			 PRIMARY KEY(countrycode, areacode, number) );

CREATE TABLE IF NOT EXISTS blacklist ( priority INTEGER,
			 countrycode INTEGER,
			 areacode INTEGER,
			 number BIGINT,
			 name varchar(64),
			 reason varchar(256),
			 PRIMARY KEY(countrycode, areacode, number) );

