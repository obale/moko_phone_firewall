INCLUDES = -I$(top_srcdir)

AM_CPPFLAGS = \
  -DPKGDATADIR=\"$(pkgdatadir)\" \
  -DTERMINAL_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
  -DDATADIR=\""$(datadir)"\" -D_GNU_SOURCE

AM_CFLAGS = -Wall -pedantic -std=c99 

# add the name of your application
pkglib_LTLIBRARIES = libphonefirewall.la

libphonefirewall_la_SOURCES   = \
	libphonefirewall.h \
	phonefirewall_administration.c 

libphonefirewall_la_LDFLAGS = --version 0:0:0

MAINTAINERCLEANFILES  = config.h.in Makefile.in
