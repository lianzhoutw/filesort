#!/bin/bash
sh_root=$(cd `dirname $0`; pwd)
cmake $sh_root
cd $sh_root
make 
