-- *******************************************************************
--  examLog: Script for creating log table 
--  Usage:
--        $ sqlite3 db/phone-firewall.db < phonefirewall.sql
--
-- *******************************************************************
-- *******************************************************************

CREATE TABLE whitelist ( priority INTEGER, 
			 countrycode INTEGER, 
			 areacode INTEGER,
			 number INTEGER, 
			 name varchar(64), 
			 reason varchar(256),
			 PRIMARY KEY(countrycode, areacode, number) );

CREATE TABLE blacklist ( priority INTEGER, 
			 countrycode INTEGER, 
			 areacode INTEGER,
			 number INTEGER, 
			 name varchar(64), 
			 reason varchar(256),
			 PRIMARY KEY(countrycode, areacode, number) );
