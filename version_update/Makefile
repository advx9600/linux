extra_flag:=`pkg-config python-2.7  --cflags --libs` -Wall
python_flags:= "CFLAGS=-D MAKE_FOR_LIB=true"
main:
	gcc *.c -o version_update  $(extra_flag) -D MAKE_FOR_CC=1

python_lib:
	gcc *.c -o versionupdate.so  $(extra_flag) -fPIC  -shared -D MAKE_FOR_PYTHON=1

