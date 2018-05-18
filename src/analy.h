/*
 * analy.h
 *
 *  Created on: Oct 17, 2017
 *      Author: root
 */
/*
 * jiexishuju
 */
#ifndef ANALY_H_
#define ANALY_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stddef.h>
#define ERR_SUCCESS     0   // 成功
#define ERR_INPUTA      1   // 输入数组错误
#define ERR_TRANSFORM   2   // 转置失败
#define ERR_FAIL        3   // 失败
#define LOCATION_LIB "./ini/location.so"
extern int (*get_position)(float(*inputA)[4], float(*outputA), int n, int c);
int init_position() ;
#define NUM 20
struct _POINT_{
	int id ;
	float x ;
	float y ;
	float z ;
	int xx ;
	int yy ;
	int zz ;
	char mac[10] ;
};
extern struct _POINT_ gpoint[20] ;
extern int point_num_all ;
extern int point_num_current ;
extern int one_point_num  ;
extern float inputData[20][4] ;
extern int  one_inputData[20][4];
extern float output[5] ;
extern int one_output[3] ;
extern void *location_lib ;

extern int maxX ;
extern int maxY ;
extern int maxZ ;

extern char pt_ip[20] ;
extern int pt_port ;

extern short last_point[10][3] ;
extern short avg_point[3] ;
extern int last_num ;
extern int now_num ;
extern int maxi[2] ;
extern int mini[2] ;
extern short ax;
extern short ay;
extern short az ;
extern short avg_num[10] ;

extern int front_back /*0,1,x+,2,x-,3,y+,4,y-*/ ;

/*
 * f1,1,0 192.168.1.105	90:c6:82:c6:13:62
 */
int get_info(char *buf,int len,char *mac,int *bx,int *cy);
int get_mac_x(char *st,int type/*f1,0,1*/,char *mac,int *bx,int *cy);

int set_info(int type,char *mac,float bx,int *out) ;
int read_file_info() ;
int get_postionxyz(float input[][4],float output[3],int n,int c) ;
int close_position() ;
int read_data() ;
int read_ip_port() ;
//int get_maxi(int maxi[]) ;
//int get_mini(int mini[]) ;
//int get_anum(short num[]) ;
//int last_xyz(short x,short y,short z,short dix) ;
//int set_xyz(short x,short avgx,short y,short avgy,short z) ;


int set_one_dimension(int type,char *mac,float bx,int *out)  ;

#endif /* ANALY_H_ */
