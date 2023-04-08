#!/usr/bin/env bash


for line in  $(grep -P '^[a-z-_A-Z]+:' -o c.y); do
    echo "%nterm <ast_node> ${line%:}"
done

