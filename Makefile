GCC = /usr/bin/gcc
RM = /bin/rm
CLFLAGS += --std=c99 -Wall -Werror
TEST_LIB = -lcunit -ltestphonefirewall `pkg-config --libs --cflags sqlite3`
DOXYGEN = /usr/bin/doxygen
SRCDIR = src
BINDIR = bin
LIBDIR = lib
SRCTESTDIR = src_test
BINTESTDIR = bin_test

test: logfile.o testphonefirewall_administration.o testphonefirewall_search.o libtestphonefirewall.so testphonefirewall

daemon: pf_daemon.o logfile.o pf_daemon

all: 	libphonefirewall.so\
	daemon\
	test

.PHONY: doc
doc: 
	@ echo -en 'Creating a documentation with doxygen \t\t\t\t\t\t'; 
	@ $(DOXYGEN) &> /dev/null\
	 && ( echo -e '[\E[32mDONE\033[0m]'; tput sgr0 )\
	 || ( echo -e '[\E[31mFAILED\033[0m]'; tput sgr0 )

pf_administration.o: $(SRCDIR)/pf_administration.c $(SRCDIR)/libphonefirewall.h
	$(CC) $(CLFLAGS) -fpic -c $(SRCDIR)/pf_administration.c -o $(SRCDIR)/$@

pf_search.o: $(SRCDIR)/pf_search.c $(SRCDIR)/libphonefirewall.h
	$(CC) $(CLFLAGS) -fpic -c $(SRCDIR)/pf_search.c -o $(SRCDIR)/$@

libphonefirewall.so: $(SRCDIR)/pf_administration.o $(SRCDIR)/pfl_search.o
	$(CC) -shared $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o -o $(LIBDIR)/$@

pf_daemon.o: $(SRCDIR)/pf_daemon.c $(SRCDIR)/libphonefirewall.h $(SRCDIR)/pf_daemon.h $(SRCDIR)/logfile.h
	$(GCC) `pkg-config --cflags dbus-1` -c $(SRCDIR)/pf_daemon.c -o $(SRCDIR)/$@

pf_daemon: $(SRCDIR)/pf_daemon.o $(SRCDIR)/logfile.o
	$(GCC) `pkg-config --cflags --libs dbus-1` $(SRCDIR)/pf_daemon.o $(SRCDIR)/logfile.o -o $(BINDIR)/$@


# 
# Begining of the testing part.
#

testphonefirewall: $(SRCTESTDIR)/testphonefirewall.c 
	$(GCC) $(CLFLAGS) $(SRCTESTDIR)/testphonefirewall.c -L$(LIBDIR) $(TEST_LIB) -o $(BINTESTDIR)/$@

testphonefirewall_administration.o: $(SRCDIR)/pf_administration.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_administration.c -o $(SRCTESTDIR)/$@

testphonefirewall_search.o: $(SRCDIR)/pf_search.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_search.c -o $(SRCTESTDIR)/$@

libtestphonefirewall.so: $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/testphonefirewall_search.o $(SRCDIR)/logfile.o
	$(GCC) $(CLFLAGS) `pkg-config --libs --cflags sqlite3` -shared $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/testphonefirewall_search.o $(SRCDIR)/logfile.o -o $(LIBDIR)/$@

logfile.o: $(SRCDIR)/logfile.c $(SRCDIR)/logfile.h
	$(GCC) -fpic -c $(SRCDIR)/logfile.c -o $(SRCDIR)/$@

.PHONY: clean
clean: 
	$(RM) -f lib/*\
	       	 src/*.o\
		 src_test/*.o\
		 bin_test/*\
		 db/*\
		 log/moksec.log
	sqlite3 db/phone-firewall.db < phonefirewall.sql;

.PHONY: clean-db
clean-db:
	$(RM) -f db/*
