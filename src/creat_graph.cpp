//
// Created by zgd on 18-4-18.
//
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "creat_graph.h"
using namespace std;

int node_num=0;//节点数量
//节点图，行为当前点，列为目标点
//把图清零
void clean_G(){
    for(int i=0;i<MAX_NODE;i++){
        for(int j=0;j<MAX_NODE;j++){
            G[i][j].link=G[j][i].link=false;
        }
    }
    return;
}




int set_G(){
    FILE *file;
    char *key="NUM";
    file=fopen("./ini/graph_conf.ini","rb");
    if(file==NULL){
    return -1;
    }
    int i=0 ;
    int j=0 ;

    char str[300];
    char buf[1024];
    memset(buf,'\0',1024);
    memset(str,'\0',300);
    while(fgets(buf,1024,file)){
            if(!memcmp(buf,key,3)){
                break;
            }
        memset(buf,'\0',1024) ;
    }
    i=4;
    j=0;
    while(buf[i]!='\n')
    {
        str[j] = buf[i] ;
        i++,j++ ;
    }

    node_num= atoi(str) ;//读到节点数量

    //读配置文件来给各个节点变量赋值
    for(int k=0;k<node_num;k++){
        memset(buf,'\0',1024) ;
        if(fgets(buf,1024,file)==NULL)break;//读入一行数据,
        int start;
        int dst;
        i=0;
        while (buf[i]!='\n'){
            if(buf[i]=='(') {
                i++;
                break;
            }
            i++;
        }

        memset(str,'\0',300);
        j=0;
        while(buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++ ,j++ ;
        }
        start=atoi(str);//当前点赋值

        memset(str,'\0',300);
        j=0;i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        dst=atoi(str);//目标点赋值
        G[start][dst].link= true;//当前点和目标点建立连接
        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].color=atoi(str);//颜色赋值

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].light_pos=atoi(str);//灯管坐标赋值

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].group=atoi(str);//分组赋值

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].dimension=atoi(str);//计算方法维度赋值

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].str_x=atoi(str);//出发点x坐标赋值


        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].str_y=atoi(str);//出发点x坐标赋值

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].dst_x=atoi(str);//目标位置x坐标

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=','&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].dst_y=atoi(str);//目标位置y坐标

        memset(str,'\0',300);
        j=0,i++;
        while (buf[i]!=')'&&buf[i]!='\n'){
            str[j]=buf[i];
            i++,j++;
        }
        G[start][dst].value=atoi(str);//路径值。
    }
    fclose(file) ;
    return 1 ;
}