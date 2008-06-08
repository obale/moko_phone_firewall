GCC = /usr/bin/gcc
RM = /bin/rm
CLFLAGS += --std=c99 -Wall 
TEST_LIB = -lcunit -ltestphonefirewall `pkg-config --libs --cflags sqlite3`
DOXYGEN = /usr/bin/doxygen
SRCDIR = src
BINDIR = bin
LIBDIR = lib
SRCTESTDIR = src_test
BINTESTDIR = bin_test

all: 	test

#all: 	libphonefirewall.so\
	test

.PHONY: doc
doc: 
	@ echo -en 'Creating a documentation with doxygen \t\t\t\t\t\t'; 
	@ $(DOXYGEN) &> /dev/null\
	 && ( echo -e '[\E[32mDONE\033[0m]'; tput sgr0 )\
	 || ( echo -e '[\E[31mFAILED\033[0m]'; tput sgr0 )

phonefirewall_administration.o: $(SRCDIR)/phonefirewall_administration.c $(SRCDIR)/libphonefirewall.h
	$(CC) $(CLFLAGS) -fpic -c $(SRCDIR)/phonefirewall_administration.c -o $(SRCDIR)/$@

libphonefirewall.so: $(SRCDIR)/phonefirewall_administration.o 
	$(CC) -shared $(SRCDIR)/phonefirewall_administration.o -o $(LIBDIR)/$@

pf_daemon.o: $(SRCDIR)/pf_daemon.c $(SRCDIR)/libphonefirewall.h
	$(CC) $(CFLAGS) `pkg-config --cflags --libs dbus-1` -c $(SRCDIR)/pf_daemon -o $(SRCDIR)/$@

pf_daemon: $(SRCDIR)/pf_daemon.c
	$(CC) `pkg-config --cflags --libs dbus-1` $(SRCDIR)/pf_daemon.c -o $(BINDIR)/$@


# 
# Begining of the testing part.
#
test: logfile.o testphonefirewall_administration.o libtestphonefirewall.so testphonefirewall

testphonefirewall: $(SRCTESTDIR)/testphonefirewall.c 
	$(GCC) $(CLFLAGS) $(SRCTESTDIR)/testphonefirewall.c -L$(LIBDIR) $(TEST_LIB) -o $(BINTESTDIR)/$@

testphonefirewall_administration.o: $(SRCDIR)/phonefirewall_administration.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/phonefirewall_administration.c -o $(SRCTESTDIR)/$@

libtestphonefirewall.so: $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCDIR)/logfile.o
	$(GCC) $(CLFLAGS) -shared $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCDIR)/logfile.o -o $(LIBDIR)/$@

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
