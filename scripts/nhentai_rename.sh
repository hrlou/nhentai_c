#!/bin/bash
printf "0: Remove\n1: Title\n2: Gallery-Id\n3: Parodies\n4: Charecters\n5: Tags\n6: Artists\n7: Groups\n8: Language\n9: Categories\n10: Pages\n\n"
read -p "Pick one: " number

for i in */; do
	id=$(echo "${i}" | sed 's/_.*//')
	if [ $number == 0 ]; then
		mv "${i}" "${id///}"
	else 
		filtered=$(cat "${i}"*.txt | sed 's/.*: //' | sed "${number}"'q;d' | sed 's/, /_/g' | sed s'/.$//')
		newdir="${id///}_${filtered}"

		if [ ! -z "${filtered}" ]; then
			echo "${newdir}"
			mv "${i}" "${newdir}"
		fi
	fi
done
