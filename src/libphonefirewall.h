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

/**
 * The maximum length of a telephone number.
 */
#define TELNR_MAXLEN	32
 
/**
 * @struct Blacklist 
 * @brief Contains the blocked numbers.
 */
struct Blacklist {
	char *name;
	long long int number;
	int priority;
	char *reason;
	struct Blacklist *left;
	struct Blacklist *right;
};

/**
 * @struct Whitelist
 * @brief Contains the accepted numbers.
 */
struct Whitelist {
	char *name;
	long long int number;
	int priority;
	char *reason;
	struct Whitelist *left;
	struct Whitelist *right;
};

/**
 * Add a number to the blacklist. The number will be blocked after that.
 *
 * @param number The telephone number of the person.
 * @param name The name of the person.
 * @param reason Why you have blocked this person.
 * @param priority Has no affect at the moment. Later one it will be possible
 * to give each number priority. So you have more control when a number will
 * be blocked/accepted.
 *
 * @return If all goes well 0 (zero) otherwise an errno code.
 */
int add_blacklist_entry(long long int number, char *name, char *reason, int priority);

/**
 * Removes a blocked number from the blacklist.
 */
int rm_blacklist_entry(long long int number);

/**
 * Add a number to the whitelist. The number will be accepted after that.
 *
 * @param number The telephone number of the person.
 * @param name The name of the person.
 * @param reason Why you have blocked this person.
 * @param priority Has no affect at the moment. Later one it will be possible
 * to give each number priority. So you have more control when a number will
 * be blocked/accepted.
 *
 * @return If all goes well 0 (zero) otherwise an errno code.
 */
int add_whitelist_entry(long long int number, char *name, char *reason, int priority);

/**
 * Removes a accepted number from the whitelist.
 */
int rm_whitelist_entry(long long int number);
