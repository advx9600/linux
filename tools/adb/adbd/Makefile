SRCS+= adb.c \
        fdevent.c \
        usb_linux_client.c \
        adb_auth_client.c \
        transport_local.c \
        transport_usb.c \
        transport.c \
        sockets.c \
        services.c \
        framebuffer_service.c \
        remount_service.c \
        jdwp_service.c \
        file_sync_service.c \
        log_service.c

SRCS+=base64.c # download from internet
DIR="libcutils/"
SRCS+= $(DIR)socket_inaddr_any_server.c
SRCS+= $(DIR)socket_local_client.c
SRCS+= $(DIR)socket_local_server.c
SRCS+= $(DIR)socket_loopback_client.c
SRCS+= $(DIR)socket_loopback_server.c
SRCS+= $(DIR)socket_network_client.c
SRCS+= $(DIR)list.c
SRCS+= $(DIR)load_file.c

CPPFLAGS+= -DADB_HOST=0
CPPFLAGS+= -DHAVE_FORKEXEC=1
CPPFLAGS+= -DHAVE_SYMLINKS
CPPFLAGS+= -DHAVE_TERMIO_H
CPPFLAGS+= -D_GNU_SOURCE
CPPFLAGS+= -D_XOPEN_SOURCE
CPPFLAGS+= -DALLOW_ADBD_ROOT=1
CPPFLAGS+= -I.
CPPFLAGS+= -I../include
CPPFLAGS+= -I../../../external/zlib
CPPFLAGS+= -I../openssl/openssl-1.0.2a/_install/include

CFLAGS+= -O2 -g -Wall -Wno-unused-parameter
#LDFLAGS= -static
LIBS= -lrt -lpthread
LIBS+= -L../openssl/openssl-1.0.2a/_install/lib -lssl -lcrypto  -ldl

TOOLCHAIN= /home/abc/Public/compiler/toolschain/4.4.3/bin/arm-none-linux-gnueabi-

CC= $(TOOLCHAIN)gcc
LD= $(TOOLCHAIN)gcc

OBJS= $(SRCS:.c=.o)

TARGET=adbd
main:
	$(CC) $(SRCS) -o $(TARGET) $(LDFLAGS) $(LIBS) $(CPPFLAGS)

#all: adb

#adb: $(OBJS)
#	$(LD) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	rm -rf $(OBJS) a.out $(TARGET)
