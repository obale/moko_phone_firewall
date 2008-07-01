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
        # TODO: -1 seams the wrong value here, means that an error occurs
        # during the call of the function.
        ret = pf.add_blacklist_entry(43, 112, 11221212, "Badboy", "mad guy", 0)
        self.assertEquals(ret, -1)

    def test_add_whitelist_entry(self):
        # TODO: -1 seams the wrong value here, means that an error occurs
        # during the call of the function.
        ret = pf.add_whitelist_entry(43, 123, 23232323, "Me", "goog guy", 10)
        self.assertEquals(ret, -1);

    def test_rm_blacklist_entry(self):
        ret = pf.rm_blacklist_entry(43, 112, 11221212)
        self.assertEquals(ret, 0)

    def test_rm_whitelist_entry(self):
        ret = pf.rm_whitelist_entry(43, 123, 23232323)
        self.assertEquals(ret, 0)

if __name__ == "__main__":
    lib_pf_path = "lib/libtestphonefirewall.so"
    print """ """
    print """Loading the libphonefirewall libary"""
    print """Full library path: """, lib_pf_path
    print """ """
    pf = cdll.LoadLibrary(lib_pf_path)

    suite_admin = unittest.TestLoader().loadTestsFromTestCase(CheckPFAdminFunctions)

    unittest.TextTestRunner(verbosity=2).run(suite_admin)
