#!/bin/bash
for doujin in */; do
    text="$(echo "${doujin///}" | sed 's/_.*//')"
    if [ -f "${doujin}${text}.txt" ]; then
        zip -r "${doujin///}.cbz" "${doujin}" && rm -r "${doujin}"
    fi
done