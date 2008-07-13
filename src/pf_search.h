/*
 * pf_search.h
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
 * along with MokSec.  If not, see <http://www.gnu.org/licenses/>
 */

/**
 * Search a entrie by name.
 *
 * @param name The name of the person which is blocked.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_blacklist_entry_by_name(char *name);

/**
 * Search a entrie by number (country code + area code + number).
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_blacklist_entry_by_number(int country_code,
                                            int area_code,
                                            unsigned long long number);

/**
 * Search a entrie by name.
 *
 * @param name The name of the person which is accepted.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_whitelist_entry_by_name(char *name);

/**
 * Search a entrie by number (country code + area code + number).
 *
 * @param country_code The country code (for example 39 for Italy, 43 for
 * Austria, and so one)
 * @param area_code The area code which indicates your mobile operator.
 * @param number The telephone number of the person (without country and area
 * code.
 *
 * @return entry Returns the found entry.
 */
struct Entry *get_whitelist_entry_by_number(int country_code,
                                            int area_code,
                                            unsigned long long number);
