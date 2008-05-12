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

static char* DELIM = "::";

int add_blacklist_entry(int country_code, int area_code, long long int number, char *name, char *reason, int priority) {
	FILE *file;
	char *prefix = "db/blacklist_";
	int filename_size = sizeof(country_code) + sizeof(area_code) + sizeof(prefix) + 10;
	char filename[filename_size];

	snprintf(filename, filename_size, "%s%d-%d", prefix, country_code, area_code);

	if ( NULL == (file = fopen(filename, "a+"))) return -EINVAL;
	fprintf(file, "%d%s%lld%s%d%s%d%s%s%s%s\n", priority, DELIM, number, DELIM, country_code, DELIM, area_code, DELIM, name, DELIM, reason);
	fflush(file);
	fclose(file);

   	return 0;
}

int add_whitelist_entry(int country_code, int area_code, long long int number, char *name, char *reason, int priority) {
	FILE *file;
	char *prefix = "db/whitelist_";
	int filename_size = sizeof(country_code) + sizeof(area_code) + sizeof(prefix) + 10;
	char filename[filename_size];

	snprintf(filename, filename_size, "%s%d-%d", prefix, country_code, area_code);

	if ( NULL == (file = fopen(filename, "a+"))) return -EINVAL;
	fprintf(file, "%d%s%d%s%d%s%lld%s%s%s%s\n", priority, DELIM, country_code, DELIM, area_code, DELIM, number, DELIM, name, DELIM, reason);
	fflush(file);
	fclose(file);

   	return 0;
}

int rm_blacklist_entry (long long int number) {
	return -ENOSYS;
}

int rm_whitelist_entry (long long int number) {
	return -ENOSYS;
}

char *check_blacklist_entry(int country_code, int area_code, long long int number) {
	FILE *file;
	char *prefix = "db/blacklist_";
	int filename_size = sizeof(country_code) + sizeof(area_code) + sizeof(prefix) + 10;
	char filename[filename_size];

	snprintf(filename, filename_size, "%s%d-%d", prefix, country_code, area_code);

	if ( NULL == (file = fopen(filename, "a+"))) return NULL;

	char line[MAX_LINE_LENGTH];
	char *substr = NULL; 
	char strnumber[MAX_LINE_LENGTH];
	char *hit = NULL;
	snprintf(strnumber, sizeof(strnumber), "%lld", number);

	while ( !feof(file) ) {
		if( 0 != fgets(line, sizeof(line), file) ) {
			substr = strtok(line, DELIM);
			substr = strtok(NULL, DELIM);
			if ( 0 == strcmp(substr, strnumber)  ) {
				hit = substr;
				return hit;
			}
		} 
	}
	return hit;
}

char *check_whitelist_entry(int country_code, int area_code, long long int number) {
	return NULL;
}
