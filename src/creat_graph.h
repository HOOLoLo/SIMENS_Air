//
// Created by zgd on 18-4-18.
//

#ifndef CIOTFLY_CREAT_GRAPH_H
#define CIOTFLY_CREAT_GRAPH_H


#define MAX_NODE 30//这里预设总共有30个节点，包括转弯起飞和降落点
struct Node{
    bool link;
    short group;//a，b路线上所在的分组。
    short dimension;
    short color;//a,b路线上灯管的颜色
    int light_pos;
    int str_x,str_y;//出发点x,y坐标
    int dst_x,dst_y;//目标点位置x,y坐标,用于与时事位置判断。
    int value;//路径的权值
    short commond;//a到b所需要发送的命令。
};

void clean_G();
int set_G();

extern Node G[MAX_NODE][MAX_NODE];
#endif //CIOTFLY_ANALY_ROUTE_H
