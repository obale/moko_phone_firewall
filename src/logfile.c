/*
 * logfile.c
 *
 * (C) 2008 by MokSec Project
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
 * along with MokSec.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "logfile.h"

/**
 * Compounds a humand readable date and time string.
 *
 * @param timeptr A pointer to the actual time.
 *
 * @return The date and time as a string.
 */
char *asctime(const struct tm *timeptr)
{
        /*
        static char wday_name[7][3] = {
                "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
        };
        */

        static char mon_name[12][3] = {
                "Jan", "Feb", "Mar", "Apr", "May", "Jun",
                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
        };

        static char result[26];

        sprintf(result, "%.2d/%.3s/%3d:%.2d:%.2d:%.2d",
                timeptr->tm_mday,
                mon_name[timeptr->tm_mon],
                1900 + timeptr->tm_year,
                timeptr->tm_hour,
                timeptr->tm_min, timeptr->tm_sec);

        return result;
}

int write_logentry(char *msg,
                   char *component,
                   int flag)
{
        FILE *fd;
        char entry_msg[MAX_ENTRY_LENGTH];
        int choice = flag;

        time_t now = time(NULL);
        char *current_time = asctime(localtime(&now));

        if ( NULL == (fd = fopen(LOGFILE, "a")) ) {
                return -1;
        }

        switch (choice) {
        case UNKNOWN: sprintf(entry_msg, "UNKNOWN\t[%s] %15s - %s\n", current_time, component, msg);
                break;
        case ERR_FLAG: sprintf(entry_msg, "ERROR\t[%s] %15s - %s\n", current_time, component, msg);
                break;
        case WARN_FLAG: sprintf(entry_msg, "WARNING\t[%s] %15s - %s\n", current_time, component, msg);
                break;
        case INFO_FLAG: sprintf(entry_msg, "INFO\t[%s] %15s - %s\n", current_time, component, msg);
                break;
        default: sprintf(entry_msg, "UNKNOWN\t[%s] %15s - %s\n", current_time, component, msg);
        }

        fprintf(fd, "%s", entry_msg);

        fclose(fd);

        return 0;

}
