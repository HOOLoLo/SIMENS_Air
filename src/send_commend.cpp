//
// Created by zgd on 18-4-20.
//

#include "send_commend.h"
#include <vector>
#include "Serial_RT.h"
#include <iostream>
using namespace std;
//è?·é??è??????????€???é???????§è?????
char switch_hover[5]={'\xFF','\x02','\x04','\xE7','\x05'};//????????°????????????

char unlock_Throttl[5]={'\xFF','\x02','\x02','\x57','\x04'};//è§?é?????é??
char unlock_Yaw[5]={'\xFF','\x02','\x03','\x82','\x07'};//è§?é????????
char recover_Yaw[5]={'\xFF','\x02','\x03','\xD0','\x05'};//è§?é??????????????Yaw????????°???é?????(??????é???????????????°±???è§?é?????è°??????????????????§)
char push_Throttl[5]={'\xFF','\x02','\x02','\xD0','\x06'};//è?·é??????????????è?±??°?????????é??è????????é???€???????é????????,????????????é??
char Throttl[5]={'\xFF','\x02','\x02','\x40','\x06'};//è?·é????????é???????€,??????è??????????€?????????é???????????è??500ms???é????????é???????°???é??????????????

//é??è???????€
const char stop_forward[5]={'\xFF','\x02','\x01','\xDC','\x05'};//前后回中命令
char go_forward[5]={'\xFF','\x02','\x01','\xDC','\x05'};//向前命令初始化为中间值具体数值在函数中加
char go_back[5]={'\xFF','\x02','\x01','\xDC','\x05'};//向后命令初始化为中间值具体数值在函数中加

char go_forward_road[5]={'\xFF','\x02','\x01','\xDC','\x05'};//路上的向前命令初始化为中间值具体数值在函数中加
char go_back_road[5]={'\xFF','\x02','\x01','\xDC','\x05'};//路上的向后命令初始化为中间值具体数值在函数中加


const char stop_cross[5]={'\xFF','\x02','\x00','\xDC','\x05'};//左右回中命令
char go_left[5]={'\xFF','\x02','\x00','\xDC','\x05'};//向左初始化为中间值具体数值在函数中加
char go_right[5]={'\xFF','\x02','\x00','\xDC','\x05'};//向右初始化为中间值具体数值在函数中加

char go_left_road[5]={'\xFF','\x02','\x00','\xDC','\x05'};//路上的向左初始化为中间值具体数值在函数中加
char go_right_road[5]={'\xFF','\x02','\x00','\xDC','\x05'};//路上的向右初始化为中间值具体数值在函数中加




char turn_left[5]={'\xFF','\x02','\x03','\xA4','\x05'};//?·?è??
const char stop_rotation[5]={'\xFF','\x02','\x03','\xD0','\x05'};//旋转回中
char turn_right[5]={'\xFF','\x02','\x03','\x13','\x06'};//???è??
char hover[5]={'\xFF','\x02','\x02','\xFD','\x05'};//??????é???????????
char go_up[5]={'\xFF','\x02','\x02','\xD0','\x06'};//??????
char go_down[5]={'\xFF','\x02','\x02','\x4A','\x07'};//???é??
char self_check[5]={'\xFF','\x02','\x04','\x57','\x04'};//自稳模式
//é??è??
char land[5]={'\xFF','\x02','\x04','\xBA','\x06'};//é??è??,??????é??è?????????????????é??è?????é???????????????????????

void take_off(){
    uart_write(command_serial_fd,self_check,5);
	usleep(1000*5000);
   	uart_write(command_serial_fd,switch_hover,5);//????????°????????????
    usleep(1000*100);
	uart_write(command_serial_fd,switch_hover,5);//????????°????????????
    usleep(1000*100);
    uart_write(command_serial_fd,stop_forward,5);
    usleep(1000*100);
    uart_write(command_serial_fd,stop_cross,5);
    usleep(1000*100);
    uart_write(command_serial_fd,unlock_Throttl,5);//è§?é?????é??
    uart_write(command_serial_fd,unlock_Yaw,5);//è§?é????????
    usleep(1000*5000);
    uart_write(command_serial_fd,recover_Yaw,5);//????????????
    usleep(1000*1000);
    uart_write(command_serial_fd,push_Throttl,5);//??????é??
    usleep(1000*2500);
    uart_write(command_serial_fd,Throttl,5);
}

void send_stop_front(){
	uart_write(command_serial_fd,stop_forward,5);
}


