/*
 * pf_daemon.h
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
 
#define LOCKFILE ".lock"

/**
 * Starts the program as a daemon and creates a lockfile (specified in the
 * LOCKFILE constant). So it's impossible to start the daemon twice.
 */
void start_daemon();

/**
 * Stops the daemon and deletes the lockfile, so a new instance can be
 * started.
 */
void stop_daemon();

/**
 * Function that provides method calls.
 *
 *    Bus name: to.networld.moksec.phonefirewall 
 * Object name: to.networld.moksec.phonefirewall.Object
 *   interface: to.networld.moksec.phonefirewall.Checking
 *     methods: checkblacklist(...)
 *              checkwhitelist(...)
 */
void dbus_listen(); 

