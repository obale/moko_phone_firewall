/*
 * pf_daemon.c
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <dbus/dbus.h>
#include "pf_daemon.h"
#include "libphonefirewall.h"
#include "logfile.h"

void start_daemon();

void stop_daemon();

void dbus_listen();

int main(int argc, char **argv)
{
        start_daemon();

        /*
         * Daemon runs...
         */
        // TODO: Test the implementation and delete the sleep statement.
        dbus_listen();
        //sleep(10);

        stop_daemon();
}

void start_daemon()
{

        if ( 0 == access(LOCKFILE, 0) ) {
                fprintf(stderr, "Daemon couldn't be started.\nIs another instance started? When not please delete the file: %s\n", LOCKFILE);
                exit(1);
        }

        pid_t d_pid = fork();

        if ( 0 > d_pid ) {
                fprintf(stderr, "Error in start_daemon(): %s\n", errno);
                exit(1);
        }

        if ( 0 < d_pid ) {
                printf("Daemon started succesfully!\n");
                write_logentry("Daemon started successfully", "moksec daemon", INFO_FLAG);
                exit(0);
        } else {
                FILE *lockfile = fopen(LOCKFILE, "w");
                fprintf(lockfile, "%d\n", getpid());
                fclose(lockfile);
        }
}

void stop_daemon()
{
        printf("Daemon will be closed...\n");
        write_logentry("Daemon exit successfully", "moksec daemon", INFO_FLAG);
        remove(LOCKFILE);
        exit(0);
}

void dbus_listen()
{
        DBusMessage *msg;
        DBusMessage *reply;
        DBusMessageIter args;
        DBusConnection *conn;
        DBusError error;
        int ret;
        char *param;
        char *logentry;

        dbus_error_init(&error);

        conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
        if ( dbus_error_is_set(&error) ) {
                sprintf(logentry, "Connection Error (%s)", error.message);
                write_logentry(logentry, "moksec daemon", ERR_FLAG);
                dbus_error_free(&error);
        }
        if ( NULL == conn ) {
                sprintf(logentry, "Connection Null");
                write_logentry(logentry, "moksec daemon", ERR_FLAG);
                exit(1);
        }

        ret = dbus_bus_request_name(conn, "to.networld.moksec.phonefirewall.server", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
        if ( dbus_error_is_set(&error) ) {
                sprintf(logentry, "Name Error (%s)\n", error.message);
                write_logentry(logentry, "moksec daemon", ERR_FLAG);
                dbus_error_free(&error);
        }
        if ( DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret ) {
                sprintf(logentry, "Not Primary Owner (%d)\n", ret);
                write_logentry(logentry, "moksec daemon", ERR_FLAG);
                exit(1);
        }

        while (true) {
                dbus_connection_read_write(conn, 0);
                msg = dbus_connection_pop_message(conn);

                if ( NULL == msg ) {
                        sleep(1);
                        continue;
                }

                if ( dbus_message_is_method_call(msg, "to.networld.moksec.phonefirewall.Checking", "checkwhitelist") )
                        write_logentry("checkwhitelist called...", "moksec daemon", INFO_FLAG);
                if ( dbus_message_is_method_call(msg, "to.networld.moksec.phonefirewall.Checking", "checkblacklist") )
                        write_logentry("checkblacklist called...", "moksec daemon", INFO_FLAG);

                dbus_message_unref(msg);
        }

}
