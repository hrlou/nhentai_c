#!/bin/bash
db_format () {
    tags=$(echo "$1" | sed -e 's/.*: //' -e 's/,//g' | tr '\n' ' ' | sed -e 's/  / /g' -e 's/.$//' -e 's/  / /g')
    # printf "$tags_spaced\n"
    echo '{'"${tags}"'}'",(00),[${PWD}/$dir];"
}

find . -name '*.cbz' -print0 | while IFS= read -r -d '' line; do 
    id=${line##*/}; id=${id//_*}
    text="$(unzip -l "$line" | grep "$id.txt" | awk '{print$4}')"
    # if [ ! -z "$(unzip -l "$line" | grep -o "$id.txt")" ]; then
    if [ ! -z "$text" ]; then
        dir=${line##*/}
        # db_format "$(unzip -p "$line" "${dir//.cbz}/$id.txt")" "$dir"
        db_format "$(unzip -p "$line" "$text")" "$dir"
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
