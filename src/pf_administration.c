/*
 * pf_administration.c
 *
 * (C) 2008 by MokSec Project
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
 * along with MokSec.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <sqlite3.h>
#include "libphonefirewall.h"

int evaluate_stmt(sqlite3_stmt *pp_stmt,
                  struct Entry *p_entry)
{
        int num_column;
        int count;
        char *col_name;
        char *col_value;
        int tmp_priority = 0;
        int tmp_country_code = 0;
        int tmp_area_code = 0;
        unsigned long long tmp_number = 0;

        num_column = sqlite3_column_count(pp_stmt);
        col_name = sqlite3_malloc(2 * num_column * sizeof(const char *) + 1);

        for ( count = 0; count < num_column; count++ ) {
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
        }

        if ( PRIO_ALL == tmp_priority ) {
                if ( tmp_country_code == (p_entry->country_code)
                                && tmp_area_code == (p_entry->area_code)
                                && tmp_number == (p_entry->number) ) {
                        return 1;
                }
        } else if ( tmp_priority <= (p_entry->priority) ) {
                if ( tmp_country_code == (p_entry->country_code)
                                && tmp_area_code == (p_entry->area_code)
                                && tmp_number == (p_entry->number) ) {
                        return 1;
                }
        }

        return 0;
}

int evaluate_stmt_string(sqlite3_stmt *pp_stmt,
                         char *number,
                         int priority)
{
        int num_column;
        int count;
        char *col_name;
        char *col_value;
        int tmp_priority = 0;
        char tmp_number[32] = "";

        num_column = sqlite3_column_count(pp_stmt);
        col_name = sqlite3_malloc(2 * num_column * sizeof(const char *) + 1);

        for ( count = 0; count < num_column; count++ ) {
                 col_name = (char *)sqlite3_column_name(pp_stmt, count);
                 col_value = (char *)sqlite3_column_text(pp_stmt, count);
                 if ( 0 == strcmp(col_name, TB_PRIORITY) ) {
                         tmp_priority = atoi(col_value);
                 } else if ( 0 == strcmp(col_name, TB_COUNTRYCODE) ) {
                         strcat(tmp_number, col_value);
                 } else if ( 0 == strcmp(col_name, TB_AREACODE) ) {
                         strcat(tmp_number, col_value);
                 } else if ( 0 == strcmp(col_name, TB_NUMBER) ) {
                         strcat(tmp_number, col_value);
                 }
        }

        if ( PRIO_ALL == tmp_priority ) {
                if ( 0 == strcmp(tmp_number, number) ) return 1;
        } else if ( tmp_priority <= priority ) {
                if ( 0 == strcmp(tmp_number, number) ) return 1;
        }

        return 0;
}

int add_entry(int country_code,
              int area_code,
              unsigned long long number,
              char *name,
              char *reason,
              int priority,
              int listflag)
{
        if ( priority < PRIO_ALL
                        || 0 == country_code
                        || 0 == area_code
                        || 0 == number ) {
                syslog(LOG_ERR, "[%s] Please fill out the obligation fields.", COMPONENT_NAME);
                return -1;
        }

        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char *errMsg = 0;
        char stmt[STMT_SIZE];
        int rc;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "INSERT INTO %s (%s, %s, %s, %s, %s, %s) VALUES(%d, %d, %d, %lld, \"%s\", \"%s\")", listname, TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, priority, country_code, area_code, number, name, reason);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        sqlite3_close(db);

        return 0;
}

int rm_entry (int country_code,
              int area_code,
              unsigned long long number,
              int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char *errMsg = 0;
        char stmt[STMT_SIZE];
        int rc;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Cant't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "DELETE FROM %s WHERE %s = %d AND %s = %d AND %s = %lld", listname, TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        sqlite3_close(db);
        return 0;
}

int check_entry(int country_code,
                int area_code,
                unsigned long long number,
                int priority,
                int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        sqlite3_stmt *pp_stmt = 0;  // The prepared statement
        const char **p_tail = 0;    // The unused part of stmt
        int rc;
        int found_flag = 0;

        struct Entry *p_entry = (struct Entry *) malloc(sizeof(struct Entry));
        p_entry->country_code = country_code;
        p_entry->area_code = area_code;
        p_entry->number = number;
        p_entry->priority = priority;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM %5$s ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, listname);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        while ( rc != SQLITE_DONE ) {
                rc = sqlite3_step(pp_stmt);
                if ( SQLITE_ROW == rc ) {
                        found_flag = evaluate_stmt(pp_stmt, p_entry);
                        if ( found_flag == 1) {
                                switch (listflag) {
                                        case WHITELIST_FLAG:
                                                syslog(LOG_DEBUG, "[%s] Number \"+%d %d %llu\" accepted successfully.", COMPONENT_NAME, country_code, area_code, number);
                                                break;
                                        case BLACKLIST_FLAG:
                                                syslog(LOG_DEBUG, "[%s] Number \"+%d %d %llu\" blocked successfully.", COMPONENT_NAME, country_code, area_code, number);
                                                break;
                                        default:
                                                syslog(LOG_CRIT, "[%s] Stuff is no blacklist nor a whitelist.", COMPONENT_NAME);
                                }
                                break;
                        }
                }
        }


        // Cleaning up
        sqlite3_finalize(pp_stmt);
        sqlite3_close(db);
        free(p_entry);

        return found_flag;
}

int check_entry_string(char *number,
                       int priority,
                       int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        sqlite3_stmt *pp_stmt = 0;  // The prepared statement
        const char **p_tail = 0;    // The unused part of stmt
        int rc;
        int found_flag = 0;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM %5$s ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, listname);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return -1;
        }

        while ( rc != SQLITE_DONE ) {
                rc = sqlite3_step(pp_stmt);
                if ( SQLITE_ROW == rc ) {
                        found_flag = evaluate_stmt_string(pp_stmt, number, priority);
                        if ( found_flag == 1) {
                                switch (listflag) {
                                        case WHITELIST_FLAG:
                                                syslog(LOG_DEBUG, "[%s] Number \"%s\" accepted successfully.", COMPONENT_NAME, number);
                                                break;
                                        case BLACKLIST_FLAG:
                                                syslog(LOG_DEBUG, "[%s] Number \"%s\" blocked successfully.", COMPONENT_NAME, number);
                                                break;
                                        default:
                                                syslog(LOG_CRIT, "[%s] Stuff is no blacklist nor a whitelist.", COMPONENT_NAME);
                                }
                                break;
                        }
                }
        }


        // Cleaning up
        sqlite3_finalize(pp_stmt);
        sqlite3_close(db);

        return found_flag;
}

int change_name(int country_code,
                int area_code,
                unsigned long long number,
                char *new_name,
                int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        char *errMsg = 0;
        int rc;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sprintf(stmt, "UPDATE %s SET %s = \"%s\" WHERE %s = %d AND %s = %d AND %s = %lld", listname, TB_NAME, new_name, TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sqlite3_close(db);

        return 1;
}

int change_number(int country_code,
                  int area_code,
                  unsigned long long number,
                  int new_country_code,
                  int new_area_code,
                  unsigned long long new_number,
                  int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        char *errMsg = 0;
        int rc;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sprintf(stmt, "UPDATE %s SET %s = %d AND %s = %d AND %s = %lld WHERE %s = %d AND %s = %d AND %s = %lld", listname, TB_COUNTRYCODE, new_country_code, TB_AREACODE, new_area_code, TB_NUMBER, new_number, TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sqlite3_close(db);

        return 1;
}

int change_reason(int country_code,
                  int area_code,
                  unsigned long long number,
                  char *new_reason,
                  int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        char *errMsg = 0;
        int rc;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sprintf(stmt, "UPDATE %s SET %s = \"%s\" WHERE %s = %d AND %s = %d AND %s = %lld", listname, TB_REASON, new_reason, TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sqlite3_close(db);

        return 1;
}

int change_priority(int country_code,
                    int area_code,
                    unsigned long long number,
                    int new_priority,
                    int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        char *errMsg = 0;
        int rc;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                syslog(LOG_ERR, "[%s] Can't open database: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sprintf(stmt, "UPDATE %s SET %s = %d WHERE %s = %d AND %s = %d AND %s = %lld", listname, TB_PRIORITY, new_priority, TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                syslog(LOG_ERR, "[%s] SQL error: %s", COMPONENT_NAME, sqlite3_errmsg(db));
                sqlite3_close(db);
                return 0;
        }

        sqlite3_close(db);

        return 1;
}
