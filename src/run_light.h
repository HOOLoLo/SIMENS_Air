//
// Created by zgd on 18-4-3.
//

#ifndef CIOTFLY_RUN_LIGHT_H
#define CIOTFLY_RUN_LIGHT_H

extern pthread_mutex_t mutex_colortag;
extern pthread_mutex_t mutex_pix;
extern int pix_x;
extern int pix_y;
extern short colortag;

void *run_light(void *arg);

extern bool light_thread;

extern pthread_mutex_t mutex_light_thread;

#endif //CIOTFLY_RUN_LIGHT_H
