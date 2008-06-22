/*
 * libphonefirewall.h
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

/**
 * @mainfile
 * @file libphonefirewall.h
 * @author Alex Oberhauser
 * @brief API of the phone firewall.
 *
 * The header file of the Phone Firewall. Blocks or accepts incoming
 * phone calls, so it's possible to prevent disturbing phone calls.
 *  Provides a API which can used by other application to build nice programs.
 *
 */

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
} entry;

/**
 * Add a number to the blacklist. The number will be blocked after that.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param name The name of the person.
 * @param reason Why you have blocked this person.
 * @param priority Gives the entry a priority. 0 is standard. If the priority
 * is higher the value will be also blocked/accepted if a higher priority is
 * choosen. <br>
 * The value "PRIO_ALL" stands for all priorities.
 *
 * @return If all goes well 0 (zero) otherwise an errno code.
 */
int add_blacklist_entry(int country_code,
                        int area_code,
                        unsigned long long number,
                        char *name,
                        char *reason,
                        int priority);

/**
 * Removes a blocked number from the blacklist.
 *
 * @param number The number which will be deleted.
 *
 * @return If all goes right 0, otherwise an error code.
 */
int rm_blacklist_entry(int country_code,
                       int area_code,
                       unsigned long long number);

/**
 * Checks if a number is on the blacklist.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param priority Gives the entry a priority. 0 is standard. If the priority
 * is higher the value will be also blocked/accepted if a higher priority is
 * choosen.<br>
 * The value "PRIO_ALL" stands for all priorities.
 *
 * @return If the number was found 1, otherwise 0.
 */
int check_blacklist_entry(int country_code,
                          int area_code,
                          unsigned long long number,
                          int priority);

/**
 * Add a number to the whitelist. The number will be accepted after that.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param name The name of the person.
 * @param reason Why you have blocked this person.
 * @param priority Gives the entry a priority. 0 is standard. If the priority
 * is higher the value will be also blocked/accepted if a higher priority is
 * choosen.<br>
 * The value "PRIO_ALL" stands for all priorities.
 *
 * @return If all goes well 0 (zero) otherwise an errno code.
 */
int add_whitelist_entry(int country_code,
                        int area_code,
                        unsigned long long number,
                        char *name,
                        char *reason,
                        int priority);

/**
 * Search a entrie by name.
 *
 * @param name The name of the person which is blocked.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_blacklist_entry_by_name(char *name);

/**
 * Search a entrie by number (country code + area code + number).
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_blacklist_entry_by_number(int country_code,
                                            int area_code,
                                            unsigned long long number);

/**
 * Removes a accepted number from the whitelist.
 *
 * @param number The number which will be deleted.
 *
 * @return If all goes right 0, otherwise an error code.
 */
int rm_whitelist_entry(int country_code,
                       int area_code,
                       unsigned long long number);

/**
 * Checks if a number is on the whitelist.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param priority Gives the entry a priority. 0 is standard. If the priority
 * is higher the value will be also blocked/accepted if a higher priority is
 * choosen.<br>
 * The value "PRIO_ALL" stands for all priorities.
 *
 * @return If the number was found 1, otherwise 0.
 */
int check_whitelist_entry(int country_code,
                          int area_code,
                          unsigned long long number,
                          int priority);

/**
 * Search a entrie by name.
 *
 * @param name The name of the person which is accepted.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_whitelist_entry_by_name(char *name);

/**
 * Search a entrie by number (country code + area code + number).
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_whitelist_entry_by_number(int country_code,
                                            int area_code,
                                            unsigned long long number);

