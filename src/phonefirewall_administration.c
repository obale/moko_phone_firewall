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

/**
  * 0 if nothing was found.
  * 1 if and entry was found.
  */
int found_flag;

int check_callback(void *p_data, int argc, char **argv, char **col_name) {
	int count;
	int tmp_priority;
	int tmp_country_code;
	int tmp_area_code;
	unsigned long long tmp_number;
	struct Entry *p_entry = p_data;
	found_flag = 0;

	for ( count = 0; count < argc; count++ ) {

		if ( 0 == strcmp(col_name[count], TB_PRIORITY) ) {
			tmp_priority = atoi(argv[count]);
		} else if ( 0 == strcmp(col_name[count], TB_COUNTRYCODE) ) {
			tmp_country_code = atoi(argv[count]);
		} else if ( 0 == strcmp(col_name[count], TB_AREACODE) ) {
			tmp_area_code = atoi(argv[count]);
		} else if ( 0 == strcmp(col_name[count], TB_NUMBER) ) {
			tmp_number = atoll(argv[count]);
		}

		if ( PRIO_ALL == tmp_priority ) {
			if ( tmp_country_code == (p_entry->country_code)
					&& tmp_area_code == (p_entry->area_code)
					&& tmp_number == (p_entry->number) ) {
				found_flag = 1;
				printf("number: %llu - %llu\n", tmp_number, p_entry->number);
				return 0;
			}
		} else if ( tmp_priority >= (p_entry->priority) ) {
			if ( tmp_country_code == (p_entry->country_code)
					&& tmp_area_code == (p_entry->area_code)
					&& tmp_number == (p_entry->number) ) {
				found_flag = 1;
				printf("number: %llu - %llu -> priority: %d\n", tmp_number, p_entry->number, p_entry->priority);
				return 0;
			} 
		} 
	}
	return 0;
}

int add_blacklist_entry(int country_code, int area_code, unsigned long long number, char *name, char *reason, int priority) {
	if ( priority < PRIO_ALL 
			|| 0 == country_code
			|| 0 == area_code 
			|| 0 == number ) return -1;

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

	sprintf(stmt, "INSERT INTO blacklist (%s, %s, %s, %s, %s, %s) VALUES(%d, %d, %d, %lld, \"%s\", \"%s\")", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, priority, country_code, area_code, number, name, reason);

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
	if ( priority < PRIO_ALL 
			|| 0 == country_code
			|| 0 == area_code 
			|| 0 == number ) return -1;

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

	sprintf(stmt, "INSERT INTO whitelist (%s, %s, %s, %s, %s, %s) VALUES(%d, %d, %d, %lld, \"%s\", \"%s\")", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, priority, country_code, area_code, number, name, reason);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
	       	fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	sqlite3_close(db);	

   	return 0;
}

int rm_blacklist_entry (int country_code, int area_code, unsigned long long number) {
	if ( 0 == country_code
			|| 0 == area_code
			|| 0 == number ) return -1;

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

	sprintf(stmt, "DELETE FROM blacklist WHERE %s = %d AND %s = %d AND %s = %lld", TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
	       	fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	sqlite3_close(db);	
	return 0;
}

int rm_whitelist_entry (int country_code, int area_code, unsigned long long number) {
	if ( 0 == country_code
			|| 0 == area_code
			|| 0 == number ) return -1;

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

	sprintf(stmt, "DELETE FROM whitelist WHERE %s = %d AND %s = %d AND %s = %lld", TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
	       	fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	sqlite3_close(db);	

	return 0;
}

/**
 * XXX: Check function doesn't work. Seams to override the found-flag.
 */
int check_blacklist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	sqlite3 *db;
	char *errMsg = 0;
	char stmt[STMT_SIZE];
	int rc;

	struct Entry *p_entry = &entry;
       	p_entry->country_code = country_code;
	p_entry->area_code = area_code;
	p_entry->number = number;
	p_entry->priority = priority;

	rc = sqlite3_open(DB_FILE, &db);

	if ( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM blacklist ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER);

	rc = sqlite3_exec(db, stmt, check_callback, p_entry, &errMsg );

	if ( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}
	
	sqlite3_close(db);

	return 0;
}

/**
 * XXX: Check function doesn't work. Seams to override the found-flag.
 */
int check_whitelist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	sqlite3 *db;
	char *errMsg = 0;
	char stmt[STMT_SIZE];
	int rc;

	struct Entry *p_entry = &entry;
       	p_entry->country_code = country_code;
	p_entry->area_code = area_code;
	p_entry->number = number;
	p_entry->priority = priority;

	rc = sqlite3_open(DB_FILE, &db);

	if ( rc ) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM whitelist ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER);

	rc = sqlite3_exec(db, stmt, check_callback, p_entry, &errMsg );

	if ( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}
	
	sqlite3_close(db);

	return found_flag;
}

