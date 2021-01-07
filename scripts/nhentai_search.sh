#!/bin/bash
# prototype search function
# input a string as you would into the nhentai search bar
# will be rewritten in C, proof of concept

search="$(echo ${1} | tr ' ' '+')"

page="1"
result="empty"
while [ ! -z "$result" ]; do
    stored="$(curl --silent "https://nhentai.net/search/?q=""${search}""&sort=popular&page=${page}")"
    result="$(echo "${stored}" | grep -o "/g/[0-9 ]*" | tr '/' ' ' | sed 's/ g //' | tr '\n' ' ')"
    printf "${result}"
    page="$(($page + 1))"
done
printf '\n'