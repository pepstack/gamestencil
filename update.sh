#!/bin/bash
# @file: update.sh
#
#   update all source file time and version.
#
# @author: zhangliang@ztgame
#
# @create: 2018-05-18
# @update: 2018-05-24
#
#######################################################################
# will cause error on macosx
_file=$(readlink -f $0)

_cdir=$(dirname $_file)
_name=$(basename $_file)

#######################################################################

$_cdir/pysrc/updt.py \
    --path=$_cdir/ecs \
    --filter="c,cpp" \
    --author="zhangliang@ztgame.com" \
    --recursive

$_cdir/pysrc/updt.py \
    --path=$_cdir/Classes \
    --filter="c,cpp" \
    --author="zhangliang@ztgame.com" \
    --recursive
    
$_cdir/pysrc/updt.py \
    --path=$_cdir/common \
    --filter="c,cpp" \
    --author="zhangliang@ztgame.com" \
    --recursive
