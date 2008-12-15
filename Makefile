GCC = /usr/bin/gcc
RM = /bin/rm
STRIP = /usr/local/openmoko/arm/bin/arm-angstrom-linux-gnueabi-strip
CLFLAGS += --std=c99 -Wall -Werror #-DDEBUG -g -o -O0
TEST_LIB = -lcunit -lphonefirewall_x86 -lsqlite3
DOXYGEN = /usr/bin/doxygen
SRCDIR = src
BINDIR = bin
LIBDIR = lib
SRCTESTDIR = src_test
BINTESTDIR = bin_test

test: phonefirewall_administration_x86.o phonefirewall_search_x86.o libphonefirewall_x86.so phonefirewall_x86

all: 	libphonefirewall.so\
	prepare\
	test

.PHONY: doc
doc: 
	@ echo -en 'Creating a documentation with doxygen \t\t\t\t\t\t'; 
	@ $(DOXYGEN) &> /dev/null\
	 && ( echo -e '[\E[32mDONE\033[0m]'; tput sgr0 )\
	 || ( echo -e '[\E[31mFAILED\033[0m]'; tput sgr0 )

pf_administration.o: $(SRCDIR)/pf_administration.c $(SRCDIR)/libphonefirewall.h
	$(CC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_administration.c -o $(SRCDIR)/$@

pf_search.o: $(SRCDIR)/pf_search.c $(SRCDIR)/libphonefirewall.h
	$(CC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_search.c -o $(SRCDIR)/$@

libphonefirewall.so: $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o
	$(CC) -shared $(SRCDIR)/pf_administration.o $(SRCDIR)/pf_search.o -o $(LIBDIR)/$@

.PHONY: prepare
prepare:
	$(STRIP) $(LIBDIR)/libphonefirewall.so	

# 
# Begining of the testing part.
#

phonefirewall_x86: $(SRCTESTDIR)/testphonefirewall.c 
	$(GCC) $(CLFLAGS) $(SRCTESTDIR)/testphonefirewall.c -L$(LIBDIR) $(TEST_LIB) -o $(BINTESTDIR)/$@

phonefirewall_administration_x86.o: $(SRCDIR)/pf_administration.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_administration.c -o $(SRCTESTDIR)/$@

phonefirewall_search_x86.o: $(SRCDIR)/pf_search.c $(SRCDIR)/libphonefirewall.h 
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/pf_search.c -o $(SRCTESTDIR)/$@

libphonefirewall_x86.so: $(SRCTESTDIR)/phonefirewall_administration_x86.o $(SRCTESTDIR)/phonefirewall_search_x86.o
	$(GCC) $(CLFLAGS) -shared $(SRCTESTDIR)/phonefirewall_administration_x86.o $(SRCTESTDIR)/phonefirewall_search_x86.o -o $(LIBDIR)/$@

.PHONY: clean
clean: 
	$(RM) -f $(LIBDIR)/*\
	       	 $(SRCDIR)/*.o\
		 $(SRCTESTDIR)/*.o\
		 $(BINTESTDIR)/*\
		 db/*\
		 log/moksec.log

.PHONY: clean-db
clean-db:
	$(RM) -f db/*
