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

int evaluate_stmt(sqlite3_stmt *pp_stmt, struct Entry *p_entry) {
	int num_column;
	int count;
	char *col_name;
	char *col_value;
	int tmp_priority;
	int tmp_country_code;
	int tmp_area_code;
	unsigned long long tmp_number;
	int found_flag = 0;


	num_column = sqlite3_column_count(pp_stmt);
	col_name = sqlite3_malloc(2 * num_column * sizeof(const char *) + 1);

	for(count = 0; count < num_column; count++){
		col_name = (char *)sqlite3_column_name(pp_stmt, count);
		col_value = (char *)sqlite3_column_text(pp_stmt, count);
		if ( 0 == strcmp(col_name, TB_PRIORITY) ) {
			tmp_priority = atoi(col_value);
		} else if ( 0 == strcmp(col_name, TB_COUNTRYCODE) ) {
			tmp_country_code = atoi(col_value);
		} else if ( 0 == strcmp(col_name, TB_AREACODE) ) {
			tmp_area_code = atoi(col_value);
		} else if ( 0 == strcmp(col_name, TB_NUMBER) ) {
			tmp_number = atoll(col_value);
		}

		if ( PRIO_ALL == tmp_priority ) {
			if ( tmp_country_code == (p_entry->country_code)
					&& tmp_area_code == (p_entry->area_code)
					&& tmp_number == (p_entry->number) ) {
				found_flag = 1;
			}
		} else if ( tmp_priority <= (p_entry->priority) ) {
			if ( tmp_country_code == (p_entry->country_code)
					&& tmp_area_code == (p_entry->area_code)
					&& tmp_number == (p_entry->number) ) {
				found_flag = 1;
			} 
		} 
	}
	
	return found_flag;
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
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "INSERT INTO blacklist (%s, %s, %s, %s, %s, %s) VALUES(%d, %d, %d, %lld, \"%s\", \"%s\")", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, priority, country_code, area_code, number, name, reason);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
		// TODO: Don't print stuff to stderr or stderr
	       	//fprintf(stderr, "SQL error: %s\n", errMsg);
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
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "INSERT INTO whitelist (%s, %s, %s, %s, %s, %s) VALUES(%d, %d, %d, %lld, \"%s\", \"%s\")", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, priority, country_code, area_code, number, name, reason);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
		// TODO: Don't print stuff to stderr or stderr
	       	//fprintf(stderr, "SQL error: %s\n", errMsg);
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
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "DELETE FROM blacklist WHERE %s = %d AND %s = %d AND %s = %lld", TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
		// TODO: Don't print stuff to stderr or stderr
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
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "DELETE FROM whitelist WHERE %s = %d AND %s = %d AND %s = %lld", TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

	rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
	if ( SQLITE_OK != rc ) {
		// TODO: Don't print stuff to stderr or stderr
	       	fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	sqlite3_close(db);	

	return 0;
}

int check_blacklist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	sqlite3 *db;
	char *errMsg = 0;
	char stmt[STMT_SIZE];   // The SQL statement as text string.
	sqlite3_stmt *pp_stmt;  // The prepared statement
	const char **p_tail;    // The unused part of stmt
	int rc;
	int found_flag = 0;

	struct Entry *p_entry = &entry;
       	p_entry->country_code = country_code;
	p_entry->area_code = area_code;
	p_entry->number = number;
	p_entry->priority = priority;

	rc = sqlite3_open(DB_FILE, &db);

	if ( rc ) {
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM blacklist ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER);

	rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

	if ( rc != SQLITE_OK ) {
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	while( rc != SQLITE_DONE) {
		rc = sqlite3_step(pp_stmt);
		if ( SQLITE_ROW == rc ) {
			found_flag = evaluate_stmt(pp_stmt, p_entry);
			if ( found_flag == 1) break;
		}
	}

	
	// Cleaning up
	sqlite3_finalize(pp_stmt);
	sqlite3_close(db);

	return found_flag;
}

int check_whitelist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	sqlite3 *db;
	char *errMsg = 0;
	char stmt[STMT_SIZE];       // The SQL statement as text string.
	sqlite3_stmt *pp_stmt = 0;  // The prepared statement
	const char **p_tail = 0;    // The unused part of stmt
	int rc;
	int found_flag = 0;

	struct Entry *p_entry = &entry;
       	p_entry->country_code = country_code;
	p_entry->area_code = area_code;
	p_entry->number = number;
	p_entry->priority = priority;

	rc = sqlite3_open(DB_FILE, &db);

	if ( rc ) {
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return -1;
	}

	sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM whitelist ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER);

	rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

	if ( rc != SQLITE_OK ) {
		// TODO: Don't print stuff to stderr or stderr
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_close(db);
		return -1;
	}

	while( rc != SQLITE_DONE) {
		rc = sqlite3_step(pp_stmt);
		if ( SQLITE_ROW == rc ) {
			found_flag = evaluate_stmt(pp_stmt, p_entry);
			if ( found_flag == 1) break;
		}
	}

	
	// Cleaning up
	sqlite3_finalize(pp_stmt);
	sqlite3_close(db);

	return found_flag;
}

