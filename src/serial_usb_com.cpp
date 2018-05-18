/*
 * serial_usb_com.c
 *
 *  Created on: Nov 26, 2017
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include "serial_usb_com.h"



void setTermios(struct termios *pNewtio, unsigned short uBaudRate)
{
      bzero(pNewtio,sizeof(struct termios));
      pNewtio->c_cflag = uBaudRate|CS8|CREAD|CLOCAL;
      pNewtio->c_iflag = IGNPAR;
      pNewtio->c_oflag = 0;
      pNewtio->c_lflag = 0;
      pNewtio->c_cc[VINTR] = 0;
      pNewtio->c_cc[VQUIT] = 0;
      pNewtio->c_cc[VERASE] = 0;
      pNewtio->c_cc[VKILL] = 0;
      pNewtio->c_cc[VEOF] = 4;
      pNewtio->c_cc[VTIME] = 5;
      pNewtio->c_cc[VMIN] = 0;
      pNewtio->c_cc[VSWTC] = 0;
      pNewtio->c_cc[VSTART] = 0;
      pNewtio->c_cc[VSTOP] = 0;
      pNewtio->c_cc[VSUSP] = 0;
      pNewtio->c_cc[VEOL] = 0;
      pNewtio->c_cc[VREPRINT] = 0;
      pNewtio->c_cc[VDISCARD] = 0;
      pNewtio->c_cc[VWERASE] = 0;
      pNewtio->c_cc[VLNEXT] =  0;
      pNewtio->c_cc[VEOL2] = 0;
}
int open_serial(char *com) {
	printf("%s\n",com) ;

    struct termios newtio;

   // printf("Message : %s\n", strerror(errno));
   if((serial_fd=open(com,O_RDWR|O_NOCTTY|O_NDELAY))<0) //open serial
    {
        printf("Message : %s\n", strerror(errno));
        printf("%d\n",serial_fd);
           printf("Can't open serial port!\n");
           return -1;
    }
    if(fcntl(serial_fd, F_SETFL, 0) < 0)
    {
        printf("fcntl failed!\n");
        return -1;
    }

  //  printf("11111");

 //   tcgetattr(fd,&oldtio);
    printf("%d\n",serial_fd);
     setTermios(&newtio,B921600);
     tcflush(serial_fd,TCIFLUSH);
     tcsetattr(serial_fd,TCSANOW,&newtio);
     return 1 ;
}

int serial_read(char *buf,int *len) {
    int lens,fs_sel;
    fd_set fs_read;
    struct timeval time;
    FD_ZERO(&fs_read);
    FD_SET(serial_fd,&fs_read);
    time.tv_sec = 10;
    time.tv_usec = 0;
    //使用select实现串口的多路通信
    fs_sel = select(serial_fd+1,&fs_read,NULL,NULL,&time);
    if(fs_sel)
    {
         lens = read(serial_fd,buf,*len);
         *len = lens ;
         return *len;
     }else{
    	 *len = 0 ;
          return -1;
      }
	return 1 ;
}
int serial_write(char *buf,int len) {

	//printf("%s\n",buf) ;
    int i=0 ,nCount=0;
    char mid[2]={'\n','\n'} ;
    write(serial_fd,mid,1) ;
    usleep(1000) ;
    write(serial_fd,mid,1) ;
    usleep(1000*5) ;
	for(i=0;i<len;i++)
    {
//		printf("i %d fd %d %02x\n",i,serial_fd,buf[i]) ;
		mid[0] = buf[i] ;
        nCount = write(serial_fd,mid,1);
//       printf("send data OK! fd =%d,count=%d\n",serial_fd,nCount);
        if(nCount<0){
        	return -1 ;
        }
        usleep(6000);
     }
	return 1 ;
}
int serial_close() {
	close(serial_fd);
	return 1 ;
}
int serial_set_group(int id) {
//	serial_write("\n\n",2) ;
//	usleep(10000) ;
	char *stop="daps\n" ;
	serial_write(stop,strlen(stop)) ;
	usleep(5000) ;
	char buf[40]={0};//"dset g01 32\n" ;
	sprintf(buf,"dset g01 %d\n",id) ;
	serial_write(buf,strlen(buf)) ;
	usleep(90000) ;
	char *xx="daps 3\n" ;
	serial_write(xx,strlen(xx)) ;
	usleep(5000) ;
	return 1 ;
}
int serial_start() {
	char *xx="daps 3\n" ;
	serial_write(xx,strlen(xx)) ;
	usleep(5000) ;
	return 1 ;
}
