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

#define BLACKLIST_PREFIX "db/blacklist_"
#define WHITELIST_PREFIX "db/whitelist_"
#define FILENAME_SIZE 256

static char* DELIM = "::";

char filename[FILENAME_SIZE];

int set_filename(char *prefix, int country_code, int area_code) {
	snprintf(filename, sizeof(filename), "%s%d-%d", prefix, country_code, area_code);
	if ( NULL != filename ) return 0;
	else return -ENOENT;
}

int add_blacklist_entry(int country_code, int area_code, unsigned long long number, char *name, char *reason, int priority) {
	if ( 0 != set_filename(BLACKLIST_PREFIX, country_code, area_code) ||
		NULL != check_blacklist_entry(country_code, area_code, number, PRIO_ALL) ||
	  	priority < PRIO_ALL ) return -1;


	FILE *file;

	if ( NULL == (file = fopen(filename, "a+"))) return -EINVAL;
	fprintf(file, "%d%2s%lld%2s%d%2s%d%2s%s%2s%s\n", priority, DELIM, number, DELIM, country_code, DELIM, area_code, DELIM, name, DELIM, reason);

	fflush(file);
	fclose(file);

   	return 0;
}

int add_whitelist_entry(int country_code, int area_code, unsigned long long number, char *name, char *reason, int priority) {
	if ( 0 != set_filename(WHITELIST_PREFIX, country_code, area_code) ||
	  	NULL != check_whitelist_entry(country_code, area_code, number, PRIO_ALL) || 
	  	priority < PRIO_ALL ) return -1;

	FILE *file;

	if ( NULL == (file = fopen(filename, "a+"))) return -EINVAL;
	fprintf(file, "%d%2s%lld%2s%d%2s%d%2s%s%2s%s\n", priority, DELIM, number, DELIM, country_code, DELIM, area_code, DELIM, name, DELIM, reason);
	
	fflush(file);
	fclose(file);

   	return 0;
}

int rm_blacklist_entry (unsigned long long number) {
	return -ENOSYS;
}

int rm_whitelist_entry (unsigned long long number) {
	return -ENOSYS;
}

char *check_blacklist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	if ( 0 != set_filename(BLACKLIST_PREFIX, country_code, area_code) ) return NULL;

	FILE *file;

	if ( NULL == (file = fopen(filename, "a+"))) return NULL;

	char line[MAX_LINE_LENGTH];
	char *substr = NULL; 
	char strnumber[MAX_LINE_LENGTH];
	char *hit = NULL;
	int tmppriority;

	snprintf(strnumber, sizeof(strnumber), "%lld", number);

	while ( !feof(file) ) {
		if( 0 != fgets(line, sizeof(line), file) ) {
			substr = strtok(line, DELIM);
			tmppriority = atoi(substr);
			substr = strtok(NULL, DELIM);
			if ( PRIO_ALL == priority ) {
				if ( 0 == strcmp(substr, strnumber)  ) {
					hit = substr;
					fclose(file);
					return hit;
				}
			} else if ( tmppriority >= priority ) {
				if ( 0 == strcmp(substr, strnumber)  ) {
					hit = substr;
					fclose(file);
					return hit;
				}
			}
		} 
	}

	fclose(file);
	return hit;
}

char *check_whitelist_entry(int country_code, int area_code, unsigned long long number, int priority) {
	if ( 0 != set_filename(WHITELIST_PREFIX, country_code, area_code) ) return NULL;

	FILE *file;

	if ( NULL == (file = fopen(filename, "a+"))) return NULL;

	char line[MAX_LINE_LENGTH];
	char *substr = NULL; 
	char strnumber[MAX_LINE_LENGTH];
	char *hit = NULL;
	int tmppriority;

	snprintf(strnumber, sizeof(strnumber), "%lld", number);

	while ( !feof(file) ) {
		if( 0 != fgets(line, sizeof(line), file) ) {
			substr = strtok(line, DELIM);
			tmppriority = atoi(substr);
			substr = strtok(NULL, DELIM);
			if ( PRIO_ALL == priority ) {
				if ( 0 == strcmp(substr, strnumber)  ) {
					hit = substr;
					fclose(file);
					return hit;
				}
			} else if ( tmppriority >= priority ) {
				if ( 0 == strcmp(substr, strnumber)  ) {
					hit = substr;
					fclose(file);
					return hit;
				}
			}
		} 
	}

	fclose(file);
	return hit;
}
