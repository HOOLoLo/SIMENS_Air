#include "get_light.h"
#include "run_light.h"
#include "send_commend.h"
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <pthread.h>  
#include <sched.h>
#include "Serial_RT.h"
using namespace std;
using namespace cv;

static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond=PTHREAD_COND_INITIALIZER;
//用于记录线程是否已经在等待的变量,用于解决Linux中signal后变量会复位的问题.
int thread_count=0;
struct arg_thread
{
	uchar red_L[3];
	uchar red_H[3];
	Point2i core;
	int theta;
	int tag;

};
const int mthreshold=10;
Mat pframe;//线程中使用的帧Mat
void mutex_unlock(void * p){
	pthread_mutex_unlock((pthread_mutex_t *)p);
	return ;
}

void * red_light_thread(void * arg)
{	
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(1,&mask);
	if (pthread_setaffinity_np(pthread_self(),sizeof(mask),&mask)<0)
		{
			fprintf(stderr,"set thread affinity failed\n");
		}
	arg_thread* arg_T = (arg_thread*)arg;
	pthread_cleanup_push(&mutex_unlock, (void*)&mutex);
	Point2i tophead;
	Point2i bottomhead;
	Point2i toptail;
	Point2i bottomtail;
	Point2i top;
	Point2i bottom;
	Mat hsv;
	Mat dst=Mat(640,480,CV_8UC1);
//	int bad_frame=0;
//        char name[20]={0};
//        bad_frame++;
//        sprintf(name,"2%d.jpg",bad_frame);
//        imwrite(name,pframe);
	while (1) {
	//	cout << "thread is running" << endl;

		pthread_mutex_lock(&mutex);
	//	cout<<"thread locked the mutex"<<endl;

		thread_count++;
	//	cout<<"thread_count:"<<thread_count<<endl;

		pthread_cond_wait(&cond, &mutex);

	//	cout<<"thread process start"<<endl;
		cvtColor(pframe, hsv, COLOR_BGR2HSV);
		inrange_c(hsv, arg_T->red_L, arg_T->red_H, dst);
		arg_T->theta = find_true_theta(dst, mthreshold, tophead, toptail, bottomhead, bottomtail, top, bottom);
		arg_T->core = find_core(top, bottom, dst.cols, dst.rows);

	//	cout << "done" << endl;
		arg_T->tag=1;
		pthread_mutex_unlock(&mutex);

	}
	pthread_cleanup_pop(0);
	pthread_exit(0);
}


