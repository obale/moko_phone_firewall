#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "../src/libphonefirewall.h"


void test_add_blacklist_entry(void) {
	CU_ASSERT(add_blacklist_entry(39, 328, 1231313212, "John Smith", "I don't like him.", 0) == 0);
	CU_ASSERT(add_blacklist_entry(39, 328, 3344242343, "Testuser 1", "I don't like him.", 0) == 0);
	CU_ASSERT(add_blacklist_entry(43, 328, 1234324223, "Testuser 2", "I don't like him.", 0) == 0);
	CU_ASSERT(add_blacklist_entry(43, 328, 9343423233, "Testuser 3", "I don't like him.", 0) == 0);
	CU_ASSERT(add_blacklist_entry(43, 328, 3424223433, "Testuser 4", "I don't like him.", 0) == 0);
}

void test_rm_blacklist_entry(void) {
}

void test_add_whitelist_entry(void) {
	CU_ASSERT(add_whitelist_entry(43, 1234, 1231313212, "John Smith", "I like him.", 0) == 0);
}

void test_rm_whitelist_entry(void) {
}

void test_check_blacklist_entry(void) {
	CU_ASSERT(NULL != check_blacklist_entry(39, 328, 1231313212) );
	CU_ASSERT(NULL != check_blacklist_entry(43, 328, 1234324223) );
	CU_ASSERT(NULL == check_blacklist_entry(39, 328, 283732) );
	CU_ASSERT(NULL == check_blacklist_entry(43, 328, 1231313212) );
}

void test_check_whitelist_entry(void) {
	CU_ASSERT(NULL != check_whitelist_entry(43, 1234, 1231313212) );
	CU_ASSERT(NULL == check_whitelist_entry(43, 1234, 2323232223) );	
	CU_ASSERT(NULL == check_whitelist_entry(43, 1234, 1231313213) );
}

int main(int argc, char *argv[]) {
	CU_pSuite pSuite = NULL;

	CU_initialize_registry();

   	pSuite = CU_add_suite("Testing Phone Firewall", NULL, NULL); 
	CU_add_test(pSuite, "test of add_blacklist_entry()", test_add_blacklist_entry);
	CU_add_test(pSuite, "test of rm_blacklist_entry()", test_rm_blacklist_entry);
	CU_add_test(pSuite, "test of add_whitelist_entry()", test_add_whitelist_entry);
	CU_add_test(pSuite, "test of rm_whitelist_entry()", test_rm_whitelist_entry);
	CU_add_test(pSuite, "test of check_blacklist_entry()", test_check_blacklist_entry);
	CU_add_test(pSuite, "test of check_whitelist_entry()", test_check_whitelist_entry);
	
   	CU_basic_set_mode(CU_BRM_VERBOSE);

	// Start basic tests
	CU_basic_run_tests();

	// Cleaning up (freeing memory and so one)
	CU_cleanup_registry();
	
	return 0;
}
