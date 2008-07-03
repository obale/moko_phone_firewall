#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# daemon_dbus_testsuite.py
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
import dbus

# TODO: Implement here the d-bus client for the quick & dirty testing of the
# daemon d-bus service

def PrintHeader():
    print """
    ***********************************************************
    *           MokSec Daemon - Python Testsuite              *
    *                                                         *
    * (C) 2008 by Networld Consulting, Ltd.                   *
    * Written by Alex Oberhauser <oberhauseralex@networld.to> *
    * All Rights Reserved - GPLv2                             *
    ***********************************************************
    """

class DBusClient:
    def InitSession(self):
        session_bus = dbus.SessionBus()
        pf = bus.get_object('to.networld.moksec.phone-firewall', '/to/networld/moksec/phone-firewall/Checking')

    def SendSignal(self):
        return ' '

if __name__ == "__main__":
    PrintHeader()
