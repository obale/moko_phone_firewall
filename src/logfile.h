#define LOGFILE "log/moksec.log"
#define MAX_ENTRY_LENGTH 128

#define UNKNOWN     0 /** Not specified                              */
#define ERR_FLAG    1 /** If an error message should be written      */
#define WARN_FLAG   2 /** If a warning message should be written     */
#define INFO_FLAG   3 /** If a information message should be written */

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
int write_logentry(char *msg, char *component, int flag);
