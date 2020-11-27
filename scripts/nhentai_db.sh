#!/bin/bash

for pdir in */; do
    cd "${pdir}"
    for cdir in */; do
        text="$(echo "${cdir///}" | sed 's/_.*//')"
        textfile="${cdir}${text}.txt" 
        if [ -f "$textfile" ]; then
            echo '{'"$(cat "$textfile" | sed -e 's/.*: //' -e 's/,//g' | tr '\n' ' ' | sed -e 's/  / /g' -e 's/.$//' -e 's/  / /g')"'}'",(00),[${PWD}/${cdir///}];"
        fi
    done	    
    cd "../"
done
