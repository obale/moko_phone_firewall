/*
 * testphonefirewall.c
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
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../src/libphonefirewall.h"


void test_add_blacklist_entry(void)
{
	/* [OK]    -> Testuser 1:  Number in Italy. */
	CU_ASSERT(add_entry(39, 328, 123456789, "Testuser 1", "I don't like him.", 0, BLACKLIST_FLAG) == 0);

        /* [OK]    -> Temporary User */
	CU_ASSERT(add_entry(39, 328, 100056789, "Testuser X", "I don't like him.", 0, BLACKLIST_FLAG) == 0);

	/* [FAILS] -> Testuser -1: The same number as "Testuser 1". */
	CU_ASSERT(add_entry(39, 328, 123456789, "Testuser -1", "I don't like him.", 0, BLACKLIST_FLAG) == -1);

	/* [FAILS] -> Testuser -1: The country code is NULL. */
	CU_ASSERT(add_entry(0, 328, 123456782, "Testuser -1", "I don't like him.", 0, BLACKLIST_FLAG) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0, BLACKLIST_FLAG) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 328, 0, "Testuser -1", "I don't like him.", 0, BLACKLIST_FLAG) == -1);

	/* [OK]    -> Testuser 2:  The same area code and number as "Testuser
	 * 1" but a different country code. */
	CU_ASSERT(add_entry(49, 328, 123456789, "Testuser 2", "I don't like him.", 0, BLACKLIST_FLAG) == 0);

	/* [OK]    -> Testuser 3:  The same country code and number as "Testuser
	 * 1" but a different area code. */
	CU_ASSERT(add_entry(49, 329, 123456789, "Testuser 3", "I don't like him.", 0, BLACKLIST_FLAG) == 0);

	/* [OK]    -> NULL      :  Username is empty. */
	CU_ASSERT(add_entry(49, 329, 123456780, NULL, "I don't like him.", 0, BLACKLIST_FLAG) == 0);

	/* [OK]    -> Testuser 4:  Reason is empty. */
	CU_ASSERT(add_entry(49, 329, 123456781, "Testuser 4", NULL, 0, BLACKLIST_FLAG) == 0);

	/* [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 329, 222222222, "Testuser 5", NULL, 2, BLACKLIST_FLAG) == 0);

	/* [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 329, 999999999, "Testuser 6", NULL, PRIO_ALL, BLACKLIST_FLAG) == 0);
}

void test_rm_blacklist_entry(void)
{
#if 1
	CU_ASSERT(rm_entry(49, 329, 999999999, BLACKLIST_FLAG) == 0);
	CU_ASSERT(rm_entry(39, 328, 123456789, BLACKLIST_FLAG) == 0);
#endif
}

void test_add_whitelist_entry(void)
{
	/* [OK]    -> Testuser 1:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456789, "Testuser 1", "I don't like him.", 0, WHITELIST_FLAG) == 0);

	/* [FAILS] -> Testuser -1: The same number as "Testuser 1". */
	CU_ASSERT(add_entry(39, 128, 123456789, "Testuser -1", "I don't like him.", 0, WHITELIST_FLAG) == -1);

	/* [FAILS] -> Testuser -1: The country code is NULL. */
	CU_ASSERT(add_entry(0, 128, 123456782, "Testuser -1", "I don't like him.", 0, WHITELIST_FLAG) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0, WHITELIST_FLAG) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 128, 0, "Testuser -1", "I don't like him.", 0, WHITELIST_FLAG) == -1);

	/* [OK]    -> Testuser 2:  The same area code and number as "Testuser
	 * 1" but a different country code. */
	CU_ASSERT(add_entry(49, 128, 123456789, "Testuser 2", "I don't like him.", 0, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 3:  The same country code and number as "Testuser
	 * 1" but a different area code. */
	CU_ASSERT(add_entry(49, 129, 123456789, "Testuser 3", "I don't like him.", 0, WHITELIST_FLAG) == 0);

	/* [OK]    -> NULL      :  Username is empty. */
	CU_ASSERT(add_entry(49, 129, 123456780, NULL, "I don't like him.", 0, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 4:  Reason is empty. */
	CU_ASSERT(add_entry(49, 129, 123456781, "Testuser 4", NULL, 0, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 129, 222222222, "Testuser 5", NULL, 2, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 129, 999999999, "Testuser 6", NULL, PRIO_ALL, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 7:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456783, "Testuser 7", "I don't like him.", 2, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 8:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456784, "Testuser 8", "I don't like him.", 5, WHITELIST_FLAG) == 0);

	/* [OK]    -> Testuser 9:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456785, "Testuser 9", "I don't like him.", 10, WHITELIST_FLAG) == 0);
}

void test_rm_whitelist_entry(void)
{
#if 1
	CU_ASSERT(rm_entry(49, 129, 999999999, WHITELIST_FLAG) == 0);
	CU_ASSERT(rm_entry(39, 128, 123456789, WHITELIST_FLAG) == 0);
#endif
}


void test_check_blacklist_entry(void)
{
#if 1
	CU_ASSERT(check_entry(39, 328, 123456789, 0, BLACKLIST_FLAG) == 1);
	CU_ASSERT(check_entry(39, 328, 103456785, 0, BLACKLIST_FLAG) == 0);
	CU_ASSERT(check_entry(49, 329, 999999999, 0, BLACKLIST_FLAG) == 1);
	CU_ASSERT(check_entry(49, 329, 222222222, 0, BLACKLIST_FLAG) == 0);
	CU_ASSERT(check_entry(49, 329, 222222222, 2, BLACKLIST_FLAG) == 1);
	CU_ASSERT(check_entry(49, 329, 222222222, 3, BLACKLIST_FLAG) == 1);
#endif
}

void test_check_whitelist_entry(void)
{
#if 1
	CU_ASSERT(check_entry(39, 128, 123456789, 0, WHITELIST_FLAG) == 1);
	CU_ASSERT(check_entry(39, 128, 103456785, 0, WHITELIST_FLAG) == 0);
	CU_ASSERT(check_entry(49, 129, 999999999, 0, WHITELIST_FLAG) == 1);
	CU_ASSERT(check_entry(49, 129, 222222222, 0, WHITELIST_FLAG) == 0);
	CU_ASSERT(check_entry(49, 129, 222222222, 2, WHITELIST_FLAG) == 1);
	CU_ASSERT(check_entry(49, 129, 222222222, 3, WHITELIST_FLAG) == 1);
#endif
}

void test_get_blacklist_entry_by_name(void)
{
#if 1
        struct Entry *tmp_entry = NULL;
	tmp_entry = get_entry_by_name("user", BLACKLIST_FLAG);
        int count = 0;
        printf("\n");
        while ( tmp_entry != NULL ) {
                printf("[CUNIT] result #%d: +%d %d %llu - %s - %s\n", count,
                                                                      tmp_entry->country_code,
                                                                      tmp_entry->area_code,
                                                                      tmp_entry->number,
                                                                      tmp_entry->name,
                                                                      tmp_entry->reason);
                tmp_entry = tmp_entry->next;
                count++;
        }
        printf("\n");
        free(tmp_entry);
#endif
}

void test_get_whitelist_entry_by_name(void)
{
#if 1
        struct Entry *tmp_entry = NULL;
	tmp_entry = get_entry_by_name("Test", WHITELIST_FLAG);
        int count = 0;
        printf("\n");
        while ( tmp_entry != NULL ) {
                printf("[CUNIT] result #%d: +%d %d %llu - %s - %s\n", count,
                                                                      tmp_entry->country_code,
                                                                      tmp_entry->area_code,
                                                                      tmp_entry->number,
                                                                      tmp_entry->name,
                                                                      tmp_entry->reason);
                tmp_entry = tmp_entry->next;
                count++;
        }
        printf("\n");
        free(tmp_entry);
#endif
}

int main(int argc, char *argv[])
{
	CU_pSuite adminSuite = NULL;
	CU_pSuite searchSuite = NULL;

	CU_initialize_registry();

        adminSuite = CU_add_suite("Testing Phone Firewall - administration features (add, remove and check)", NULL, NULL);
	CU_add_test(adminSuite, "test of add_blacklist_entry()", test_add_blacklist_entry);
	CU_add_test(adminSuite, "test of add_whitelist_entry()", test_add_whitelist_entry);
	CU_add_test(adminSuite, "test of check_whitelist_entry()", test_check_whitelist_entry);
	CU_add_test(adminSuite, "test of check_blacklist_entry()", test_check_blacklist_entry);
	CU_add_test(adminSuite, "test of rm_blacklist_entry()", test_rm_blacklist_entry);
	CU_add_test(adminSuite, "test of rm_whitelist_entry()", test_rm_whitelist_entry);

	searchSuite = CU_add_suite("Testing Phone Firewall - searching features (by name, by number ...)", NULL, NULL);
	CU_add_test(searchSuite, "test of get_blacklist_entry_by_name()", test_get_blacklist_entry_by_name);
	CU_add_test(searchSuite, "test of get_whitelist_entry_by_name()", test_get_whitelist_entry_by_name);

        //CU_basic_set_mode(CU_BRM_NORMAL);
        //CU_basic_set_mode(CU_BRM_SILENT);
        CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_basic_run_tests();
	CU_cleanup_registry();

	return 0;
}
