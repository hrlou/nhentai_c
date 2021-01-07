#!/bin/bash
DB_FILE="$PWD/$(basename $(pwd))_library.sh"
echo "#!/bin/sh" > $DB_FILE
echo 'WORKING="$(pwd)"' >> $DB_FILE

find . -type d -print0 | while IFS= read -r -d '' dir; do
    if [ ! -z "$(find "${dir}" -maxdepth 1 -name *.cbz)" ]; then
        printf "mkdir -p ${dir}\ncd ${dir}\necho Downloading ${dir}\nnhentai " >> $DB_FILE
        find "${dir}" -maxdepth 1 -name '*.cbz' -print0 | while IFS= read -r -d '' cbz; do
            printf "$(basename "$(unzip -l "$cbz" | grep ".*.txt" | awk '{print$4}' | sed 's/.txt//')") " >> $DB_FILE
        done
        printf "\n" >> $DB_FILE
        echo 'cd $WORKING' >> $DB_FILE
    fi
done


chmod +x $DB_FILE
