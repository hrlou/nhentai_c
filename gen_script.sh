#!/bin/bash
DB_FILE="$PWD/$(date +%s)_library.sh"
#DB_FILE="$PWD/test_library.sh"
echo "#!/bin/sh" > $DB_FILE

for main in */; do
	cd $main
	echo "mkdir ${main} && cd ${main}" >> $DB_FILE
	subs=$(ls | tr ' ' '\n' | sed 's/_.*//' | tr '\n' ' ')
	echo "echo Downloading ${main}" >> $DB_FILE
	echo "nhentai ${subs} && cd .." >> $DB_FILE
	cd ..
done