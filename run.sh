#!/bin/bash
sh_root=$(cd `dirname $0`; pwd)
$sh_root/build.sh 
in_file="/home/www/tangwu/filesort/in.txt"
out_file="/home/www/tangwu/filesort/out.txt"
if [ $? -eq 0 ]; then
	st_time=`date +"%s"`
	$sh_root/bin/filesort $in_file $out_file
	ed_time=`date +"%s"`
	used_time=$((ed_time-st_time))
	echo "耗时:${used_time}秒"
else
	echo "编译失败"
fi
