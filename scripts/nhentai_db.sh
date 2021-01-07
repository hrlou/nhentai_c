#!/bin/bash
db_format () {
    tags=$(echo "$1" | sed -e 's/.*: //' -e 's/,//g' | tr '\n' ' ' | sed -e 's/  / /g' -e 's/.$//' -e 's/  / /g')
    printf '{'"${tags}"'}'",(00),[${PWD}/$2];\n"
}

find . -name '*.cbz' -print0 | while IFS= read -r -d '' line; do 
    textfile="$(unzip -l "$line" | grep ".*.txt" | awk '{print$4}')" 
    if [ ! -z "$textfile" ]; then
        db_format "$(unzip -p "$line" "$textfile")" "$line"
    fi
done

find . -name '*.txt' -print0 | while IFS= read -r -d '' line; do 
    id=${line##*/}; id=${id//.txt}
    count=$(echo "$line" | sed "s/$id/$id\n/g" | grep -c "$id")
    if [ "$count" -ge 2 ]; then
        dir=${line///$id.txt}; dir=${dir//.\/}
        db_format "$(cat "$line")" "$dir"
    fi
done
