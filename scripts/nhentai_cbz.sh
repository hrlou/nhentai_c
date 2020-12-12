#!/bin/bash
find . -name '*.txt' -print0 | while IFS= read -r -d '' line; do 
    # echo "$line"
    id=${line##*/}
    id=${id//.txt}
    count=$(echo "$line" | sed "s/$id/$id\n/g" | grep -c "$id")
    if [ "$count" -ge 2 ]; then
        zip -r "$(echo "$line" | sed "s/\/$id.txt/.cbz/")" "$(echo "$line" | sed "s/\/$id.txt//")" && rm -r "$(echo "$line" | sed "s/\/$id.txt//")"
    fi
done
