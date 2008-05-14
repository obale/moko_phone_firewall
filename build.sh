#!/bin/sh
echo -en "Starting the build process... \t\t\t\t\t\t";
. env-export;
make clean &> log/make_clean.log;
make &> log/make.log\
&& ( echo -e '[\E[32mDONE\033[0m]'; ./bin_test/testphonefirewall )\
|| ( echo -e '[\E[31mFAILED\033[0m]'; echo "For the output of the compile \
process look at log/make.log and log/make_clean.log" )
