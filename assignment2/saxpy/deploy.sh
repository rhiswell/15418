#!/bin/bash

project_name=saxpy
app_bin=./cudaSaxpy
target_root=~/Workspace/cmu15418/assignment2/$project_name/

function init_once
{
    ssh k "mkdir -p $target_root"
}

function build_ispc_project
{
    local project_name=prog5_saxpy
    local app_bin=./saxpy
    local target_root=~/Workspace/cmu15418/assignment1/$project_name/
    local last_dir=`pwd`
    # ISPC-based SAXPY
    cd ../../assignment1/prog5_saxpy && tar --exclude="./objs" --exclude="*.png" \
        --exclude="saxpy" -cvf /tmp/$project_name.tar . && \
        scp /tmp/$project_name.tar k:/tmp/ && \
        ssh k "mkdir -p $target_root && cd $target_root && \
               tar --no-same-owner -xvf /tmp/$project_name.tar && \
               make clean && make"
    cd $last_dir
}

function build_project
{
    tar --exclude="./objs" --exclude="./deploy.sh" -cvf /tmp/$project_name.tar . && \
        scp /tmp/$project_name.tar k:/tmp/ && \
        ssh k "cd $target_root && \
               tar --no-same-owner -xvf /tmp/$project_name.tar && \
               make clean && make"
}

function build_all
{
    build_project
    build_ispc_project
}

function run_test
{
    ssh k "cd $target_root && $app_bin"
}

function run_stat
{
    # CUDA-based SAXPY
    run_test
    # ISPC-based SAXPY
    local target_root=~/Workspace/cmu15418/assignment1/prog5_saxpy
    ssh k "cd $target_root && ./saxpy"
}

case $1 in
    init )
        init_once
        ;;
    build )
        build_project
        ;;
    build_all )
        build_all_project
        ;;
    run_test )
        run_test
        ;;
    stat )
        run_stat
        ;;
    * )
        ;;
esac