void *run_light(void *arg) {
	cpu_set_t m_mask;	
	CPU_ZERO(&m_mask);
	CPU_SET(2,&m_mask);
	if (pthread_setaffinity_np(pthread_self(),sizeof(m_mask),&m_mask)<0)
	{
		fprintf(stderr,"set thread affinity failed\n");
	}
	pthread_t tid;//红色线程id
	//ofstream ofile("data.txt");
	/*int iSend, iRecv;
	//int sock;
	char msg[22] = { 0x4D,0x4B,0x01,0x06,0x02,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	char rec_buff[512];
	 */
	Mat hsv;
	Mat gray;
	Mat gray2;
	Mat fingray;
	Point2i core3 = Point2i(0, 0);
	int HvL = 115;
	int SvL = 200;
	int VvL = 100;
	int HvH = 125;
	int SvH = 255;
	int VvH = 255;
	uchar red_L[3] = { 170,150,20 };
	uchar red_H[3] = { 180,255,255 };
	uchar blue_L[3] = { 115,100,100 };
	uchar blue_H[3] = { 125,255,255 };
	arg_thread arg_thread;//{ { red_L[0],red_L[1],red_L[2] },{ red_H[0],red_H[1],red_H[2] },core3,0 };
	arg_thread.red_H[0]=red_H[0];
	arg_thread.red_H[1]=red_H[1];
	arg_thread.red_H[2]=red_H[2];
	arg_thread.red_L[0]=red_L[0];
	arg_thread.red_L[1]=red_L[1];
	arg_thread.red_L[2]=red_L[2];
	arg_thread.core=core3;
    arg_thread.tag=0;
	uchar table_H[256];
	uchar table_S[256];
	uchar table_V[256];
	pthread_create(&tid, NULL,red_light_thread,(void *) &arg_thread);
	VideoCapture cap(209);

	int chec_num=0;
	if (!cap.isOpened())
		cout << "camera error" << endl;
	//设置摄像头曝光度
	//if(cap.set(CV_CAP_PROP_EXPOSURE,-1)<0)
/*	//	cout<<"set error"<<endl;
//	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP))<0)
//	{
//		perror("socket error");
//		return 1;
//	}
//	struct sockaddr_in server_addr;
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_port = htons(23456);
//	server_addr.sin_addr.s_addr = inet_addr("193.168.1.100");
//	socklen_t len = sizeof(server_addr);


	//char buf[1024];
	//while(1){
	//	int r=recvfrom(sever_sockfd,buf,sizeof(buf)-1,0,);
	//	if(r<0){
	//	perror("recvfrom");
	//	exit(3);
	//	}
	//	else{
	*/
	Mat frame;

	Size nsize = Size(640, 480);
	//保存视频用
	        VideoWriter writer=VideoWriter("video.avi",CV_FOURCC('M','J','P','G'),30,nsize,true);
	int color_count = 0;

	int bad_frame=0;
	while (cap.isOpened()) {



		//if(frame_count<3) continue;

		pthread_mutex_lock(&mutex_light_thread);
		if(!light_thread){//如果当前不要图像处理就结束线程
            pthread_mutex_unlock(&mutex_light_thread);
		    break;
		}
        pthread_mutex_unlock(&mutex_light_thread);

		//double t = (double)getTickCount();
		while (!cap.read(frame)){
			cap.read(frame);
		//	cout<<"reading camera...."<<endl;
		}


		if (pthread_mutex_lock(&mutex) == 0) {
		//	cout<<"main thread locked the mutex"<<endl;
			//while (!cap.read(pframe)){
				//cap.read(pframe);
				//cout<<"reading camera...."<<endl;
				pframe=frame;
			//}
			pthread_mutex_unlock(&mutex);
		//	cout<<"main thread released the mutex"<<endl;
		}


		while(1){
			if(thread_count>0){
			pthread_mutex_lock(&mutex);
			//	cout<<"signaling"<<endl;
				pthread_cond_signal(&cond);
				thread_count--;
			pthread_mutex_unlock(&mutex);
			break;
			}
		}
		
		
		Mat edgePic;
		Mat linePic;
		int nRows = frame.rows;
		int nCols = frame.cols;
		Point2i tophead;
		Point2i bottomhead;
		Point2i toptail;
		Point2i bottomtail;
		Point2i top;
		Point2i bottom;
		Point2i core = Point2i(0, 0);
		short int tag;
		double theta1 = err;
	
		Point2i tophead2;
		Point2i bottomhead2;
		Point2i toptail2;
		Point2i bottomtail2;
		Point2i top2;
		Point2i bottom2;
		Point2i core2 = Point2i(0, 0);
		double theta2 = err;
		Mat dst = Mat(frame.rows, frame.cols, CV_8UC1);
		Mat dst_change = Mat(frame.cols, frame.rows, CV_8UC1);
		Mat rotation = Mat(frame.cols, frame.rows, CV_8UC1);
		Mat rotate = (Mat_<float>(2, 3) << 0, 1, 0, 1, 0, 0);


		cvtColor(frame, hsv, COLOR_BGR2HSV);
		inrange(hsv, blue_L, blue_H, dst );

//        Mat dst2 = Mat(frame.cols, frame.rows, CV_8UC1);
//		inrange_c(hsv,red_L,red_H,dst2);


/*		//Mat LUT= Mat(frame.rows, frame.cols, CV_8UC1);

		//inrange_lut(hsv, table_H, table_S, table_V, LUT);
		//仿射变换
		//warpAffine(dst_change, rotation, rotate, Size(480, 640));
*/
		theta1 = find_true_theta(dst, mthreshold, tophead, toptail, bottomhead, bottomtail, top, bottom);
	//	cout<<"theta1="<<theta1<<endl;
/*
		//保存时间
		//time_t ltime;
		//struct tm * timeinfo;
		//time(&ltime);
		//timeinfo =localtime(&ltime);
		//ofile<<timeinfo->tm_mday<<" "<<timeinfo->tm_hour<<" "<<timeinfo->tm_min<<" "<<timeinfo->tm_sec<<" "<<endl;
*/
		if (theta1 != err) {
            //	cout<<theta<<endl;
            core = find_core(top, bottom, frame.cols, frame.rows);

          //  cout << "Blue:" << "x:" << core.x << "y:" << core.y << endl;
        }
        else{
          //  char name[20];
		    //bad_frame++;
            //sprintf(name,"%d.jpg",bad_frame);
            //imwrite(name,frame);
		 //   cout<<"blue_error"<<endl;
			core.x=0;
			core.y=0;
			}

        //d
		while(1){
			if(arg_thread.tag==1){
			if (arg_thread.theta!=err) {
			//core2 = find_core(top2, bottom2, frame.cols, frame.rows);
			//ofile<<core2.y<<endl;
			theta2=arg_thread.theta;
			core2=arg_thread.core;
	//		cout << "red:" << "x:" << arg_thread.core.x << "y:" << arg_thread.core.y << endl;

			}
			else{
				core2.x=0;
				core2.y=0;
				//cout<<"red_error"<<endl;
               // theta2 = find_true_theta(dst2, mthreshold, tophead2, toptail2, bottomhead2, bottomtail2, top2, bottom2);
               // core2 = find_core(top2, bottom2, dst2.cols, dst2.rows);
              //  cout<<"main_theta2="<<theta2<<endl;

//                char name[20];
//                  bad_frame++;
//                  sprintf(name,"%d.jpg",bad_frame);
//                  imwrite(name,dst2);
			}
			arg_thread.tag=0;
			break;
		}
		//	writer.write(frame);
	}
       //  char name[20];
      //  bad_frame++;
      //  sprintf(name,"%d.jpg",bad_frame);
       // imwrite(name,dst2);
        //writer.write(dst);
		//如果两个theta有一个不为0
        double t = (double)getTickCount();
		if (theta1 != err || theta2 != err) {
			//colortag=1是蓝色2是红色在读的时候先锁住
		//	chec_num++;
		//	cout<<"light thread try to lock colortag"<<endl;
			pthread_mutex_lock(&mutex_colortag);
		//	pthread_mutex_lock(&mutex_theta);
			if (colortag == 1 && theta1 != err&&!(core.x>dst.cols)) {
				if (theta1 > 5 && theta1 < 175) {
                    pthread_mutex_lock(&mutex_theta);
					theta=theta1;
                    pthread_mutex_unlock(&mutex_theta);
				//	cout<<"blue_theta="<<theta<<endl;
				}
			/*if(chec_num>0){		
				if(core.x>340&&core.x<640){
					send_go_left();
                                        cout<<"go_left"<<endl;
					usleep(1000*600);
					//send_go_right();
					//usleep(1000*25);
					send_stop_cross();
											
				}
				else if(core.x>0&&core.x<300){
					send_go_right();
					cout<<"go_right"<<endl;
					usleep(1000*600);
					//send_go_left();
					//usleep(1000*25);
					send_stop_cross();
				}
			             // usleep(1000*50);
				else {
					send_stop_cross();
					cout<<"stop"<<endl;	
				}
				chec_num=0;
			}*/
			}
//colortag=2是红色
			else if (colortag == 2 && theta2 != err && !(core2.x>dst.rows)) {
				if (theta2 > 5 && theta2 < 175) {
                    pthread_mutex_lock(&mutex_theta);
					theta=(180-theta2);
                    pthread_mutex_unlock(&mutex_theta);
					//cout<<"red_theta="<<theta<<endl;
					}
			/*if(chec_num>0){
				if(core2.x>240&&core2.x<480){
					send_go_back();
                                        cout<<"go_left"<<endl;
					usleep(1000*100);
										
				}
				else if(core2.x>0&&core2.x<240){
					send_go_forward();
					cout<<"go_right"<<endl;
					usleep(1000*100);
				}
			             // usleep(1000*50);
				//else {
					send_stop_front();
					cout<<"stop"<<endl;	
				//}
				
				chec_num=0;
                            }*/
			}
/*			else if(colortag==3 && theta1!=err && theta2!=err){//转角处任意theta都能赋值
				if (theta1 > 5 && theta1 < 175) {
					theta=theta1;
				}
				else if (theta2 > 5 && theta2 < 175) {
					theta=(180-theta2);
				}
			}*/
			else if(colortag==0){//xuanting
				send_hover();
			}
			pthread_mutex_unlock(&mutex_colortag);
		//	cout<<"unlock colortag"<<endl;
		//	pthread_mutex_unlock(&mutex_theta);
            t = ((double)getTickCount() - t) / getTickFrequency();
		}

		else {//停止旋转
			uart_write(command_serial_fd,stop_rotation,5);

		}

           // double t = (double)getTickCount();
			pthread_mutex_lock(&mutex_pix);
			//判断core的x和y是否超过了像素值，没有则正常
			if(!(core.x>dst.cols)&&!(core2.x>dst.rows)) {//无论什么时候都给
				pix_x = core.x;
				pix_y = core2.x;

			}
			pthread_mutex_unlock(&mutex_pix);
		//    cout << colortag << endl;

		//cout << "light_times passed in seconds: " << t << endl;
	/*	//	if(iRecv<0){
		//	perror("recvfrom");
		//	}
		//	else if(iRecv==0){
		//	break;
		//	else{
		//		break
		//	}
	 */
	}
	writer.release();//保存视频用
	//close(sock);
	cap.release();
	cin.get();
	return NULL;
}

