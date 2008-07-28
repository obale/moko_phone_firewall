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
 * along with MokSec.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
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
                        || 0 == number ) return -1;

        char *listname;
        switch (listflag) {
                case WHITELIST_FLAG:
                        listname = "whitelist";
                        break;
                case BLACKLIST_FLAG:
                        listname = "blacklist";
                        break;
                default: return -1;
        }

        sqlite3 *db;
        char *errMsg = 0;
        char stmt[STMT_SIZE];
        int rc;
        char error[MAX_LINE_LENGTH];

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(error, "Can't open database: %s", sqlite3_errmsg(db));
                ERR_LOG(error);
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "INSERT INTO %s (%s, %s, %s, %s, %s, %s) VALUES(%d, %d, %d, %lld, \"%s\", \"%s\")",listname, TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, priority, country_code, area_code, number, name, reason);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                sprintf(error, "SQL error: %s", sqlite3_errmsg(db));
                ERR_LOG(error);
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
        if ( 0 == country_code
                        || 0 == area_code
                        || 0 == number ) return -1;

        char *listname;
        switch (listflag) {
                case WHITELIST_FLAG:
                        listname = "whitelist";
                        break;
                case BLACKLIST_FLAG:
                        listname = "blacklist";
                        break;
                default: return -1;
        }

        sqlite3 *db;
        char *errMsg = 0;
        char stmt[STMT_SIZE];
        int rc;
        char error[MAX_LINE_LENGTH];

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(error, "Can't open database: %s", sqlite3_errmsg(db));
                ERR_LOG(error);
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "DELETE FROM %s WHERE %s = %d AND %s = %d AND %s = %lld", listname, TB_COUNTRYCODE, country_code, TB_AREACODE, area_code, TB_NUMBER, number);

        rc = sqlite3_exec(db, stmt, NULL, 0, &errMsg);
        if ( SQLITE_OK != rc ) {
                sprintf(error, "SQL error: %s", sqlite3_errmsg(db));
                ERR_LOG(error);
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
        char *listname;
        switch (listflag) {
                case WHITELIST_FLAG:
                        listname = "whitelist";
                        break;
                case BLACKLIST_FLAG:
                        listname = "blacklist";
                        break;
                default: return -1;
        }

        sqlite3 *db;
        char stmt[STMT_SIZE];       // The SQL statement as text string.
        sqlite3_stmt *pp_stmt = 0;  // The prepared statement
        const char **p_tail = 0;    // The unused part of stmt
        int rc;
        char logmsg[MAX_LINE_LENGTH];
        int found_flag = 0;

        struct Entry *p_entry = (struct Entry *) malloc(sizeof(struct Entry));
        p_entry->country_code = country_code;
        p_entry->area_code = area_code;
        p_entry->number = number;
        p_entry->priority = priority;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(logmsg, "Can't open database: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
                sqlite3_close(db);
                return -1;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s FROM %5$s ORDER BY %1$s, %2$s, %3$s, %4$s", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, listname);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                sprintf(logmsg, "SQL error: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
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
                                                sprintf(logmsg, "Number \"+%d %d %llu\" accepted successfully.", country_code, area_code, number);
                                                break;
                                        case BLACKLIST_FLAG:
                                                sprintf(logmsg, "Number \"+%d %d %llu\" blocked successfully.", country_code, area_code, number);
                                                break;
                                        default:
                                                sprintf(logmsg, "Something goes wrong...");
                                }
                                INFO_LOG(logmsg);
                                break;
                        }
                }
        }


        // Cleaning up
        sqlite3_finalize(pp_stmt);
        sqlite3_close(db);

        return found_flag;
}

