#!/bin/bash

project_name=render
target_root=~/Workspace/cmu15418/assignment2/$project_name/
host=k4
nvcc_path=/usr/local/cuda-8.0/bin

function init_once
{
    ssh $host "mkdir -p $target_root"
}

function build_project
{
    tar --exclude="./objs" --exclude="./deploy.sh" -cvf /tmp/$project_name.tar . && \
        scp /tmp/$project_name.tar $host:/tmp/ && \
        ssh $host "export PATH=$nvcc_path:\$PATH && cd $target_root && \
               tar --no-same-owner -xvf /tmp/$project_name.tar && \
               make clean && make"
}

case $1 in
    init )
        init_once
        ;;
    build )
        build_project
        ;;
    run )
        ssh -X $host "cd $target_root && ./render rgb"
        ;;
    * )
        ;;
esac
