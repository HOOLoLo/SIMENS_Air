//
// Created by zgd on 18-4-23.
//

#ifndef CIOTFLY_SERIAL_RT_H
#define CIOTFLY_SERIAL_RT_H
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>
#include<termios.h>
#include<string.h>
#include<sys/time.h>
#include<sys/types.h>
#include<errno.h>


int uart_open(int &fd,char *pathname,int para);
int uart_open(int &fd,const char *pathname);
int uart_close(int fd);
int uart_set(int fd,int baude,int c_flow,int bits,char parity,int stop);
ssize_t safe_write(int fd, const void *vptr, size_t n);
ssize_t safe_read(int fd,void *vptr,size_t n);
int uart_write(int fd,const char *w_buf,size_t len);
int uart_read(int fd,char *r_buf,size_t len);

#endif //CIOTFLY_SERIAL_RT_H
