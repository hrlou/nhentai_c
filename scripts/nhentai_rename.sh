#!/usr/bin/env bash
if [ -z "${1}" ]; then
    printf "0: Remove\n1: Title\n2: Gallery-Id\n3: Parodies\n4: Charecters\n5: Tags\n6: Artists\n7: Groups\n8: Language\n9: Categories\n10: Pages\n\n"
    read -p "Pick one: " number
else
    number="${1}"
fi

rename () {
    if [ "${number}" -ge "1" ]; then
        filtered=$(echo "$1" | sed -e 's/.*: //' -e "${number}"'q;d' -e 's/ //g' | tr ',' '_' | sed 's/\&\#.*;//g')
        filtered="_${filtered// }"
        [ "${filtered: -1}" == "_" ] && filtered="${filtered: : -1}"
    fi

    dir=$(dirname "${2}")

    [ "${2: -4}" == ".cbz" ] && original="${2}" && newdir="${dir}/${3}${filtered}.cbz"
    [ "${2: -4}" == ".txt" ] && original="$(dirname ${2})" && newdir="$(dirname ${dir})/${3}${filtered}"

    [ "${newdir}" != "${original}" ] && echo "mv ${original}" "${newdir}" && mv "${original}" "${newdir}"
}

find . -name '*.txt' -print0 | while IFS= read -r -d '' line; do 
    id=${line##*/}; id=${id//.txt}
    count=$(echo "$line" | sed "s/$id/$id\n/g" | grep -c "$id")
    [ "$count" -ge 2 ] && rename "$(cat $line)" "$line" "$id"
done

find . -name '*.cbz' -print0 | while IFS= read -r -d '' line; do 
    textfile="$(unzip -l "$line" | grep ".*.txt" | awk '{print$4}')" 
    id="$(basename $textfile)"; id=${id//.txt}
    [ ! -z "$textfile" ] && rename "$(unzip -p "$line" "$textfile")" "$line" "$id"
done
