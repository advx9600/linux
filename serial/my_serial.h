#ifndef __MY_SERIAL_H__
#define __MY_SERIAL_H__

int create_my_serial(const char* dev);

void close_my_serial();

int read_my_serial(char* buf,int len);

int poll_my_serial(int polltime);

int write_my_serial(char* buf,int len);

#endif
