#!/bin/bash
sh_root=$(cd `dirname $0`; pwd)
prefix=/usr/local/filesort
if [ ! -d $prefix ]; then
	sudo mkdir $prefix
fi
cmake -D CMAKE_INSTALL_PREFIX=$prefix $sh_root
cd $sh_root
make && sudo make install
