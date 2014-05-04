export PKG_CONFIG_PATH=/home/abc/Public/tools/libxml2/libxml2-2.7.2/install_dir/lib/pkgconfig

LIBS=`pkg-config --libs   libxml-2.0`
FLAGS=`pkg-config --cflags   libxml-2.0`
main:
	gcc add_class_filter.c $(LIBS) $(FLAGS) -o win_add_filter
	gcc add_class.c $(LIBS) $(FLAGS) -o win_add
	gcc add_class_file.c -o add_file
	gcc add_class_android.c -o android_add

