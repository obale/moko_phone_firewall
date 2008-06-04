#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "libphonefirewall.h"

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

int main(int argc, char **argv) {
	start_daemon();

	/*
	 * Daemon runs... 
	 */
	sleep(10);

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

