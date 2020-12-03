#!/bin/bash
DB_FILE="$PWD/$(date +%s)_library.sh"
echo "#!/bin/sh" > $DB_FILE

for main in */; do
    cd "$main" >> $DB_FILE
    printf "mkdir ${main}\ncd ${main}\necho Downloading ${main}\nnhentai " >> $DB_FILE
    for doujin in */; do
        text="$(echo "${doujin///}" | sed 's/_.*//')"
        if [ -f "${doujin}${text}.txt" ]; then
            printf "$text " >> $DB_FILE
        fi
    done

    for cbz in *.cbz; do
        text="$(echo "${cbz}" | sed 's/_.*//')"
        printf "$text " >> $DB_FILE
    done

    printf "\n" >> $DB_FILE
    echo "cd ../" >> $DB_FILE
    cd "../" >> $DB_FILE
done

chmod +x $DB_FILE