void send_go_forward(){
    short speed=0x05AA;
    go_forward[3] = (speed & 0x00ff);
    go_forward[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_left[i]&0xFF)<<endl;
//    }
    // cout<<"time1="<<time1<<endl;
    uart_write(command_serial_fd, go_forward, 5);
    usleep(1000*13);
    uart_write(command_serial_fd,stop_forward,5);
    usleep(1000*10);
}

void send_go_back(){
    short speed=0x062C;
    go_back[3] = (speed & 0x00ff);
    go_back[4] = ((speed & 0xff00) >> 8);

    uart_write(command_serial_fd, go_back, 5);
    usleep(1000*13);
    uart_write(command_serial_fd,stop_forward,5);
    usleep(1000*10);

}
//è§???????????????°
void theta_hold(double theta) {

    // uart_write(command_serial_fd,stop_rotation,5);
//	cout<<"theta="<<theta<<endl;
    double deviation=(theta-90);//è§????????·????90???????????????è§?????????????????·?è??è§????????€§??????è??è§????????°?
    
    if(abs(deviation)>=30){
	cout<<deviation<<endl;
	uart_write(command_serial_fd,land,5);
	}

    else if(deviation>3) {
        uart_write(command_serial_fd, turn_left, 5);

	usleep(1000*20);
     uart_write(command_serial_fd,stop_rotation,5);
//	cout<<"theta="<<theta<<endl;
    }
    else if(deviation<-3){
        uart_write(command_serial_fd,turn_right,5);

     usleep(1000*20);
     uart_write(command_serial_fd,stop_rotation,5);
//	cout<<"theta="<<theta<<endl;
    }
        uart_write(command_serial_fd, stop_rotation, 5);
        usleep(1000*20);


}



void send_stop_rotation() {
    uart_write(command_serial_fd,stop_rotation,5);
}

//???é??é??è???????€
void send_land(){
    uart_write(command_serial_fd,land,5);
    sleep(60);
    uart_write(command_serial_fd,switch_hover,5);//é??è?????????????????é??????????????????????????

}


void send_go_left(){
    short speed=0x05AA;
    go_left[3] = (speed & 0x00ff);
    go_left[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_left[i]&0xFF)<<endl;
//    }
    // cout<<"time1="<<time1<<endl;
    uart_write(command_serial_fd, go_left, 5);
    usleep(1000*13);
    uart_write(command_serial_fd,stop_cross,5);
    usleep(1000*10);
}

void send_go_right(){
    short speed=0x062C;
    go_right[3] = (speed & 0x00ff);
    go_right[4] = ((speed & 0xff00) >> 8);
//    for(int i=0;i<5;i++){
//        cout<<hex<<(go_right[i]&0xFF)<<endl;
//    }
    // cout<<"time1="<<time1<<endl;
    uart_write(command_serial_fd, go_right, 5);
    usleep(1000*13);
    uart_write(command_serial_fd,stop_cross,5);
    usleep(1000*10);
}



void send_stop_cross(){
	uart_write(command_serial_fd,stop_cross,5);
}


/*int generate_command(int dst_x,int dst_y,int cur_X,int cur_Y){
    //?????€???????????°è??????????
	cout<<"dst_x="<<dst_x<<"dst_y"<<dst_y<<endl;
    if(abs(cur_X-dst_x)<=30&&abs(cur_Y-dst_y<=30)){
        uart_write(command_serial_fd,stop_forward,5);
        uart_write(command_serial_fd,stop_back,5);
        uart_write(command_serial_fd,stop_left,5);
        uart_write(command_serial_fd,stop_right,5);
        //???é??è?????é???????????è°?????????€
        sleep(3);
        return 1;//è???????°è??
    }
    else{
	//cout<<"dst_x="<<dst_x<<endl;
        if((cur_X-dst_x)<0){
            //?????????????????€
		
            uart_write(command_serial_fd,go_forward,5);
	//	cout<<"forward success"<<endl;
        }
        else{
            //?????????????????€
            uart_write(command_serial_fd,go_back,5);
        }
        if((cur_Y-dst_y)<0){
            //??????????????€
            uart_write(command_serial_fd,go_right,5);
        }
        else{
            //???????·??????€
            uart_write(command_serial_fd,go_left,5);
        }
    }
    return 0;
}*/

