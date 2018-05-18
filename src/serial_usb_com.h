/*
 * serial_usb_com.h
 *
 *  Created on: Nov 26, 2017
 *      Author: root
 */

#ifndef SERIAL_USB_COM_H_
#define SERIAL_USB_COM_H_

extern int serial_fd ;
int open_serial(char *com) ;
int serial_read(char *buf,int *len) ;
int serial_write(char *buf,int len) ;
int serial_set_group(int id) ;
int serial_start() ;
int serial_close() ;
#endif /* SERIAL_USB_COM_H_ */
