# run.sh
# 
# (C) 2008 by Networld Consulting, Ltd. 
# Written by Alex Oberhauser <oberhauseralex@networld.to> 
# All Rights Reserved 
# 
# This program is free software: you can redistribute it and/or modify 
# it under the terms of the GNU General Public License as published by 
# the Free Software Foundation, version 2 of the License.
#  
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#  
# You should have received a copy of the GNU General Public License
# along with phone-firewall.  If not, see <http://www.gnu.org/licenses/>.
#  
#!/bin/sh

show_gpl() {
  echo "This program is free software: you can redistribute it and/or modify"
  echo "it under the terms of the GNU General Public License as published by"
  echo "the Free Software Foundation, version 2 of the License."
  echo ""
  echo "This program is distributed in the hope that it will be useful,"
  echo "but WITHOUT ANY WARRANTY; without even the implied warranty of"
  echo "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the"
  echo "GNU General Public License for more details."
  echo ""
  echo "You should have received a copy of the GNU General Public License"
  echo "long with Foobar.  If not, see <http://www.gnu.org/licenses/>."
}

if [[ $1 = "licence" ]];
	then show_gpl;
else 
	echo "USAGE: $0 [licence]";
        echo -en "Starting the build process... \t\t\t\t\t\t";
        . env-export;
        make clean &> log/make_clean.log;
        make &> log/make.log\
        && ( echo -e '[\E[32mDONE\033[0m]'; )\
        || ( echo -e '[\E[31mFAILED\033[0m]'; echo "For the output of the compile \
        process look at log/make.log and log/make_clean.log" );

        echo ""

        select CHOICE in CLOSE CUnit CUnit+Valgrind+verbose CUnit+Valgrind
        do
                case "$CHOICE" in
                        "CLOSE")
                        exit
                        ;;
                        "CUnit")
                        ./bin_test/testphonefirewall;
                        exit
                        ;;
                        "CUnit+Valgrind+verbose")
                        valgrind -v --leak-check=full ./bin_test/testphonefirewall
                        exit
                        ;;
                        "CUnit+Valgrind")
                        valgrind ./bin_test/testphonefirewall
                        exit
                        ;;
                esac
        done
fi
