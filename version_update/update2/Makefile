INCLUDE=-I/home/abc/Public/git/linux/
export PKG_CONFIG_PATH=/home/abc/Public/tools/mxml/mxml-2.9/install_dir/lib/pkgconfig
CFLAGS= `pkg-config --cflags mxml`
LIBS= `pkg-config --libs mxml`
TARGET="version_update2"
.PHONY:test main

main:
	gcc update_main.c $(INCLUDE) $(CFLAGS) $(LIBS) -o $(TARGET)
	rm my_dir -rf
	./$(TARGET) smdkv210 1.0.0 uboot

