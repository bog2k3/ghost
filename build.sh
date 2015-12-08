#!/bin/bash

echo Building \"common\"...
cd common/Release
make clean
make -j4 all
echo Building \"trad\"...
cd ../../trad/Release
make clean 
make -j4 all
echo Building \"maimutza\"...
cd ../../maimutza/Release
make clean
make -j4 all
echo Building \"cupid\"...
cd ../../cupid/Release
make clean
make -j4 all
echo Building \"watchdog\"...
cd ../../watchdog/Release
make clean
make -j4 all

echo "Done. Enjoy! :-)  [maimutza prietenoasa]"

