#!/bin/bash

case $1 in
    "make" )
        make >/dev/null
        ;;
    "stat" )
        ;;
    "stat_all" )
        ;;
    * )
        echo unknown subcommand
        ;;
esac
