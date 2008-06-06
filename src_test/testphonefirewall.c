#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../src/libphonefirewall.h"


void test_add_blacklist_entry(void) {
	/* [OK]    -> Testuser 1:  Number in Italy. */
	CU_ASSERT(add_blacklist_entry(39, 328, 123456789, "Testuser 1", "I don't like him.", 0) == 0);
	
	/* [FAILS] -> Testuser -1: The same number as "Testuser 1". */
	CU_ASSERT(add_blacklist_entry(39, 328, 123456789, "Testuser -1", "I don't like him.", 0) == -1);

	/* [FAILS] -> Testuser -1: The country code is NULL. */
	CU_ASSERT(add_blacklist_entry(0, 328, 123456782, "Testuser -1", "I don't like him.", 0) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_blacklist_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_blacklist_entry(39, 328, 0, "Testuser -1", "I don't like him.", 0) == -1);

	/* [OK]    -> Testuser 2:  The same area code and number as "Testuser
	 * 1" but a different country code. */
	CU_ASSERT(add_blacklist_entry(49, 328, 123456789, "Testuser 2", "I don't like him.", 0) == 0);

	/* [OK]    -> Testuser 3:  The same country code and number as "Testuser
	 * 1" but a different area code. */
	CU_ASSERT(add_blacklist_entry(49, 329, 123456789, "Testuser 3", "I don't like him.", 0) == 0);

	/* [OK]    -> NULL      :  Username is empty. */
	CU_ASSERT(add_blacklist_entry(49, 329, 123456780, NULL, "I don't like him.", 0) == 0);

	/* [OK]    -> Testuser 4:  Reason is empty. */
	CU_ASSERT(add_blacklist_entry(49, 329, 123456781, "Testuser 4", NULL, 0) == 0);

	/* [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_blacklist_entry(49, 329, 222222222, "Testuser 5", NULL, 2) == 0);

	/* [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_blacklist_entry(49, 329, 999999999, "Testuser 6", NULL, PRIO_ALL) == 0);
}

void test_rm_blacklist_entry(void) {
	CU_ASSERT(rm_blacklist_entry(49, 329, 999999999) == 0);
	CU_ASSERT(rm_blacklist_entry(39, 328, 123456789) == 0);
}

void test_add_whitelist_entry(void) {
	/* [OK]    -> Testuser 1:  Number in Italy. */
	CU_ASSERT(add_whitelist_entry(39, 128, 123456789, "Testuser 1", "I don't like him.", 0) == 0);
	
	/* [FAILS] -> Testuser -1: The same number as "Testuser 1". */
	CU_ASSERT(add_whitelist_entry(39, 128, 123456789, "Testuser -1", "I don't like him.", 0) == -1);

	/* [FAILS] -> Testuser -1: The country code is NULL. */
	CU_ASSERT(add_whitelist_entry(0, 128, 123456782, "Testuser -1", "I don't like him.", 0) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_whitelist_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0) == -1);

	/* [FAILS] -> Testuser -1: The area code is NULL. */
	CU_ASSERT(add_whitelist_entry(39, 128, 0, "Testuser -1", "I don't like him.", 0) == -1);

	/* [OK]    -> Testuser 2:  The same area code and number as "Testuser
	 * 1" but a different country code. */
	CU_ASSERT(add_whitelist_entry(49, 128, 123456789, "Testuser 2", "I don't like him.", 0) == 0);

	/* [OK]    -> Testuser 3:  The same country code and number as "Testuser
	 * 1" but a different area code. */
	CU_ASSERT(add_whitelist_entry(49, 129, 123456789, "Testuser 3", "I don't like him.", 0) == 0);

	/* [OK]    -> NULL      :  Username is empty. */
	CU_ASSERT(add_whitelist_entry(49, 129, 123456780, NULL, "I don't like him.", 0) == 0);

	/* [OK]    -> Testuser 4:  Reason is empty. */
	CU_ASSERT(add_whitelist_entry(49, 129, 123456781, "Testuser 4", NULL, 0) == 0);

	/* [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_whitelist_entry(49, 129, 222222222, "Testuser 5", NULL, 2) == 0);

	/* [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0) . */
	CU_ASSERT(add_whitelist_entry(49, 129, 999999999, "Testuser 6", NULL, PRIO_ALL) == 0);

	/* [OK]    -> Testuser 7:  Number in Italy. */
	CU_ASSERT(add_whitelist_entry(39, 128, 123456783, "Testuser 7", "I don't like him.", 2) == 0);

	/* [OK]    -> Testuser 8:  Number in Italy. */
	CU_ASSERT(add_whitelist_entry(39, 128, 123456784, "Testuser 8", "I don't like him.", 5) == 0);

	/* [OK]    -> Testuser 9:  Number in Italy. */
	CU_ASSERT(add_whitelist_entry(39, 128, 123456785, "Testuser 9", "I don't like him.", 10) == 0);
}

void test_rm_whitelist_entry(void) {
	CU_ASSERT(rm_blacklist_entry(49, 129, 999999999) == 0);
	CU_ASSERT(rm_blacklist_entry(39, 128, 123456789) == 0);
}

void test_check_blacklist_entry(void) {
	//CU_ASSERT(check_blacklist_entry(39, 328, 123456789, 0) == 1);
	CU_ASSERT(check_blacklist_entry(39, 328, 103456785, 0) == 0);
	//CU_ASSERT(check_blacklist_entry(49, 329, 999999999, 0) == 1);
	CU_ASSERT(check_blacklist_entry(49, 329, 222222222, 0) == 0);
	CU_ASSERT(check_blacklist_entry(49, 329, 222222222, 2) == 1);
	CU_ASSERT(check_blacklist_entry(49, 329, 222222222, 3) == 1);
}

void test_check_whitelist_entry(void) {
	CU_ASSERT(check_whitelist_entry(39, 128, 123456789, 0) == 1);
	CU_ASSERT(check_whitelist_entry(39, 128, 103456785, 0) == 0);
	CU_ASSERT(check_whitelist_entry(49, 129, 999999999, 0) == 1);
	CU_ASSERT(check_whitelist_entry(49, 129, 222222222, 0) == 0);
	CU_ASSERT(check_whitelist_entry(49, 129, 222222222, 2) == 1);
	CU_ASSERT(check_whitelist_entry(49, 129, 222222222, 3) == 1);
}

int main(int argc, char *argv[]) {
	CU_pSuite pSuite = NULL;

	CU_initialize_registry();

   	pSuite = CU_add_suite("Testing Phone Firewall", NULL, NULL); 
	CU_add_test(pSuite, "test of add_blacklist_entry()", test_add_blacklist_entry);
	CU_add_test(pSuite, "test of add_whitelist_entry()", test_add_whitelist_entry);
	/* 
	 * XXX: check_blacklist_entry() before check_whitelist_entry() doesn't
	 * work.
	 */
	CU_add_test(pSuite, "test of check_whitelist_entry()", test_check_whitelist_entry);
	CU_add_test(pSuite, "test of check_blacklist_entry()", test_check_blacklist_entry);
	CU_add_test(pSuite, "test of rm_blacklist_entry()", test_rm_blacklist_entry);
	CU_add_test(pSuite, "test of rm_whitelist_entry()", test_rm_whitelist_entry);
	
   	CU_basic_set_mode(CU_BRM_NORMAL);
   	//CU_basic_set_mode(CU_BRM_SILENT);
   	//CU_basic_set_mode(CU_BRM_VERBOSE);

	// Start basic tests
	CU_basic_run_tests();

	// Cleaning up (freeing memory and so one)
	CU_cleanup_registry();

	return 0;
}
