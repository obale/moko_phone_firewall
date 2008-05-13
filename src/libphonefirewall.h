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
 * @file libphonefirewall.h
 * @author Alex Oberhauser
 * @brief API of the phone firewall.
 * 
 * The header file of the Phone Firewall. Blocks or accepts incoming
 * phone calls, so it's possible to prevent disturbing phone calls.
 *  Provides a API which can used by other application to build nice programs.
 *
 * Implemented for the OpenMoko framework.
 */

#define MAX_LINE_LENGTH 512

/**
 * Add a number to the blacklist. The number will be blocked after that.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person.
 * @param name The name of the person.
 * @param reason Why you have blocked this person.
 * @param priority Has no affect at the moment. Later one it will be possible
 * to give each number priority. So you have more control when a number will
 * be blocked/accepted.
 *
 * @return If all goes well 0 (zero) otherwise an errno code.
 */
int add_blacklist_entry(int country_code, int area_code, long long int number, char *name, char *reason, int priority); 

/**
 * Removes a blocked number from the blacklist.
 *
 * @param number The number which will be deleted.
 *
 * @return If all goes right 0, otherwise an error code.
 */
int rm_blacklist_entry(long long int number);

/**
 * Checks if a number is on the blacklist.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person.
 *
 * @return If noting is found NULL, otherwise the number.
 */
char *check_blacklist_entry(int country_code, int area_code, long long int number);

/**
 * Add a number to the whitelist. The number will be accepted after that.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person.
 * @param name The name of the person.
 * @param reason Why you have blocked this person.
 * @param priority Has no affect at the moment. Later one it will be possible
 * to give each number priority. So you have more control when a number will
 * be blocked/accepted.
 *
 * @return If all goes well 0 (zero) otherwise an errno code.
 */
int add_whitelist_entry(int country_code, int area_code, long long int number, char *name, char *reason, int priority); 

/**
 * Removes a accepted number from the whitelist.
 *
 * @param number The number which will be deleted.
 *
 * @return If all goes right 0, otherwise an error code.
 */
int rm_whitelist_entry(long long int number);

/**
 * Checks if a number is on the whitelist.
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person.
 *
 * @return If noting is found NULL, otherwise the number.
 */
char *check_whitelist_entry(int country_code, int area_code, long long int number);
