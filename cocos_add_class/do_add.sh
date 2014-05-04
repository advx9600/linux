#!/bin/bash

pro_path=/mnt/hgfs/cocos2d-x-3.0rc2/dafeng_cocos/TVMain
pro_name=TVMain
class_name=$1

open_win_pro=${pro_path}/proj.win32/${pro_name}.vcxproj
open_win_filter=${pro_path}/proj.win32/${pro_name}.vcxproj.filters
android_mk_path=${pro_path}/proj.android/jni/Android.mk
class_path=${pro_path}/Classes


./add_file ${class_path} ${class_name}
if [ ! $? -eq 0 ];then
	exit 1
fi
./android_add ${android_mk_path} ${class_name}
./win_add ${open_win_pro} ${class_name}
./win_add_filter ${open_win_filter} ${class_name}

echo "excue success!"
