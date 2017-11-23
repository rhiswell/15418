#!/bin/bash

VECTOR_WIDTH=( 2 4 8 16 )
default_vector_width=4

function inject_vector_width
{
    local vector_width=$1
    local target_file="./CMU418intrin.h"

    sed -i "s/#define VECTOR_WIDTH [0-9]\+/#define VECTOR_WIDTH $vector_width/g" \
        $target_file
}

function collect_stat
{
    local vector_width=$1
    local app_bin="./myexp"
    local app_opt="-s 10000"

    echo "$vector_width" >stat_$vector_width.log
    $app_bin $app_opt | grep -A5 -m 1 "*" | awk -F':[ ]*' 'NR>1 {print $2}' \
        >stat_$vector_width.log
}

function merge_stat
{
    printf "Vector Width\nTotal Vector Instructions\nVector Utilization" \
        >title.csv
    printf "\nUtilized vector lanes\nTotal Vector Lanes" \
        >>title.csv
    paste -d',' title.csv `find . -name "*.log" | sort` >merged_stat.csv && \
        sed -i "s/%//g" merged_stat.csv
    rm *.log title.csv
}

case $1 in
    "make" )
        make >/dev/null
        ;;
    "stat" )
        inject_vector_width $default_vector_width
        make >/dev/null
        collect_stat $default_vector_width
        ;;
    "stat_all" )
        for vector_width in ${VECTOR_WIDTH[@]}; do
            inject_vector_width $vector_width
            make >/dev/null
            collect_stat $vector_width
        done
        merge_stat
        ;;
    *)
        echo unknown subcommand
        ;;
esac
