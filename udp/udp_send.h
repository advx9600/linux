#ifndef __UDP_SEND_H__
#define __UDP_SEND_H__

int create_udp_socket(int port);

int read_udp_data(char* data,int len);

int send_udp_data(const char* data ,int len,const char* ip,int port);

void close_udp_socket();

#endif
