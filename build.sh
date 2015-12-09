#!/bin/bash

rootDir=`pwd`

rm build.2 2>/dev/null

MODULES=(common trad maimutza cupid watchdog)
MODULE_FILES=(libcommon.a trad maimutza cupid watchdog)
OK=1
for ((i=0;i<${#MODULES[@]};i++)); do
	module="${MODULES[i]}";
	moduleFile="${MODULE_FILES[i]}";
	printf "\n\nBuilding \"$module\"...--------------------------------------------"
	buildDir=$rootDir/$module/Release;
	if [ ! -d $buildDir ]; then
		printf "EROARE: nu exista directorul $buildDir"
		OK=0
		break
	fi
	cd $buildDir
	make -s clean 2>>$rootDir/build.2
	make -s -j4 all 2>>$rootDir/build.2
	if [ ! -f $buildDir/$moduleFile ]; then
		printf "EROARE build modul $module. STOP."
		OK=0
		break
	fi
	printf "$module output: $buildDir/$moduleFile		[OK]"
done

cd $rootDir

if [ "$OK" -eq "1" ]; then
	printf "\n\nDone. Enjoy! :-)  [maimutza prietenoasa]\n\n"
	rm build.2 2>/dev/null
	rm build.err 2>/dev/null
else
	if [ -f build.2 ]; then
		cat build.2 | grep -i -n3 error > build.err
		rm build.2
	fi
	printf "\n\nAu aparut erori. Vezi build.err\n\n"
fi

