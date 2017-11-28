#!/bin/bash

project_name=scan
target_root=~/Workspace/cmu15418/assignment2/$project_name/
host=k4

function init_once
{
    ssh $host "mkdir -p $target_root"
}

function build_project
{
    tar --exclude="./objs" --exclude="./deploy.sh" -cvf /tmp/$project_name.tar . && \
        scp /tmp/$project_name.tar $host:/tmp/ && \
        ssh $host "sh -c 'cd $target_root && \
               tar --no-same-owner -xvf /tmp/$project_name.tar && \
               make clean && make'"
}

case $1 in
    init )
        init_once
        ;;
    build )
        build_project
        ;;
    check_scan )
        ssh k "source ~/.bashrc && cd $target_root && make check_scan"
        ;;
    check_find_repeats )
        ssh k "source ~/.bashrc && cd $target_root && make check_find_repeats"
        ;;
    * )
        ;;
esac
