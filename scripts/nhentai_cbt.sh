#!/bin/bash
find . -name '*.txt' -print0 | while IFS= read -r -d '' line; do 
    id=${line##*/}; id=${id//.txt}
    count=$(echo "$line" | sed "s/$id/$id\n/g" | grep -c "$id")
    if [ "$count" -ge 2 ]; then
        tar -cf "$(echo "$line" | sed "s/\/$id.txt/.cbt/")" "$(echo "$line" | sed "s/\/$id.txt//")" && rm -r "$(echo "$line" | sed "s/\/$id.txt//")"
    fi
done
