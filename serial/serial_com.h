#ifndef __SERIAL_COM_H_
#define __SERIAL_COM_H_

 int pollRead(int sd, int msec);

 int openSerial(const char* dev, int baudRate);

 void closeSerial(int fd);

 int writeData(int fd_write, const char* bts,
                int leng);
 
 int readData(int fd_read,char*bts,
                int leng, int timeOut);
#endif
