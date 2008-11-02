/*
 * libphonefirewall.h
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

/**
 * @mainfile
 * @file libphonefirewall.h
 * @author Alex Oberhauser
 * @brief API of the phone firewall.
 *
 * The header file of the Phone Firewall. Blocks or accepts incoming
 * phone calls, so it's possible to prevent disturbing phone calls.
 * Provides a API which can used by other application to build nice programs.
 */

#include "pf_search.h"
#include "pf_administration.h"
#include "logfile.h"

#define PRIO_ALL -999
#define DB_FILE "db/phone-firewall.db"
#define STMT_SIZE 1024
#define MAX_LINE_LENGTH 512

#define TB_COUNTRYCODE  "countrycode"
#define TB_AREACODE  "areacode"
#define TB_NUMBER "number"
#define TB_NAME  "name"
#define TB_REASON "reason"
#define TB_PRIORITY "priority"

#define BLACKLIST_FLAG 0x01
#define WHITELIST_FLAG 0x02

#define COMPONENT_NAME "phone firewall"

/**
 * The struct which includes all information about entries (black- and
 * whitelist).
 *
 * @struct entry
 * @brief Includes all informations for an entry.
 */
struct Entry {
    int country_code;
    int area_code;
    unsigned long long number;
    char *name;
    char *reason;
    int priority;
    struct Entry *next;
};

