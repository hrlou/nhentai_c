#!/bin/bash
DB_FILE="$PWD/$(date +%s)_library.sh"
echo "#!/bin/sh" > $DB_FILE

for main in */; do
	cd $main
	echo "mkdir ${main}" >> $DB_FILE
    echo "cd ${main}" >> $DB_FILE
	subs=$(ls | tr ' ' '\n' | sed 's/_.*//' | tr '\n' ' ')
	echo "echo Downloading ${main}" >> $DB_FILE
	echo "nhentai ${subs}" >> $DB_FILE
    echo "cd .." >> $DB_FILE
	cd ..
done

chmod +x $DB_FILE