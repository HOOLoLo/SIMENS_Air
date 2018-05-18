//
// Created by zgd on 18-4-20.
//

//接收后台指令然后生成路线链表
//用Djistra算法生成最短路径
#include <cstdio>

#include <iostream>
#include "analy_route.h"

#include "generate_pos.h"


using namespace std;

int dis[MAX_NODE];//起点到达各点的最短路径长度
int pre[MAX_NODE];//pre[v]表示从起点到顶点v的最短路径上v的前一个顶点
bool vis[MAX_NODE]={false};//标记数组，vis[i]==true表示已经访问。初值均为false


int generate_route(int start,int dst){
    if(start==dst){//防止起点和终点是同一个点
        cout<<"mission error start=dst"<<endl;
        return 0;
    }
    route.clear();

    //生成最短路径的pre[]数组
    fill(dis,dis+MAX_NODE,INF);//将整个dis数组赋值为INF
    fill(vis,vis+MAX_NODE,false);
    for(int i=0;i<MAX_NODE;i++)pre[i]=i;//初始状态下设每个节点的前驱为自身
    dis[start]=0;
    for(int i=0;i<MAX_NODE;i++){
        int u=-1,MIN=INF;//u使dis[u]最小，MIN存放最小的dis[u],每次循环的时候都赋值为INF
        for(int j=0;j<MAX_NODE;j++){//找到未访问的顶点中d[]最小的
            if(vis[j]==false&&dis[j]<MIN){
                u=j;
                MIN=dis[j];
            }
        }
        if(u==-1) break;//说明剩下的顶点和起点不连通
        vis[u]=true;//标记u已经访问
        for(int v=0;v<MAX_NODE;v++){
            //如果v未访问&&u能达到v&&以u为中介点可以使dis[v]更优
            if(vis[v]==false&&G[u][v].link==true && dis[u]+G[u][v].value<dis[v]){//这里要考虑点到本身是不连通的问题吗，不用考虑因为u和v相等时，link也是false的
                dis[v]=dis[u]+G[u][v].value;
                pre[v]=u;
            }
        }
    }
    if(dis[dst]!=INF) {//如果目的地是能够到达的
        route.push_back(start);//链表第一个节点赋值为起点
        DFS(start, dst);
        return 1;
    }
    else {
        cout<<"can't arrive dst"<<endl;
        return 0;
    }//如果不能到达
}

//深度遍历，递归的将每个节点编号顺序的push到route链表中
void DFS(int start,int dst){
    if(start==dst){//当前要访问的节点已经是起点,直接返回。
        return;
    }
    DFS(start,pre[dst]);
    route.push_back(dst);//最深的return之后,把每个节点加到链表中
}
