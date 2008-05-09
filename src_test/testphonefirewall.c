#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "libphonefirewall.h"


void test_add_blacklist_entry(void) {
	CU_PASS("A success message.");
	CU_ASSERT(add_blacklist_entry("+393286823221", "foobar", "spammer", 0) == 0);
	CU_ASSERT(add_blacklist_entry("+393286823222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222222", "foobar", "spammer", 0) == -EOVERFLOW);
}

void test_rm_blacklist_entry(void) {
	CU_PASS("A success message.");
}

void test_add_whitelist_entry(void) {
	CU_PASS("A success message.");
}

void test_rm_whitelist_entry(void) {
	CU_PASS("A success message.");
}

int main(int argc, char *argv[]) {
	CU_pSuite pSuite = NULL;

	CU_initialize_registry();

   	pSuite = CU_add_suite("Testing Phone Firewall", NULL, NULL); 
	CU_add_test(pSuite, "test of add_blacklist_entry()", test_add_blacklist_entry);
	CU_add_test(pSuite, "test of rm_blacklist_entry()", test_rm_blacklist_entry);
	CU_add_test(pSuite, "test of add_whitelist_entry()", test_add_whitelist_entry);
	CU_add_test(pSuite, "test of rm_whitelist_entry()", test_rm_whitelist_entry);
	
   	CU_basic_set_mode(CU_BRM_VERBOSE);

	// Start basic tests
	CU_basic_run_tests();

	// Cleaning up (freeing memory and so one)
	CU_cleanup_registry();
	
	return 0;
}