int generate_command(int dst_x,int dst_y,int str_x,int str_y,int cur_X,int cur_Y){
    //先判断是否到达目的?
	//cout<<"dst_x="<<dst_x<<"dst_y"<<dst_y<<endl;

    short speed_forward=0x05C8;
    short speed_back=0x05F0;
    short speed_left=0x05C8;
    short speed_right=0x05F0;


    go_forward_road[3] = (speed_forward & 0x00ff);
    go_forward_road[4] = ((speed_forward & 0xff00) >> 8);
    go_back_road[3] = (speed_back & 0x00ff);
    go_back_road[4] = ((speed_back & 0xff00) >> 8);

    go_left_road[3] = (speed_left & 0x00ff);
    go_left_road[4] = ((speed_left& 0xff00) >> 8);
    go_right_road[3] = (speed_right & 0x00ff);
    go_right_road[4] = ((speed_right & 0xff00) >> 8);

    if(dst_x==str_x){//y方向飞行
        if(abs(cur_Y-dst_y)<=30){
            return 1;
        }
        else{
            if (cur_Y-dst_y>0){
                uart_write(command_serial_fd,go_left_road,5);
                
            }
            else if(cur_Y-dst_y<0){
                uart_write(command_serial_fd,go_right_road,5);
            }
        }
    }
    else if(dst_y==str_y){//x方向飞行
        if(abs(cur_X-dst_x)<=30){
            return 1;
        }
        else{
            if (cur_X-dst_x>0){
                uart_write(command_serial_fd,go_back_road,5);

            }
            else if(cur_X-dst_x<0){
                uart_write(command_serial_fd,go_forward_road,5);
            }
        }
    }
return 0;
}



void send_adj(int pix_x,int pix_y){

    if(pix_x>340&&pix_x<640&&pix_y>250&&pix_y<480){
        short speed_left=0x05AA;

        go_left[3] = (speed_left & 0x00ff);
        go_left[4] = ((speed_left & 0xff00) >> 8);

        short speed_back=0x062C;
        go_back[3] = (speed_back & 0x00ff);
        go_back[4] = ((speed_back & 0xff00) >> 8);

        uart_write(command_serial_fd, go_left, 5);
        uart_write(command_serial_fd, go_back, 5);
        usleep(1000*13);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*10);

        send_go_left();
        cout<<"send_go_left,go_back"<<endl;

    }

    else if(pix_x>0&&pix_x<300&&pix_y>0&&pix_y<230){
        short speed_right=0x062C;
        go_right[3] = (speed_right & 0x00ff);
        go_right[4] = ((speed_right & 0xff00) >> 8);

        short speed_forward=0x05AA;
        go_forward[3]=(speed_forward & 0x00ff);
        go_forward[4] =  ((speed_forward & 0xff00) >> 8);

        uart_write(command_serial_fd, go_right, 5);
        uart_write(command_serial_fd, go_forward, 5);
        usleep(1000*13);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*10);
        cout<<"send_go_right,go_forward"<<endl;
    }
    else if(pix_x>340&&pix_x<640&&pix_y>0&&pix_y<230){
        short speed_left=0x05AA;
        go_left[3] = (speed_left & 0x00ff);
        go_left[4] = ((speed_left & 0xff00) >> 8);

        short speed_forward=0x05AA;
        go_forward[3]=(speed_forward & 0x00ff);
        go_forward[4] =  ((speed_forward & 0xff00) >> 8);

        uart_write(command_serial_fd, go_left, 5);
        uart_write(command_serial_fd, go_forward, 5);
        usleep(1000*13);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*10);
        cout<<"send_go_left,go_forward"<<endl;
    }
    else if(pix_x>0&&pix_x<300&&pix_y>250&&pix_y<480){
        short speed_right=0x062C;
        go_right[3] = (speed_right & 0x00ff);
        go_right[4] = ((speed_right & 0xff00) >> 8);

        short speed_back=0x062C;
        go_back[3] = (speed_back & 0x00ff);
        go_back[4] = ((speed_back & 0xff00) >> 8);

        uart_write(command_serial_fd, go_right, 5);
        uart_write(command_serial_fd, go_back, 5);
        usleep(1000*13);
        uart_write(command_serial_fd,stop_cross,5);
        uart_write(command_serial_fd,stop_forward,5);
        usleep(1000*10);
        cout<<"send_go_left,go_back"<<endl;

    }

    else {
        send_stop_cross();
        send_stop_front();
        cout<<"stop"<<endl;
        usleep(1000*500);
    }




}

void send_hover() {
	    uart_write(command_serial_fd,stop_forward,5);	

	    uart_write(command_serial_fd,stop_cross,5);

            }


void calculate(int dev){
	int max=0x064B;
	int min=0x0578;
	int result; 
	result=max-min;
		
	
}
