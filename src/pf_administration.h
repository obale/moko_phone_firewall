/*
 * pf_administration.h
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
 * along with MokSec.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef _PF_H
# error "Never include the header file pf_administration.h directly. Include instead libphonefirewall.h"
#endif

/**
 * Add a number to the blacklist/whitelist. The number will be blocked after that.
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
 * choosen. 
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * The value "PRIO_ALL" stands for all priorities.
 *
 * @return If all goes well 0 (zero) otherwise -1 and the errno variable will
 * be set..
 */
int add_entry(int country_code,
              int area_code,
              unsigned long long number,
              char *name,
              char *reason,
              int priority,
              int listflag);

/**
 * Removes a number from the blacklist/whitelist.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The number which will be deleted.
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * @return If all goes right 0, otherwise an error code.
 */
int rm_entry(int country_code,
             int area_code,
             unsigned long long number,
             int listlfag);

/**
 * Checks if a number is on the blacklist/whitelist.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param priority Gives the entry a priority. 0 is standard. If the priority
 * is higher the value will be also blocked/accepted if a higher priority is
 * choosen.
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * The value "PRIO_ALL" stands for all priorities.
 *
 * @return If the number was found 1, otherwise 0.
 */
int check_entry(int country_code,
                int area_code,
                unsigned long long number,
                int priority,
                int listflag);

/**
 * Checks if a number is on the blacklist/whitelist.
 * 
 * @param number The whole number with country code, area code and phone
 * number.
 * @param priority Gives the entry a priority. 0 is standard. If the priority
 * is higher the value will be also blocked/accepted if a higher priority is
 * choosen.
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * @return If the number was found 1, otherwise 0.
 */
int check_entry_string(char *number,
                       int priority,
                       int listflag);

/**
 * Changes the name of the entry. For the unique identification you need to
 * enter the country code, area code and the number because this tripple
 * identifies the entry.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param new_name The new name
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * @return If the number was changed 1, otherwise 0.
 */
int change_name(int country_code,
                int area_code,
                unsigned long long number,
                char *new_name,
                int listflag);

/**
 * Changes the number of the entry. You need to enter the country code, 
 * area code and the number because this tripple identifies the entry.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param new_country_code The new country code.
 * @param new_area_code The new area code.
 * @param new_number The new number.
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * @return If the number was changed 1, otherwise 0.
 */
int change_number(int country_code,
                  int area_code,
                  unsigned long long number,
                  int new_country_code,
                  int new_area_code,
                  unsigned long long new_number,
                  int listflag);

/**
 * Changes the reason of the entry. For the unique identification you need to
 * enter the country code, area code and the number because this tripple
 * identifies the entry.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param new_reason The new reason.
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * @return If the number was changed 1, otherwise 0.
 */
int change_reason(int country_code,
                  int area_code,
                  unsigned long long number,
                  char *new_reason,
                  int listflag);

/**
 * Changes the priority of the entry. For the unique identification you need to
 * enter the country code, area code and the number because this tripple
 * identifies the entry.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 * @param new_priority The new priority.
 * @param listflag A flag, which indicates if you would use the blacklist
 * (BLACKLIST_FLAG) or the whitelist (WHITELIST_FLAG).<br>
 *
 * @return If the number was changed 1, otherwise 0.
 */
int change_priority(int country_code,
                    int area_code,
                    unsigned long long number,
                    int new_priority,
                    int listflag);
