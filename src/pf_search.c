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

#define ASCII_PERCENT_CHAR 37

struct Entry *insert_into_list(struct Entry *p_root,
                               struct Entry *p_entry)
{
        struct Entry *tmp_entry;
        if ( NULL == p_root ) {
                if ( NULL == (p_root = (struct Entry *) malloc(sizeof(struct Entry)) ) )
                        return NULL;
                p_root->country_code = p_entry->country_code;
                p_root->area_code = p_entry->area_code;
                p_root->number = p_entry->number;
                p_root->name = (char *)calloc(0, strlen(p_entry->name));
                strncpy(p_root->name, p_entry->name, strlen(p_entry->name));
                p_root->reason = p_entry->reason;
                p_root->next = NULL;
                #if DEBUG
                printf("\n\n[DEBUG]: +%d %d %llu - %s - %s\n", p_root->country_code,
                                                               p_root->area_code,
                                                               p_root->number,
                                                               p_root->name,
                                                               p_root->reason);
                #endif
        } else {
                tmp_entry = p_root;
                while ( tmp_entry->next != NULL )
                        tmp_entry = tmp_entry->next;
                if ( NULL == (tmp_entry->next = (struct Entry *) malloc(sizeof(struct Entry)) ) )
                        return NULL;
                tmp_entry = tmp_entry->next;
                tmp_entry->country_code = p_entry->country_code;
                tmp_entry->area_code = p_entry->area_code;
                tmp_entry->number = p_entry->number;
                tmp_entry->name = (char *)calloc(0, strlen(p_entry->name));
                strncpy(tmp_entry->name, p_entry->name, strlen(p_entry->name));
                tmp_entry->reason = p_entry->reason;
                tmp_entry->next = NULL;
                #if DEBUG
                printf("[DEBUG]: +%d %d %llu - %s - %s\n", tmp_entry->country_code,
                                                           tmp_entry->area_code,
                                                           tmp_entry->number,
                                                           tmp_entry->name,
                                                           tmp_entry->reason);
                #endif
        }

        return p_root;
}

struct Entry *find_entry(sqlite3_stmt *pp_stmt)
{
        int num_column;
        int count;
        char *col_name;
        char *col_value;
        struct Entry *p_entry = (struct Entry *) malloc(sizeof(struct Entry));

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
                        p_entry->name = calloc(0, strlen(col_value));
                        strncpy(p_entry->name, col_value, strlen(col_value));
                } else if ( 0 == strcmp(col_name, TB_REASON) ) {
                        p_entry->reason = calloc(0, strlen(col_value));
                        /* XXX: String copy seams not to work. */
                        //strncpy(p_entry->reason, col_value, strlen(col_value));
                        /* TODO: Implement this part or fix the part above. */
                        p_entry->reason = NULL;
                }
        }
        return p_entry;
}

struct Entry *get_entry_by_name(char *name,
                                int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];
        sqlite3_stmt *pp_stmt = 0;
        const char **p_tail = 0;
        int rc;
        char logmsg[MAX_LINE_LENGTH];
        struct Entry *p_root = NULL;
        struct Entry *p_entry = NULL;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(logmsg, "Can't open database: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
                sqlite3_close(db);
                return NULL;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s, %5$s, %6$s FROM %8$s WHERE %5$s like \'%7$c%9$s%7$c\'", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, ASCII_PERCENT_CHAR, listname, name);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                sprintf(logmsg, "SQL error: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
                sqlite3_close(db);
                return NULL;
        }

        while ( rc != SQLITE_DONE ) {
                rc = sqlite3_step(pp_stmt);
                if ( SQLITE_ROW == rc ) {
                        if ( NULL == (p_entry = find_entry(pp_stmt)) ) break;
                        if ( NULL == (p_root = insert_into_list(p_root, p_entry)) ) return NULL;
                }
        }
        free(p_entry);
        sqlite3_close(db);
        return p_root;
}

struct Entry *get_entry_by_number(int country_code,
                                  int area_code,
                                  unsigned long long number,
                                  int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];
        sqlite3_stmt *pp_stmt = 0;
        const char **p_tail = 0;
        int rc;
        char logmsg[MAX_LINE_LENGTH];
        struct Entry *p_root = NULL;
        struct Entry *p_entry = NULL;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(logmsg, "Can't open database: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
                sqlite3_close(db);
                return NULL;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s, %5$s, %6$s FROM %8$s WHERE %2$s like \'%7$c%9$d%7$c\' AND %3$s like \'%7$c%10$d%7$c\' AND %4$s like \'%7$c%11$llu%7$c\'", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, ASCII_PERCENT_CHAR, listname, country_code, area_code, number);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                sprintf(logmsg, "SQL error: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
                sqlite3_close(db);
                return NULL;
        }

        while ( rc != SQLITE_DONE ) {
                rc = sqlite3_step(pp_stmt);
                if ( SQLITE_ROW == rc ) {
                        if ( NULL == (p_entry = find_entry(pp_stmt)) ) break;
                        if ( NULL == (p_root = insert_into_list(p_root, p_entry)) ) return NULL;
                }
        }
        free(p_entry);
        sqlite3_close(db);
        return p_root;
}

struct Entry *get_entry_by_reason(char *reason,
                                  int listflag)
{
        char *listname = (WHITELIST_FLAG == listflag) ? "whitelist" : "blacklist";

        sqlite3 *db;
        char stmt[STMT_SIZE];
        sqlite3_stmt *pp_stmt = 0;
        const char **p_tail = 0;
        int rc;
        char logmsg[MAX_LINE_LENGTH];
        struct Entry *p_root = NULL;
        struct Entry *p_entry = NULL;

        rc = sqlite3_open(DB_FILE, &db);

        if ( rc ) {
                sprintf(logmsg, "Can't open database: %s", sqlite3_errmsg(db));
                ERR_LOG(logmsg);
                sqlite3_close(db);
                return NULL;
        }

        sprintf(stmt, "SELECT %1$s, %2$s, %3$s, %4$s, %5$s, %6$s FROM %8$s WHERE %6$s like \'%7$c%9$s%7$c\'", TB_PRIORITY, TB_COUNTRYCODE, TB_AREACODE, TB_NUMBER, TB_NAME, TB_REASON, ASCII_PERCENT_CHAR, listname, reason);

        rc = sqlite3_prepare_v2(db, stmt, sizeof(stmt), &pp_stmt, p_tail);

        if ( rc != SQLITE_OK ) {
                sprintf(logmsg, "SQL error: %s", sqlite3_errmsg(db));
                sqlite3_close(db);
                return NULL;
        }

        while ( rc != SQLITE_DONE ) {
                rc = sqlite3_step(pp_stmt);
                if ( SQLITE_ROW == rc ) {
                        if ( NULL == (p_entry = find_entry(pp_stmt)) ) break;
                        if ( NULL == (p_root = insert_into_list(p_root, p_entry)) ) return NULL;
                }
        }
        free(p_entry);
        sqlite3_close(db);
        return p_root;
}
