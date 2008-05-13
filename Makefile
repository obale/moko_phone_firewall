CCMOKO = /usr/local/openmoko/arm/bin/arm-angstrom-linux-gnueabi-gcc
CCMOKOLD = /usr/local/openmoko/arm/bin/arm-angstrom-linux-gnueabi-ld
GCC = /usr/bin/gcc
RM = /bin/rm
CLFLAGS += --std=c99 -Wall
TEST_LIB = -lcunit -ltestphonefirewall
DOXYGEN = /usr/bin/doxygen
SRCDIR = src
BINDIR = bin
LIBDIR = lib
SRCTESTDIR = src_test
BINTESTDIR = bin_test


all: 	libphonefirewall.so\
	test

.PHONY: doc
doc: 
	@ echo -en 'Creating a documentation with doxygen \t\t\t\t\t\t'; 
	@ $(DOXYGEN) &> /dev/null\
	 && ( echo -e '[\E[32mDONE\033[0m]'; tput sgr0 )\
	 || ( echo -e '[\E[31mFAILED\033[0m]'; tput sgr0 )

phonefirewall_administration.o: $(SRCDIR)/phonefirewall_administration.c $(SRCDIR)/libphonefirewall.h
	$(CCMOKO) $(CLFLAGS) -fpic -c $(SRCDIR)/phonefirewall_administration.c -o $(SRCDIR)/$@

libphonefirewall.so: $(SRCDIR)/phonefirewall_administration.o 
	$(CCMOKOLD) -shared $(SRCDIR)/phonefirewall_administration.o -o $(LIBDIR)/$@

# 
# Begining of the testing part.
#
test: testphonefirewall_administration.o libtestphonefirewall.so testphonefirewall

testphonefirewall: $(SRCTESTDIR)/testphonefirewall.c 
	$(GCC) $(CLFLAGS) $(SRCTESTDIR)/testphonefirewall.c -L$(LIBDIR) $(TEST_LIB) -o $(BINTESTDIR)/$@

testphonefirewall_administration.o: $(SRCDIR)/phonefirewall_administration.c $(SRCDIR)/libphonefirewall.h
	$(GCC) $(CLFLAGS) -fPIC -c $(SRCDIR)/phonefirewall_administration.c -o $(SRCTESTDIR)/$@

libtestphonefirewall.so: $(SRCTESTDIR)/testphonefirewall_administration.o
	$(GCC) $(CLFLAGS) -shared $(SRCTESTDIR)/testphonefirewall_administration.o -o $(LIBDIR)/$@


.PHONY: clean
clean: 
	$(RM) 	bin/*\
	       	lib/*\
	       	src/*.o\
		src_test/*.o\
		bin_test/*\
		db/* 
