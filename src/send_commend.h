//
// Created by zgd on 18-4-20.
//

#ifndef CIOTFLY_SEND_COMMEND_H
#define CIOTFLY_SEND_COMMEND_H

#include <zconf.h>
#include "generate_pos.h"


extern int command_serial_fd;
//起飞过程指令，顺序执行。
extern char switch_hover[5];
extern char unlock_Throttl[5];
extern char unlock_Yaw[5];
extern char recover_Yaw[5];//={'\xFF','\x02','\x03','\xD0','\x05'};//解锁后方向（Yaw）回到中间值(不知道啥意思，就是解锁后调整一下方向吧)
extern char push_Throttl[5];//={'\xFF','\x02','\x02','\xD0','\x06'};//起飞的时候摆脱地心引力需要一个额外的加速度力,所以推油门
extern char Throttl[5];//={'\xFF','\x02','\x02','\xFD','\x05'};//起飞的油门指令,注意这条指令在推油门之后延迟500ms发送。油门回到中间值会悬停

//飞行指令
extern char go_forward[5];//={'\xFF','\x02','\x01','\x4B','\x06'};//飞行前进指令
extern const char stop_forward[5];//={'\xFF','\x02','\x01','\xFD','\x05'};//停止前进
extern char go_back[5];//={'\xFF','\x02','\x01','\x4A','\x07'};//后退
//extern char stop_back[5];//={'\xFF','\x02','\x01','\xFD','\x05'};//停止后退
//extern char go_left[5];//={'\xFF','\x02','\x00','\x6D','\x04'};//左移
extern const char stop_cross[5];//={'\xFF','\x02','\x00','\xD0','\x05'};//停止左移
extern char go_right[5];//={'\xFF','\x02','\x00','\x34','\x07'};//右移
//extern char stop_right[5];//={'\xFF','\x02','\x00','\xD0','\x05'};//停止右移
extern char turn_left[5];//={'\xFF','\x02','\x03','\x6D','\x04'};//左转
extern const char stop_rotation[5];//={'\xFF','\x02','\x03','\xD0','\x05'};//停止旋转
extern char turn_right[5];//={'\xFF','\x02','\x03','\x34','\x07'};//右转
extern char hover[5];//={'\xFF','\x02','\x02','\xFD','\x05'};//当前高度悬停
extern char go_up[5];//={'\xFF','\x02','\x02','\xD0','\x06'};//上升
extern char go_down[5];//={'\xFF','\x02','\x02','\x4A','\x07'};//下降
//降落
extern char land[5];//={'\xFF','\x02','\x04','\xBA','\x06'};//降落,注意降落结束后一分钟要发送切换悬停模式。
extern char self_check[5];
void send_go_left();
void take_off();
void theta_hold();//对航向角微调
void theta_hold(double theta);
void send_go_back();
void send_land();

int generate_command(int dst_x,int dst_y,int str_x,int str_y,int cur_X,int cur_Y);
void send_go_left();
void send_go_right();
void send_go_forward();

void send_hover();
void send_stop_cross();
void send_stop_front();

void send_stop_rotation();

void send_adj(int pix_x,int pix_y);
#endif //CIOTFLY_SEND_COMMEND_H
