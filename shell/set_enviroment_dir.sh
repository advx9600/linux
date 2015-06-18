myDirs="/home/abc/Public/tools/ffmpeg/_install
        "
setEnvs="LD_LIBRARY_PATH=lib
	C_INCLUDE_PATH=include
	CPLUS_INCLUDE_PATH=include
	PKG_CONFIG_PATH=lib/pkgconfig
	XDG_DATA_DIRS=share
	PATH=bin
	"

for i in $setEnvs
do
for j in $myDirs
do
	arr=(${i//=/ })
	valName=${arr[0]}
	valPath=$j"/"${arr[1]}
	eval k=\$$valName
	if [ "$k" = "" ];then
		export $valName=$valPath
	else
		export $valName=$k:$valPath
	fi
done
done
