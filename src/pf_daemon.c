#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <dbus/dbus.h>
#include "pf_daemon.h"
#include "libphonefirewall.h"

void start_daemon();

void stop_daemon();

void dbus_listen(); 

int main(int argc, char **argv) {
	start_daemon();

	/*
	 * Daemon runs... 
	 */
	dbus_listen();

	stop_daemon();
}

void start_daemon() {

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
		exit(0);
	} else {
		FILE *lockfile = fopen(LOCKFILE, "w");
		fprintf(lockfile, "%d\n", getpid());
		fclose(lockfile);
	}
}

void stop_daemon() {
	printf("Daemon will be closed...\n");
	remove(LOCKFILE);
	exit(0);
}

void dbus_listen() {
	DBusMessage *msg;
	DBusMessage *reply;
	DBusMessageIter args;
	DBusConnection *conn;
	DBusError error;
	int ret;
	char *param;

	dbus_error_init(&error);

	conn = dbus_bus_get(DBUS_BUS_SESSION, &error);
	if ( dbus_error_is_set(&error) ) {
		// TODO: Substitude with a log entry
		fprintf(stderr, "Connection Error (%s)\n", error.message);
		dbus_error_free(&error);
	}
	if ( NULL == conn ) {
		fprintf(stderr, "Connection Null\n");
		exit(1);
	}

	ret = dbus_bus_request_name(conn, "to.networld.moksec.phonefirewall", DBUS_NAME_FLAG_REPLACE_EXISTING, &error);
	if ( dbus_error_is_set(&error) ) {
		// TODO: Substitude with a log entry
		fprintf(stderr, "Name Error (%s)\n", error.message);
		dbus_error_free(&error);
	}
	if ( DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER != ret ) {
		fprintf(stderr, "Not Primary Owner (%d)\n", ret);
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
			printf("Here comes the method call check_blacklist_entry(...)\n");
		if ( dbus_message_is_method_call(msg, "to.networld.moksec.phonefirewall.Checking", "checkblacklist") )
			printf("Here comes the method call check_whitelist_entry(...)\n");

		dbus_message_unref(msg);
	}

}
