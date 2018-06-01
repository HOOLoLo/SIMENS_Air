//
// Created by zgd on 18-4-20.
//

#include "send_commend.h"
#include <vector>
#include "Serial_RT.h"
#include <iostream>
using namespace std;

char switch_hover[5]={'\xFF','\x02','\x04','\xE7','\x05'};//切换成悬停模式

char unlock_Throttl[5]={'\xFF','\x02','\x02','\x57','\x04'};//解锁油门
char unlock_Yaw[5]={'\xFF','\x02','\x03','\x82','\x07'};//解锁方向
char recover_Yaw[5]={'\xFF','\x02','\x03','\xD0','\x05'};//方向回中
char push_Throttl[5]={'\xFF','\x02','\x02','\xD0','\x06'};//推油门
char Throttl[5]={'\xFF','\x02','\x02','\x40','\x06'};//油门回中

//前后方向
const char stop_forward[5]={'\xFF','\x02','\x01','\xDC','\x05'};//前后回中命令
char go_forward[5]={'\xFF','\x02','\x01','\xDC','\x05'};//向前命令初始化为中间值具体数值在函数中加
char go_back[5]={'\xFF','\x02','\x01','\xDC','\x05'};//向后命令初始化为中间值具体数值在函数中加

char go_forward_road[5]={'\xFF','\x02','\x01','\xDC','\x05'};//路上的向前命令初始化为中间值具体数值在函数中加
char go_back_road[5]={'\xFF','\x02','\x01','\xDC','\x05'};//路上的向后命令初始化为中间值具体数值在函数中加

//左右方向
const char stop_cross[5]={'\xFF','\x02','\x00','\xDC','\x05'};//左右回中命令
char go_left[5]={'\xFF','\x02','\x00','\xDC','\x05'};//向左初始化为中间值具体数值在函数中加
char go_right[5]={'\xFF','\x02','\x00','\xDC','\x05'};//向右初始化为中间值具体数值在函数中加

char go_left_road[5]={'\xFF','\x02','\x00','\xDC','\x05'};//路上的向左初始化为中间值具体数值在函数中加
char go_right_road[5]={'\xFF','\x02','\x00','\xDC','\x05'};//路上的向右初始化为中间值具体数值在函数中加



//旋转指令
char turn_left[5]={'\xFF','\x02','\x03','\xA4','\x05'};//左转
const char stop_rotation[5]={'\xFF','\x02','\x03','\xD0','\x05'};//旋转回中
char turn_right[5]={'\xFF','\x02','\x03','\x13','\x06'};//右转

//高度上面的指令暂时不用
char hover[5]={'\xFF','\x02','\x02','\xFD','\x05'};//高度回中
char go_up[5]={'\xFF','\x02','\x02','\xD0','\x06'};//高度增加
char go_down[5]={'\xFF','\x02','\x02','\x4A','\x07'};//高度减小

char self_check[5]={'\xFF','\x02','\x04','\x57','\x04'};//自检查模式
char land[5]={'\xFF','\x02','\x04','\xBA','\x06'};//降落


//起飞指令
void take_off(){
    uart_write(command_serial_fd,self_check,5);//先发一个自检查命令
	usleep(1000*5000);
   	uart_write(command_serial_fd,switch_hover,5);//切换到悬停模式
    usleep(1000*100);
	uart_write(command_serial_fd,switch_hover,5);//再次发送切换悬停
    usleep(1000*100);
    uart_write(command_serial_fd,stop_forward,5);//前后回中
    usleep(1000*100);
    uart_write(command_serial_fd,stop_cross,5);//左右回中
    usleep(1000*100);
    uart_write(command_serial_fd,unlock_Throttl,5);//解锁油门
    uart_write(command_serial_fd,unlock_Yaw,5);//解锁方向
    usleep(1000*5000);
    uart_write(command_serial_fd,recover_Yaw,5);//方向回中
    usleep(1000*1000);
    uart_write(command_serial_fd,push_Throttl,5);//推油门
    usleep(1000*2500);
    uart_write(command_serial_fd,Throttl,5);//油门回中
}

//前后回中指令
void send_stop_front(){
	uart_write(command_serial_fd,stop_forward,5);
}

//向前微调指令
void send_go_forward(){
    short speed=0x05BE;
    go_forward[3] = (speed & 0x00ff);
    go_forward[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_left[i]&0xFF)<<endl;
//    }
    // cout<<"time1="<<time1<<endl;
    uart_write(command_serial_fd, go_forward, 5);
    usleep(1000*20);
    uart_write(command_serial_fd,stop_forward,5);
    usleep(1000*8);
}


