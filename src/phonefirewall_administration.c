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

typedef struct Blacklist blacklist_t;
typedef struct Whitelist whitelist_t;

blacklist_t *add_to_blacklist(blacklist_t *node, long long int number, char *name, char *reason, int priority) {
	printf("%llu", number);
   	if( NULL == node ) {
      		node = (blacklist_t *)malloc(sizeof(blacklist_t));
      		if( NULL == node ) exit(-ENOMEM);
      		node->number = number;
      		node->name = name;
      		node->reason = reason;
		node->priority = priority;
		node->left = node->right = NULL;
   	} 
	else if( node->number >= number )
      		node->left = add_to_blacklist(node->left, number, name, reason, priority);
	else if( node->number < number )
      		node->right = add_to_blacklist(node->right, number, name, reason, priority);

   	return node;
}

whitelist_t *add_to_whitelist(whitelist_t *node, long long int number, char *name, char *reason, int priority) {
   	if( NULL == node ) {
      		node = (whitelist_t *)malloc(sizeof(whitelist_t));
      		if( NULL == node ) exit(-ENOMEM);
      		node->number = number;
      		node->name = name;
      		node->reason = reason;
		node->priority = priority;
		node->left = node->right = NULL;
   	} 
	else if( node->number >= number )
      		node->left = add_to_whitelist(node->left, number, name, reason, priority);
	else if( node->number < number )
      		node->right = add_to_whitelist(node->right, number, name, reason, priority);

   	return node;
}

int add_blacklist_entry(long long int number, char *name, char *reason, int priority) {

	blacklist_t *root = NULL;
	root = add_to_blacklist(root, number, name, reason, priority);

	return 0;
}

int rm_blacklist_entry (long long int number) {
	return -ENOSYS;
}

int add_whitelist_entry (long long int number, char *name, char *reason, int priority) {
	return -ENOSYS;
}

int rm_whitelist_entry (long long int number) {
	return -ENOSYS;
}

