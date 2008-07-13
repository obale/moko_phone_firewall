#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../src/libphonefirewall.h"
#include "../src/pf_daemon.h"


void test_add_blacklist_entry(void) {
	/* [OK]    -> Testuser 1:  Number in Italy. */
	CU_ASSERT(add_entry(39, 328, 123456789, "Testuser 1", "I don't like him.", 0, FL_BLACKLIST) == 0);

	/* [FAILS] -> Testuser -1: The same number as "Testuser 1". */
	CU_ASSERT(add_entry(39, 328, 123456789, "Testuser -1", "I don't like him.", 0, FL_BLACKLIST) == -1);

	/* [FAILS] -> Testuser -1: The country code is NULL. */
	CU_ASSERT(add_entry(0, 328, 123456782, "Testuser -1", "I don't like him.", 0, FL_BLACKLIST) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0, FL_BLACKLIST) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 328, 0, "Testuser -1", "I don't like him.", 0, FL_BLACKLIST) == -1);

	/* [OK]    -> Testuser 2:  The same area code and number as "Testuser
	 * 1" but a different country code. */
	CU_ASSERT(add_entry(49, 328, 123456789, "Testuser 2", "I don't like him.", 0, FL_BLACKLIST) == 0);

	/* [OK]    -> Testuser 3:  The same country code and number as "Testuser
	 * 1" but a different area code. */
	CU_ASSERT(add_entry(49, 329, 123456789, "Testuser 3", "I don't like him.", 0, FL_BLACKLIST) == 0);

	/* [OK]    -> NULL      :  Username is empty. */
	CU_ASSERT(add_entry(49, 329, 123456780, NULL, "I don't like him.", 0, FL_BLACKLIST) == 0);

	/* [OK]    -> Testuser 4:  Reason is empty. */
	CU_ASSERT(add_entry(49, 329, 123456781, "Testuser 4", NULL, 0, FL_BLACKLIST) == 0);

	/* [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 329, 222222222, "Testuser 5", NULL, 2, FL_BLACKLIST) == 0);

	/* [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 329, 999999999, "Testuser 6", NULL, PRIO_ALL, FL_BLACKLIST) == 0);
}

void test_rm_blacklist_entry(void) {
	CU_ASSERT(rm_entry(49, 329, 999999999, FL_BLACKLIST) == 0);
	CU_ASSERT(rm_entry(39, 328, 123456789, FL_BLACKLIST) == 0);
}

void test_add_whitelist_entry(void) {
	/* [OK]    -> Testuser 1:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456789, "Testuser 1", "I don't like him.", 0, FL_WHITELIST) == 0);

	/* [FAILS] -> Testuser -1: The same number as "Testuser 1". */
	CU_ASSERT(add_entry(39, 128, 123456789, "Testuser -1", "I don't like him.", 0, FL_WHITELIST) == -1);

	/* [FAILS] -> Testuser -1: The country code is NULL. */
	CU_ASSERT(add_entry(0, 128, 123456782, "Testuser -1", "I don't like him.", 0, FL_WHITELIST) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0, FL_WHITELIST) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_entry(39, 128, 0, "Testuser -1", "I don't like him.", 0, FL_WHITELIST) == -1);

	/* [OK]    -> Testuser 2:  The same area code and number as "Testuser
	 * 1" but a different country code. */
	CU_ASSERT(add_entry(49, 128, 123456789, "Testuser 2", "I don't like him.", 0, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 3:  The same country code and number as "Testuser
	 * 1" but a different area code. */
	CU_ASSERT(add_entry(49, 129, 123456789, "Testuser 3", "I don't like him.", 0, FL_WHITELIST) == 0);

	/* [OK]    -> NULL      :  Username is empty. */
	CU_ASSERT(add_entry(49, 129, 123456780, NULL, "I don't like him.", 0, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 4:  Reason is empty. */
	CU_ASSERT(add_entry(49, 129, 123456781, "Testuser 4", NULL, 0, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 129, 222222222, "Testuser 5", NULL, 2, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_entry(49, 129, 999999999, "Testuser 6", NULL, PRIO_ALL, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 7:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456783, "Testuser 7", "I don't like him.", 2, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 8:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456784, "Testuser 8", "I don't like him.", 5, FL_WHITELIST) == 0);

	/* [OK]    -> Testuser 9:  Number in Italy. */
	CU_ASSERT(add_entry(39, 128, 123456785, "Testuser 9", "I don't like him.", 10, FL_WHITELIST) == 0);
}

void test_rm_whitelist_entry(void) {
	CU_ASSERT(rm_entry(49, 129, 999999999, FL_WHITELIST) == 0);
	CU_ASSERT(rm_entry(39, 128, 123456789, FL_WHITELIST) == 0);
}

void test_check_blacklist_entry(void) {
	CU_ASSERT(check_entry(39, 328, 123456789, 0, FL_BLACKLIST) == 1);
	CU_ASSERT(check_entry(39, 328, 103456785, 0, FL_BLACKLIST) == 0);
	CU_ASSERT(check_entry(49, 329, 999999999, 0, FL_BLACKLIST) == 1);
	CU_ASSERT(check_entry(49, 329, 222222222, 0, FL_BLACKLIST) == 0);
	CU_ASSERT(check_entry(49, 329, 222222222, 2, FL_BLACKLIST) == 1);
	CU_ASSERT(check_entry(49, 329, 222222222, 3, FL_BLACKLIST) == 1);
}

void test_check_whitelist_entry(void) {
	CU_ASSERT(check_entry(39, 128, 123456789, 0, FL_WHITELIST) == 1);
	CU_ASSERT(check_entry(39, 128, 103456785, 0, FL_WHITELIST) == 0);
	CU_ASSERT(check_entry(49, 129, 999999999, 0, FL_WHITELIST) == 1);
	CU_ASSERT(check_entry(49, 129, 222222222, 0, FL_WHITELIST) == 0);
	CU_ASSERT(check_entry(49, 129, 222222222, 2, FL_WHITELIST) == 1);
	CU_ASSERT(check_entry(49, 129, 222222222, 3, FL_WHITELIST) == 1);
}

void test_start_daemon(void) {
}

void test_stop_daemon(void) {
}

void test_get_blacklist_entry_by_name(void) {
        struct Entry *entry_array;
	CU_ASSERT( (entry_array = get_blacklist_entry_by_name("user")) != NULL );

        printf("\ntest_get_blacklist_entry_by_name(void):\n");
        int count = 0;
        for ( count = 0; count < 4; count++)
                printf("+%d %d %llu -> %s\n", entry_array[count].country_code, entry_array[count].area_code, entry_array[count].number, entry_array[count].name);
}

int main(int argc, char *argv[]) {
	CU_pSuite adminSuite = NULL;
	CU_pSuite searchSuite = NULL;
	CU_pSuite daemonSuite = NULL;

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

        daemonSuite = CU_add_suite("Testing Phone Firewall - daemon features", NULL, NULL);
	CU_add_test(daemonSuite, "test of start_daemon()", test_start_daemon);
	CU_add_test(daemonSuite, "test of stop_daemon()", test_stop_daemon);

        CU_basic_set_mode(CU_BRM_NORMAL);
        //CU_basic_set_mode(CU_BRM_SILENT);
        //CU_basic_set_mode(CU_BRM_VERBOSE);

	CU_basic_run_tests();
	CU_cleanup_registry();

	return 0;
}
