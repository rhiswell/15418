#!/bin/bash

function collect_speedup
{
    local app_bin=./mandelbrot

    for view_idx in {1..2}; do
        echo "nr_threads, speedup" > speedup_view$view_idx.csv
        for nr_threads in {1..32}; do
            echo -n "$nr_threads, " >>speedup_view$view_idx.csv
            $app_bin -v $view_idx -t $nr_threads | \
                grep -oE "[0-9]+.[0-9]+x" | sed "s/\(.*\)x/\1/g" \
                >>speedup_view$view_idx.csv
        done
    done
}

case $1 in 
    "make" )
        make >/dev/null
        ;;
    "speedup" )
        collect_speedup
        ;;
    *)
        echo unknown subcommand
        ;;
esac
