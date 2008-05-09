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
#include "libphonefirewall.h" 

int add_blacklist_entry (char *number, char *name, char *reason, int priority) {

	if ( TELNR_MAXLEN < strlen(number) ) return -EOVERFLOW;

	Blacklist *blacklist[MAX_ENTRIES+1];
	blacklist[0] = (Blacklist *) malloc(sizeof(Blacklist));
	blacklist[1] = (Blacklist *) malloc(sizeof(Blacklist));

	blacklist[0]->number = number;
	blacklist[0]->name = name;
	blacklist[0]->reason = reason;
	free(blacklist[0]);
	
	blacklist[1]->number = "3493424242";
	blacklist[1]->name = "testing";
	blacklist[1]->reason = "don't know";
	free(blacklist[1]);

	return 0;
}

int rm_blacklist_entry (char *number) {
	return 0;
}

int add_whitelist_entry (char *number, char *name, char *reason, int priority) {
	return 0;
}

int rm_whitelist_entry (char *number) {
	return 0;
}
