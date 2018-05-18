/*
 * iotfly.c
 *
 *  Created on: Oct 12, 2017
 *      Author: root
 *      raspberrypi
 *      pi@192.168.1.151
 */

#include     <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <stddef.h>
#include <sys/time.h>
#include "analy.h"
#include "signal.h"



#include<pthread.h>

#include "creat_graph.h"
#include "analy_route.h"
#include "run_light.h"
#include "generate_pos.h"

#include <list>
#include <iostream>
#include <zconf.h>
#define theta_error 500;//初始化theta为error
using namespace std;
int sock_fd = 0 ;
int connect_fdd = 0 ;
int sock_fd_flag = 0 ;
Node G[MAX_NODE][MAX_NODE];
//
 _POINT_ gpoint[20]={{0,0,0}} ;


list<int> route;

int point_num_all=0 ;

int point_num_current=0 ;//记录当前读入基站个数，大于3说明四个基站已经读入完成

float inputData[20][4]={{0,0,0,0}} ;//inputdata数据传给定位算法得到output数组，然后point_num_current赋值为0

int  one_inputData[20][4]={{0,0,0,0}} ;

short last_point[10][3]={{0,0,0}} ;

//short avg_point[3] = {0,0,0} ;
float output[5]={0.0,0.0,0.0} ;//存储算法得到的位置数据

//int one_output[3]={0,0,0,} ;


//short point_short[20][6]={{0,0}} ;//point_short二维数组存储基站切换点的位置,数组大小为[20][6]

//short ab_point[20][2]={{0,0}} ;
int one_point_num = 0 ;
//short point_num = 0 ;
//short current_point = 0 ;//current_point当前飞机所在分组位置初始为0





int last_num = 0 ;
short i_lr = 0 ;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;//
pthread_mutex_t mutex_serial = PTHREAD_MUTEX_INITIALIZER;//
pthread_mutex_t mutex_send = PTHREAD_MUTEX_INITIALIZER;//
pthread_mutex_t mutex_pix = PTHREAD_MUTEX_INITIALIZER ;//pix_x,pix_y和pix_buf的互斥量
pthread_mutex_t mutex_colortag=PTHREAD_MUTEX_INITIALIZER;//蓝红灯光的互斥量，因为航向角只能用一个颜色的
pthread_mutex_t mutex_light_thread=PTHREAD_MUTEX_INITIALIZER;//图像线程互斥量

//char send_buf[100]={0} ;//给无人机发送的数组
//int send_len=0 ;
//int send_flag = 0 ;
int avg_d[10] = {0} ;


int serial_fd;//读取uwb数据的句柄
int command_serial_fd;//无人机串口发送数据句柄


short colortag=1;//用于在图像线程中直接对无人机调整角度初始化为蓝色
int pix_x = 0 ;//图像x坐标
int pix_y = 0 ;//图像y坐标
int X;//飞机实际X坐标
int Y;//飞机实际Y坐标


int start=0;//起点
int dst=1;//终点

bool light_thread=true;//标记是否要开启图像线程


pthread_t run_light_id;//图像处理 id
//pthread_t command_thread_id;

pthread_t adjust_thread_id;//微调线程id
int main(void){
	cout<<"123"<<endl;	
//char *com = "/dev/ttyUSB0" ;
	sleep(1) ;
    pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&mutex_send,NULL);
	pthread_mutex_init(&mutex_serial,NULL);
	pthread_mutex_init(&mutex_pix,NULL);
    pthread_mutex_init(&mutex_colortag,NULL);

	char mac[20] ;
	read_file_info();//读取基站的基本数据
    cout<<"read data success"<<endl;
    read_ip_port() ;//读取ip地址
    cout<<"read ip success"<<endl;
    clean_G();//清空图G
    cout<<"clean G success"<<endl;
    set_G();//设置图G;
    cout<<"set G success"<<endl;
//
//	cetc_create_send_thread();
//
//	server_create_thread();
    //cout<<"start run_light"<<endl;
//    pthread_create(&run_light_id,NULL,run_light,NULL);
    //cout<<"run_light"<<endl;
    cout<<"try to init"<<endl;
	init_position() ;//打开动态链接库，把get_position函数指针指向动态链接库中的函数
    cout<<"init postion success"<<endl;

    command_thread();

////	usleep(1000*15);
//	while (1) {
//		if(send_flag == 3){
//			if(send_len>100)
//				send_len = 0 ;
//			m_send_len = send_len ;
//			memcpy(m_send_buf,send_buf,m_send_len) ;
//			send_flag = 0 ;
//			//printf("send x,y,z 1\n") ;
//			//向无人机发送串口数据，还是用的旧板子的函数，这里先注释掉
//			com_send(m_send_buf,m_send_len) ;
//
//			//printf("send x,y,z 2\n") ;
////			gettimeofday (&tvafter , &tz);
////			printf("send 花费时间:%d %d\n",(tvafter.tv_sec-tvpre.tv_sec)*1000+(tvafter.tv_usec-tvpre.tv_usec)/1000,m_send_len);
//		}else{
//			usleep(1000) ;
//		}
//	}
//	close_position() ;
//
//    while(1){
//
//        usleep(10);
//       // printf("waiting\n");
 //   }

	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&mutex_send);
	pthread_mutex_destroy(&mutex_serial) ;
	pthread_mutex_destroy(&mutex_pix) ;
	pthread_mutex_destroy(&mutex_colortag);
    pthread_mutex_destroy(&mutex_light_thread);
	return EXIT_SUCCESS;
}
