#!/bin/bash
db_format () {
    tags=$(echo "$1" | sed -e 's/.*: //' -e 's/,//g' | tr '\n' ' ' | sed -e 's/  / /g' -e 's/.$//' -e 's/  / /g')
    tags_seperated=$(echo "${tags#* }" | sed 's/ / \n/g')
    # count="$(echo "$tags_seperated" | wc -l)"
    while IFS= read -r line; do
        # count=$(($count - 1))
        if [ ! -z "$(echo "$line" | grep -o '-')" ]; then
            # printf "$line" | tr '-' ' '
            tags_spaced="${tags_spaced} $(printf $line | tr '-' ' ')"
        fi
    done <<< "$tags_seperated"
    # printf "$tags_spaced\n"
    printf '{'"${tags}${tags_spaced}"'}'",(00),[${PWD}/$dir];\n"
}

find . -name '*.cbz' -print0 | while IFS= read -r -d '' line; do 
    id=${line##*/}; id=${id//_*}
    if [ ! -z "$(unzip -l "$line" | grep -o "$id.txt")" ]; then
        dir=${line##*/}
        db_format "$(unzip -p "$line" "${dir//.cbz}/$id.txt")" "$dir"
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
