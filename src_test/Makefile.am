INCLUDES = -I$(top_srcdir)

AM_CPPFLAGS = \
  -DPKGDATADIR=\"$(pkgdatadir)\" \
  -DTERMINAL_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
  -DDATADIR=\""$(datadir)"\" -D_GNU_SOURCE

#AM_CFLAGS = -Wall -pedantic -std=c99 @DEPENDENCIES_CFLAGS@ 
AM_CFLAGS = -Wall -pedantic -std=c99 -lcunit -Lsrc/ -lphonefirewall

# add the name of your application
bin_PROGRAMS = testphonefirewall

# add the sources to compile for your application
testphonefirewall_SOURCES = \
	testphonefirewall.c

#testphonefirewall_LDADD  = @DEPENDENCIES_LIBS@@


MAINTAINERCLEANFILES  = config.h.in Makefile.in

