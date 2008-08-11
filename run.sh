#!/bin/bash
echo -en "Starting the build process... \t\t\t\t\t\t";
. env-export;
make clean &> log/make_clean.log;
make &> log/make.log\
&& ( echo -e '[\E[32mDONE\033[0m]'; )\
|| ( echo -e '[\E[31mFAILED\033[0m]'; echo "For the output of the compile \
process look at log/make.log and log/make_clean.log" );

echo ""

select CHOICE in CLOSE CUnit CUnit+Valgrind
do
        case "$CHOICE" in
                "CLOSE")
                exit
                ;;
                "CUnit")
                ./bin_test/testphonefirewall;
                exit
                ;;
                "CUnit+Valgrind")
                valgrind -v --leak-check=full ./bin_test/testphonefirewall
                exit
                ;;
        esac
done
