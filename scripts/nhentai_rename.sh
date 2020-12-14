#!/usr/bin/env bash
if [ -z "${1}" ]; then
    printf "0: Remove\n1: Title\n2: Gallery-Id\n3: Parodies\n4: Charecters\n5: Tags\n6: Artists\n7: Groups\n8: Language\n9: Categories\n10: Pages\n\n"
    read -p "Pick one: " number
else
    number="${1}"
fi

for i in */; do
    id=$(echo "${i}" | sed 's/_.*//')
    if [ $number == 0 ]; then
        mv "${i}" "${id///}"
    elif [ -f "${i}${id///}.txt" ]; then
        filtered=$(cat "${i}""${id///}".txt | sed -e 's/.*: //' -e "${number}"'q;d' -e 's/ //g' | tr ',' '_')
        filtered=${filtered// }
        if [ "${filtered: -1}" == "_" ]; then
            echo "test"
            filtered="${filtered: : -1}"
        fi
        newdir="${id///}_${filtered}"
        if [[ "${i///}" != "${newdir}" ]] && [ ! -z "${filtered}" ]; then
            echo "${newdir}"
            mv "${i}" "${newdir}"
        fi
    fi
done
