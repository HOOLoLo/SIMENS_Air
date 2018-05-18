//
// Created by zgd on 18-4-20.
//

#ifndef CIOTFLY_ANALY_ROUTE_H

#define CIOTFLY_ANALY_ROUTE_H
#include <list>
#include "creat_graph.h"

//路径链表节点
const int INF=100000000;
int generate_route(int start,int dst);
void DFS(int start,int dst);

#endif //CIOTFLY_ANALY_ROUTE_H
