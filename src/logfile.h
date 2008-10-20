/*
 * logfile.h
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

#define LOGFILE "/tmp/phonefirewall.log"
#define MAX_ENTRY_LENGTH 128

#define UNKNOWN     0 /** Not specified                              */
#define ERR_FLAG    1 /** If an error message should be written      */
#define WARN_FLAG   2 /** If a warning message should be written     */
#define INFO_FLAG   3 /** If a information message should be written */

#define UNKNOWN_LOG(m) write_logentry(m, COMPONENT_NAME, UNKNOWN_FLAG)
#define ERR_LOG(m) write_logentry(m, COMPONENT_NAME, ERR_FLAG)
#define WARN_LOG(m) write_logentry(m, COMPONENT_NAME, WARN_FLAG)
#define INFO_LOG(m) write_logentry(m, COMPONENT_NAME, INFO_FLAG)

/**
 * Writes a logfile enty.
 *
 * @param msg The message which should be written in the logfile.
 * @param component The program which calls the write_logentry function, for
 * example "phonefirewall"
 * @param flag What message should be written. Use the defined flags.
 *
 * @return -1 if something fails, otherwise 0
 */
int write_logentry(char *msg,
                   char *component,
                   int flag);
