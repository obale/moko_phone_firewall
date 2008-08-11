/*
 * main_test.c
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
 * along with MokSec.  If not, see <http://www.gnu.org/licenses/>
 */
#include <stdio.h>
#include "../src/libphonefirewall.h"

void test_get_entry_by_name(void);

/*
 * XXX: Only a quick hack to find the memory corruption error.
 *      Written manually to exclude the CUnit components.
 */
int main(void) {
        printf("Starting the manually written test...\n\n");
        test_get_entry_by_name();
}

void test_get_entry_by_name(void) {
        struct Entry *tmp_entry = NULL;
	tmp_entry = get_entry_by_name("Test", WHITELIST_FLAG);
        int count = 0;
        printf("\n");
        while ( tmp_entry != NULL ) {
                printf("[Manual Test] result #%d: +%d %d %llu - %s - %s\n", count,
                                                                      tmp_entry->country_code,
                                                                      tmp_entry->area_code,
                                                                      tmp_entry->number,
                                                                      tmp_entry->name,
                                                                      tmp_entry->reason);
                tmp_entry = tmp_entry->next;
                count++;
        }
        printf("\n");
}
