#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# pf_testsuite.py
#
# (C) 2008 by Networld Consulting, Ltd.
# Written by Alex Oberhauser <oberhauseralex@networld.to>
# All Rights Reserved
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with MokSec.  If not, see <http://www.gnu.org/licenses/>
#
from ctypes import *
import unittest

class CheckPFAdminFunctions(unittest.TestCase):
    def test_add_blacklist_entry(self):
        # [OK]    -> Testuser 1:  Number in Italy.
        ret = pf.add_blacklist_entry(39, 328, 123456789, "Testuser 1", "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [FAILS] -> Testuser -1: The same number as "Testuser 1"
        ret = pf.add_blacklist_entry(39, 328, 123456789, "Testuser -1", "I don't like him.", 0)
        self.assertEquals(ret, -1)

        # [FAILS] -> Testuser -1: The country code is NULL.
        ret = pf.add_blacklist_entry(0, 328, 123456782, "Testuser -1", "I  don't like him.", 0)
        self.assertEquals(ret, -1)

        # [FAILS] -> Testuser -1: The area code is NULL.
        ret = pf.add_blacklist_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0)
        self.assertEquals(ret, -1)

        # [FAILS] -> Testuser -1: The area code is NULL.
        ret = pf.add_blacklist_entry(39, 328, 0, "Testuser -1", "I don't like him.", 0)
        self.assertEquals(ret, -1)

        # [OK]    -> Testuser 2:  The same area code and number as "Testuser
        #  1" but a different country code.
        ret = pf.add_blacklist_entry(49, 328, 123456789, "Testuser 2", "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 3:  The same country code and number as 
        # "Testuser 1" but a different area code
        ret = pf.add_blacklist_entry(49, 329, 123456789, "Testuser 3", "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [OK]    -> NULL      :  Username is empty.
        ret = pf.add_blacklist_entry(49, 329, 123456780, NULL, "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 4:  Reason is empty.
        ret = pf.add_blacklist_entry(49, 329, 123456781, "Testuser 4", NULL, 0)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0)
        ret = pf.add_blacklist_entry(49, 329, 222222222, "Testuser 5", NULL, 2)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0)
        ret = pf.add_blacklist_entry(49, 329, 999999999, "Testuser 6", NULL, PRIO_ALL)
        self.assertEquals(ret, 0)

    def test_add_whitelist_entry(self):
        # [OK]    -> Testuser 1:  Number in Italy.
        ret = pf.add_whitelist_entry(39, 128, 123456789, "Testuser 1", "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [FAILS] -> Testuser -1: The same number as "Testuser 1"
        ret = pf.add_whitelist_entry(39, 128, 123456789, "Testuser -1", "I don't like him.", 0)
        self.assertEquals(ret, -1)

        # [FAILS] -> Testuser -1: The country code is NULL.
        ret = pf.add_whitelist_entry(0, 128, 123456782, "Testuser -1", "I  don't like him.", 0)
        self.assertEquals(ret, -1)

        # [FAILS] -> Testuser -1: The area code is NULL.
        ret = pf.add_whitelist_entry(39, 0, 123456782, "Testuser -1", "I don't like him.", 0)
        self.assertEquals(ret, -1)

        # [FAILS] -> Testuser -1: The area code is NULL.
        ret = pf.add_whitelist_entry(39, 128, 0, "Testuser -1", "I don't like him.", 0)
        self.assertEquals(ret, -1)

        # [OK]    -> Testuser 2:  The same area code and number as "Testuser
        #  1" but a different country code.
        ret = pf.add_whitelist_entry(49, 128, 123456789, "Testuser 2", "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 3:  The same country code and number as 
        # "Testuser 1" but a different area code
        ret = pf.add_whitelist_entry(49, 129, 123456789, "Testuser 3", "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [OK]    -> NULL      :  Username is empty.
        ret = pf.add_whitelist_entry(49, 129, 123456780, NULL, "I don't like him.", 0)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 4:  Reason is empty.
        ret = pf.add_whitelist_entry(49, 129, 123456781, "Testuser 4", NULL, 0)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 5:  Priority is 2, which is higher than normal (0)
        ret = pf.add_whitelist_entry(49, 129, 222222222, "Testuser 5", NULL, 2)
        self.assertEquals(ret, 0)

        # [OK]    -> Testuser 6:  Priority is 2, which is higher than normal (0)
        ret = pf.add_whitelist_entry(49, 129, 999999999, "Testuser 6", NULL, PRIO_ALL)
        self.assertEquals(ret, 0)

    def test_check_blacklist_entry(self):
        ret = pf.check_blacklist_entry(43, 112, 11221212)
        self.assertEquals(ret, 0)

    def test_check_whitelist_entry(self):
        ret = pf.check_whitelist_entry(43, 123, 23232323)
        self.assertEquals(ret, 0)

    def test_rm_blacklist_entry(self):
       ret = pf.rm_blacklist_entry(43, 112, 11221212)
       self.assertEquals(ret, 0)

    def test_rm_whitelist_entry(self):
       ret = pf.rm_whitelist_entry(43, 123, 23232323)
       self.assertEquals(ret, 0)


class CheckPFSearchFunctions(unittest.TestCase):
    def test_get_blacklist_entry_by_name(self):
        return ""

    def test_get_whitelist_entry_by_name(self):
        return ""

    def test_get_blacklist_entry_by_number(self):
        return ""


if __name__ == "__main__":
    lib_pf_path = "lib/libtestphonefirewall.so"
    print """***********************************************************"""
    print """*        MokSec Phone Firewall - Python Testsuite         *"""
    print """*                                                         *"""
    print """* (C) 2008 by Networld Consulting, Ltd.                   *"""
    print """* Written by Alex Oberhauser <oberhauseralex@networld.to> *"""
    print """* All Rights Reserved - GPLv2                             *"""
    print """***********************************************************"""
    print """ """
    pf = cdll.LoadLibrary(lib_pf_path)

    suite_admin = unittest.TestLoader().loadTestsFromTestCase(CheckPFAdminFunctions)
    suite_search = unittest.TestLoader().loadTestsFromTestCase(CheckPFSearchFunctions)

    unittest.TextTestRunner(verbosity=1).run(suite_admin)
    unittest.TextTestRunner(verbosity=1).run(suite_search)
