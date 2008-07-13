/*
 * phonefirewall_search.c
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
#include "logfile.h"

#define ASCII_PERCENT_CHAR 37
#define MAX_ENTRY_ARRAY 1024

struct Entry *p_entry = &entry;
struct Entry entry_array[MAX_ENTRY_ARRAY];

struct Entry *find_entry_by_name(sqlite3_stmt *pp_stmt,
                                 char *name)
{
        int num_column;
        int count;
        char *col_name;
        char *col_value;

        num_column = sqlite3_column_count(pp_stmt);
        col_name = sqlite3_malloc(2 * num_column * sizeof(const char *) + 1);

        for ( count = 0; count < num_column; count++ ) {
                col_name = (char *)sqlite3_column_name(pp_stmt, count);
                col_value = (char *)sqlite3_column_text(pp_stmt, count);
                if ( 0 == strcmp(col_name, TB_PRIORITY) ) {
                        p_entry->priority = atoi(col_value);
                } else if ( 0 == strcmp(col_name, TB_COUNTRYCODE) ) {
                        p_entry->country_code = atoi(col_value);
                } else if ( 0 == strcmp(col_name, TB_AREACODE) ) {
                        p_entry->area_code = atoi(col_value);
                } else if ( 0 == strcmp(col_name, TB_NUMBER) ) {
                        p_entry->number = atoll(col_value);
                } else if ( 0 == strcmp(col_name, TB_NAME) ) {
                        p_entry->name = col_value;
                } else if ( 0 == strcmp(col_name, TB_REASON) ) {
                        p_entry->reason = col_value;
                }
        }
        return p_entry;
}

struct Entry *get_blacklist_entry_by_name(char *name)
{
        sqlite3 *db;
        char stmt[STMT_SIZE];
        sqlite3_stmt *pp_stmt = 0;
        const char **p_tail = 0;
        int rc;
        char logmsg[MAX_LINE_LENGTH];

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(logmsg, "Can't open database: %s", sqlite3_errmsg(db));
                write_logentry(logmsg, "phonefirewall", ERR_FLAG);
                sqlite3_close(db);
                return NULL;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s, %5$s, %6$s FROM blacklist WHERE %5$s like \'%8$c%7$s%8$c\'", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, name, ASCII_PERCENT_CHAR);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                sprintf(logmsg, "SQL error: %s", sqlite3_errmsg(db));
                write_logentry(logmsg, "phonefirewall", ERR_FLAG);
                sqlite3_close(db);
                return NULL;
        }

        int count = 0;
        printf("\nafter the find_entry_by_name(...) in get_blacklist_entry_by_name(char *name):\n");
        while ( rc != SQLITE_DONE ) {
                rc = sqlite3_step(pp_stmt);
                if ( SQLITE_ROW == rc ) {
                        p_entry = find_entry_by_name(pp_stmt, name);
                        entry_array[count] = *p_entry;
                        printf("+%d %d %llu -> %s :: %s\n", entry_array[count].country_code, entry_array[count].area_code, entry_array[count].number, entry_array[count].name, entry_array[count].reason);
                        count++;
                }
        }

        printf("\nbefore the return statement in get_blacklist_entry_by_name(char *name):\n");
        count = 0;
        for ( count = 0; count < 4; count++ )
                printf("+%d %d %llu -> %s :: %s\n", entry_array[count].country_code, entry_array[count].area_code, entry_array[count].number, entry_array[count].name, entry_array[count].reason);

        return entry_array;
}

struct Entry *get_blacklist_entry_by_number(int country_code,
                                            int area_code,
                                            unsigned long long number)
{
        return NULL;
}

struct Entry *get_whitelist_entry_by_name(char *name)
{
        return NULL;
}

struct Entry *get_whitelist_entry_by_number(int country_code,
                                            int area_code,
                                            unsigned long long number)
{
        return NULL;
}

