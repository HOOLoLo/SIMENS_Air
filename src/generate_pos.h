//
// Created by zgd on 18-4-20.
//

#ifndef CIOTFLY_GENERATE_POS_H
#define CIOTFLY_GENERATE_POS_H

#include <list>
#include <pthread.h>
extern int pix_x;
extern int pix_y;
extern double theta;
extern int X;//最终坐标
extern int Y;//最终坐标
extern pthread_mutex_t mutex_pix;
extern pthread_mutex_t mutex_colortag;
extern pthread_mutex_t mutex_theta;

extern std::list<int> route;

extern int command_serial_fd;

extern short colortag;
extern pthread_t run_light_id;

extern pthread_t adjust_thread_id;//微调线程
extern pthread_t adjust_theta_id;//微调角度id

void *adjustment(void* arg);
void *adjustment_theta(void *arg);
void command_thread();
int create_command_thread();
#endif //CIOTFLY_GENERATE_POS_H
