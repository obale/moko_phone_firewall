/*
 * phonefirewall_administration.c
 * 
 * (C) 2008 by Networld Consulting, Ltd. 
 * Written by Alex Oberhauser <oberhauseralex@networld.to> 
 * All Rights Reserved 
 * 
 * This program is free software: you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation, version 2 of the License.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */  
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sqlite3.h>
#include "libphonefirewall.h" 

int add_blacklist_entry(int country_code, int area_code, unsigned long long number, char *name, char *reason, int priority) {
	if ( priority <= PRIO_ALL ) return -1;

	sqlite3 *db;
	char *errMsg = 0;
	char stmt[STMT_SIZE];
	int rc;

	rc = sqlite3_open(DB_FILE, &db);

	if ( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "insert into blacklist (priority, countrycode, areacode, number, name, reason) values(%d, %d, %d, %lld, \"%s\", \"%s\")", priority, country_code, area_code, number, name, reason);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
	       	fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	sqlite3_close(db);	

   	return 0;
}

int add_whitelist_entry(int country_code, int area_code, unsigned long long number, char *name, char *reason, int priority) {
	sqlite3 *db;
	char *errMsg = 0;
	char stmt[2048];
	int rc;

	rc = sqlite3_open(DB_FILE, &db);

	if ( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "insert into whitelist (priority, countrycode, areacode, number, name, reason) values(%d, %d, %d, %lld, \"%s\", \"%s\")", priority, country_code, area_code, number, name, reason);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
	       	fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	sqlite3_close(db);	

   	return 0;
}

int rm_blacklist_entry (unsigned long long number) {
	return -ENOSYS;
}

int rm_whitelist_entry (unsigned long long number) {
	return -ENOSYS;
}

char *check_blacklist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	return NULL;
}

char *check_whitelist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	return NULL;
}
