#!/bin/bash
#
# File : clean.sh
#
# init created: 2016-05-27
# last updated: 2016-05-27
#
########################################################################
_file=$(readlink -f $0)
_cdir=$(dirname $_file)
_name=$(basename $_file)
SRC_DIR=${_cdir}/ecs

echo "**** clean temp files (*.pyc)"
find ${SRC_DIR} -name *.pyc | xargs rm -f

echo "**** clean temp files (*.log)"
find ${SRC_DIR} -name *.log | xargs rm -f

echo "**** clean ok."