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

