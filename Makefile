GCC = /usr/bin/gcc
RM = /bin/rm
CLFLAGS += --std=c99 -Wall -Werror -DDEBUG -g -o -O0
TEST_LIB = -lcunit -ltestphonefirewall `pkg-config --libs --cflags sqlite3`
DOXYGEN = /usr/bin/doxygen
SRCDIR = src
BINDIR = bin
LIBDIR = lib
SRCTESTDIR = src_test
BINTESTDIR = bin_test

test: testlogfile.o testphonefirewall_administration.o testphonefirewall_search.o libtestphonefirewall.so testphonefirewall

all: 	libphonefirewall.so\
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

logfile.o: $(SRCDIR)/logfile.c $(SRCDIR)/logfile.h
	$(CC) -fpic -c $(SRCDIR)/logfile.c -o $(SRCDIR)/$@

libphonefirewall.so: $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o $(SRCDIR)/logfile.o
	$(CC) -shared $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o -o $(LIBDIR)/$@

# 
# Begining of the testing part.
#

testphonefirewall: $(SRCTESTDIR)/testphonefirewall.c 
	$(GCC) $(CLFLAGS) $(SRCTESTDIR)/testphonefirewall.c -L$(LIBDIR) $(TEST_LIB) -o $(BINTESTDIR)/$@

testphonefirewall_administration.o: $(SRCDIR)/pf_administration.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_administration.c -o $(SRCTESTDIR)/$@

testphonefirewall_search.o: $(SRCDIR)/pf_search.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_search.c -o $(SRCTESTDIR)/$@

libtestphonefirewall.so: $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/testphonefirewall_search.o $(SRCDIR)/testlogfile.o
	$(GCC) $(CLFLAGS) -shared $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/testphonefirewall_search.o $(SRCDIR)/testlogfile.o -o $(LIBDIR)/$@

testlogfile.o: $(SRCDIR)/logfile.c $(SRCDIR)/logfile.h
	$(GCC) -fpic -c $(SRCDIR)/logfile.c -o $(SRCDIR)/$@

# XXX: Only a quick hack to find the memory corruption error.
main_test: $(SRCTESTDIR)/main_test.c $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/testphonefirewall_search.o
	$(GCC) $(CLFLAGS) -L$(LIBDIR) $(TEST_LIB) $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/testphonefirewall_search.o $(SRCTESTDIR)/main_test.c -o $(BINTESTDIR)/$@

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