//向后微调指令
void send_go_back(){
    short speed=0x062C;
    go_back[3] = (speed & 0x00ff);
    go_back[4] = ((speed & 0xff00) >> 8);

    uart_write(command_serial_fd, go_back, 5);
    usleep(1000*20);
    uart_write(command_serial_fd,stop_forward,5);
    usleep(1000*8);

}
//维持角度的函数
void theta_hold(double theta) {
    // uart_write(command_serial_fd,stop_rotation,5);
    //cout<<"theta="<<theta<<endl;
    double deviation=(theta-90);//
    
    if(abs(deviation)>=30){//当角度大于30度的时候就降落
	cout<<"deviation="<<deviation<<endl;
	cout<<"theta="<<theta<<endl;
	uart_write(command_serial_fd,land,5);
	}

    else if(deviation>3) {//当角度大于93向左调整
        uart_write(command_serial_fd, turn_left, 5);
	    usleep(1000*20);//时间间隔20ms
        uart_write(command_serial_fd,stop_rotation,5);
        //	cout<<"theta="<<theta<<endl;
    }
    else if(deviation<-3){//当角度小于87向右调整
        uart_write(command_serial_fd,turn_right,5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_rotation,5);
        //	cout<<"theta="<<theta<<endl;
        }
        uart_write(command_serial_fd, stop_rotation, 5);
        usleep(1000*15);
}


//停止指令
void send_stop_rotation() {
    uart_write(command_serial_fd,stop_rotation,5);
}

//发送降落指令
void send_land(){
    cout<<"send_lend"<<endl;
    uart_write(command_serial_fd,land,5);
    sleep(60);
    uart_write(command_serial_fd,switch_hover,5);//降落60ms之后切换悬停模式

}


void send_go_left(int pix_x,int pix_y){
    short div=(short)(pix_x-320);//div左偏移像素
    short speed=0x05BE;//设置速度
    //if(div<=160){
    //    speed+=div/4;
    //  }

    //速度赋值给数组
    go_left[3] = (speed & 0x00ff);
    go_left[4] = ((speed & 0xff00) >> 8);

    /*for(int i=0;i<5;i++){
       cout<<hex<<(go_left[i]&0xFF)<<endl;
    }
     cout<<"time1="<<time1<<endl;*/
    uart_write(command_serial_fd, go_left, 5);
    usleep(1000*20);
    uart_write(command_serial_fd,stop_cross,5);
    usleep(1000*8);
}


//向右微调
void send_go_right(int pix_x,int pix_y){
    short div=(short)(320-pix_x);//div右偏移的像素距离
    short speed=0x0622;
    //if(div<=160){
   //     speed-=(160-div)/2;
 //   }
  //  cout<<"speed ="<<speed<<endl;
    //速度赋值给数组
    go_right[3] = (speed & 0x00ff);
    go_right[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_right[i]&0xFF)<<endl;
//    }
    // cout<<"time1="<<time1<<endl;

    uart_write(command_serial_fd, go_right, 5);
    usleep(1000*20);
    uart_write(command_serial_fd,stop_cross,5);
    usleep(1000*8);
}


//左右油门回中
void send_stop_cross(){
	uart_write(command_serial_fd,stop_cross,5);
}




int generate_command(int dst_x,int dst_y,int str_x,int str_y,int cur_X,int cur_Y) {
    //先判断是否到达目的?
    //cout<<"dst_x="<<dst_x<<"dst_y"<<dst_y<<endl;

    short speed_forward = 0x05C8;//速度都设为20
    short speed_back = 0x05F0;
    short speed_left = 0x05BE;//左速度30
    short speed_right = 0x05F0;


    go_forward_road[3] = (speed_forward & 0x00ff);
    go_forward_road[4] = ((speed_forward & 0xff00) >> 8);
    go_back_road[3] = (speed_back & 0x00ff);
    go_back_road[4] = ((speed_back & 0xff00) >> 8);

    go_left_road[3] = (speed_left & 0x00ff);
    go_left_road[4] = ((speed_left & 0xff00) >> 8);
    go_right_road[3] = (speed_right & 0x00ff);
    go_right_road[4] = ((speed_right & 0xff00) >> 8);


    if (dst_x == str_x) {//y方向飞行

        if ((str_y - dst_y) > 0) {//出发点的y大于终点的y

            if ((cur_Y - dst_y) <= 30) {//只要小于dst_y+30就认为到达
                return 1;
            }
            else {
                uart_write(command_serial_fd, go_left_road, 5);
            }//否则向左飞行
        }
        else if ((str_y - dst_y) < 0) {
            if ((cur_Y - dst_y) >= -30) {//只要大于dst_y-30就认为到达
                return 1;
            } else {
                uart_write(command_serial_fd, go_right_road, 5);
            }//否则向右飞行
        }
        else return 1;//如果出发点y=目标点y就认为到达
    }

    else if (dst_y == str_y) {//x方向飞行
        if ((str_x - dst_x) > 0) {//出发点的x大于终点的x
            if ((cur_X - dst_x) <= 30) {//只要小于dst_x+30就认为到达
                return 1;
            }
            else uart_write(command_serial_fd, go_back_road, 5);//否则向后飞行
        }

        else if ((str_x - dst_x) < 0) {
            if ((cur_X - dst_x) >= -30) {//只要大于dst_x-30就认为到达
                return 1;
            }
            else {
                uart_write(command_serial_fd, go_forward_road, 5);
            }//否则向前飞行
        }
        else return 1;
    }
    return 0;
}





