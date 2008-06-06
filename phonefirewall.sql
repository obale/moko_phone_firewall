-- *******************************************************************
--  examLog: Script for creating log table 
--  Usage:
--        $ sqlite3 db/phone-firewall.db < phonefirewall.sql
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

