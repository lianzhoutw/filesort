#!/bin/bash
sh_root=$(cd `dirname $0`; pwd)
in_file="/home/www/tangwu/test/filesort/in.txt"
out_file="/home/www/tangwu/test/filesort/out.txt"
$sh_root/build.sh 
if [ $? -eq 0 ]; then
	st_time=`date +"%s"`
	#Generate test content
	#$sh_root/bin/filesort -cgen_test -t20 $in_file
	#Sort big file
	$sh_root/bin/filesort -s 100 -o $out_file $in_file
	ed_time=`date +"%s"`
	used_time=$((ed_time-st_time))
	echo "耗时:${used_time}秒"
else
	echo "编译失败"
fi