void send_adj(int pix_x,int pix_y){
   //往左往后
    if(pix_x>340&&pix_x<640&&pix_y>250&&pix_y<480){
        short speed_left=0x05BE;
        go_left[3] = (speed_left & 0x00ff);
        go_left[4] = ((speed_left & 0xff00) >> 8);

        short speed_back=0x062C;
        go_back[3] = (speed_back & 0x00ff);
        go_back[4] = ((speed_back & 0xff00) >> 8);

        uart_write(command_serial_fd, go_left, 5);
        uart_write(command_serial_fd, go_back, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*8);

        cout<<"send_go_left,go_back"<<endl;

    }

    else if(pix_x>0&&pix_x<300&&pix_y>0&&pix_y<230){
        short speed_right=0x0622;
        go_right[3] = (speed_right & 0x00ff);
        go_right[4] = ((speed_right & 0xff00) >> 8);

        short speed_forward=0x05BE;
        go_forward[3]=(speed_forward & 0x00ff);
        go_forward[4] =  ((speed_forward & 0xff00) >> 8);

        uart_write(command_serial_fd, go_right, 5);
        uart_write(command_serial_fd, go_forward, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*8);
        cout<<"send_go_right,go_forward"<<endl;
    }
    else if(pix_x>340&&pix_x<640&&pix_y>0&&pix_y<230){
        short speed_left=0x05BE;
        go_left[3] = (speed_left & 0x00ff);
        go_left[4] = ((speed_left & 0xff00) >> 8);

        short speed_forward=0x05BE;
        go_forward[3]=(speed_forward & 0x00ff);
        go_forward[4] =  ((speed_forward & 0xff00) >> 8);

        uart_write(command_serial_fd, go_left, 5);
        uart_write(command_serial_fd, go_forward, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*8);
        cout<<"send_go_left,go_forward"<<endl;
    }
    else if(pix_x>0&&pix_x<300&&pix_y>250&&pix_y<480){
        short speed_right=0x0622;
        go_right[3] = (speed_right & 0x00ff);
        go_right[4] = ((speed_right & 0xff00) >> 8);

        short speed_back=0x062C;
        go_back[3] = (speed_back & 0x00ff);
        go_back[4] = ((speed_back & 0xff00) >> 8);

        uart_write(command_serial_fd, go_right, 5);
        uart_write(command_serial_fd, go_back, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*8);
        cout<<"send_go_left,go_back"<<endl;
    }
    else if(pix_x>340&&pix_x<640){//只向左
        short speed=0x05BE;
        go_left[3] = (speed & 0x00ff);
        go_left[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_left[i]&0xFF)<<endl;
//    }
        // cout<<"time1="<<time1<<endl;
        uart_write(command_serial_fd, go_left, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_cross,5);
        usleep(1000*8);
        cout<<"send_go_left"<<endl;
    }
    else if (pix_x>0&&pix_x<300){//只向右
        short speed=0x0622;
        go_right[3] = (speed & 0x00ff);
        go_right[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_right[i]&0xFF)<<endl;
//    }
        // cout<<"time1="<<time1<<endl;
        uart_write(command_serial_fd, go_right, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_cross,5);
        usleep(1000*8);
        cout<<"send_go_right"<<endl;

    }
    else if(pix_y>250&&pix_y<480){//只向后
        short speed=0x0622;
        go_back[3] = (speed & 0x00ff);
        go_back[4] = ((speed & 0xff00) >> 8);

        uart_write(command_serial_fd, go_back, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*8);
        cout<<"send_go_back"<<endl;
    }
    else if(pix_y>0&&pix_y<230){//只向前
        short speed=0x05BE;
        go_forward[3] = (speed & 0x00ff);
        go_forward[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_left[i]&0xFF)<<endl;
//    }
        // cout<<"time1="<<time1<<endl;
        uart_write(command_serial_fd, go_forward, 5);
        usleep(1000*20);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*8);
        cout<<"send_go_forward"<<endl;
    }
    else {
        send_stop_cross();
        send_stop_front();
        cout<<"stop"<<endl;
        usleep(1000*500);
    }




}


//前后左右油门值回中，就是悬停
void send_hover() {
	    uart_write(command_serial_fd,stop_forward,5);
	    uart_write(command_serial_fd,stop_cross,5);
	    }



