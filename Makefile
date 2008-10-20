GCC = /usr/bin/gcc
RM = /bin/rm
CLFLAGS += --std=c99 -Wall -Werror -DDEBUG -g -o -O0
TEST_LIB = -lcunit -lphonefirewall_x86 `pkg-config --libs --cflags sqlite3`
DOXYGEN = /usr/bin/doxygen
SRCDIR = src
BINDIR = bin
LIBDIR = lib
SRCTESTDIR = src_test
BINTESTDIR = bin_test

test: logfile_x86.o phonefirewall_administration_x86.o phonefirewall_search_x86.o libphonefirewall_x86.so phonefirewall_x86

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
	$(CC) $(CLFLAGS) -fpic -c $(SRCDIR)/logfile.c -o $(SRCDIR)/$@

libphonefirewall.so: $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o $(SRCDIR)/logfile.o
	$(CC) -shared $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o $(SRCDIR)/logfile.o -o $(LIBDIR)/$@

# 
# Begining of the testing part.
#

phonefirewall_x86: $(SRCTESTDIR)/testphonefirewall.c 
	$(GCC) $(CLFLAGS) $(SRCTESTDIR)/testphonefirewall.c -L$(LIBDIR) $(TEST_LIB) -o $(BINTESTDIR)/$@

phonefirewall_administration_x86.o: $(SRCDIR)/pf_administration.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_administration.c -o $(SRCTESTDIR)/$@

phonefirewall_search_x86.o: $(SRCDIR)/pf_search.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_search.c -o $(SRCTESTDIR)/$@

libphonefirewall_x86.so: $(SRCTESTDIR)/phonefirewall_administration_x86.o $(SRCTESTDIR)/phonefirewall_search_x86.o $(SRCDIR)/logfile_x86.o
	$(GCC) $(CLFLAGS) -shared $(SRCTESTDIR)/phonefirewall_administration_x86.o $(SRCTESTDIR)/phonefirewall_search_x86.o $(SRCDIR)/logfile_x86.o -o $(LIBDIR)/$@

logfile_x86.o: $(SRCDIR)/logfile.c $(SRCDIR)/logfile.h
	$(GCC) -fpic -c $(SRCDIR)/logfile.c -o $(SRCDIR)/$@

# XXX: Only a quick hack to find the memory corruption error.
main_test: $(SRCTESTDIR)/main_test.c $(SRCTESTDIR)/phonefirewall_administration_x86.o $(SRCTESTDIR)/phonefirewall_search_x86.o
	$(GCC) $(CLFLAGS) -L$(LIBDIR) $(TEST_LIB) $(SRCTESTDIR)/testphonefirewall_administration.o $(SRCTESTDIR)/phonefirewall_search_x86.o $(SRCTESTDIR)/main_test.c -o $(BINTESTDIR)/$@

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
