#!/bin/bash
sh_root=$(cd `dirname $0`; pwd)
in_file="/home/www/tangwu/test/filesort/in.txt"
out_file="/home/www/tangwu/test/filesort/out.txt"
$sh_root/build.sh 
if [ $? -eq 0 ]; then
	st_time=`date +"%s"`
	#$sh_root/bin/filesort $in_file $out_file
	$sh_root/bin/filesort $in_file $out_file
	ed_time=`date +"%s"`
	used_time=$((ed_time-st_time))
	echo "耗时:${used_time}秒"
else
	echo "编译失败"
fi
