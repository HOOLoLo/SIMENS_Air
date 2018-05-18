/*
 * analy.c
 *
 *  Created on: Oct 17, 2017
 *      Author: root
 */
#include "analy.h"
#include <iostream>
char pt_ip[20]={'\0','\0'} ;
//int pt_port=0 ;
void *location_lib=NULL ;
int (*get_position)(float(*inputA)[4], float(*outputA), int n, int c)=NULL;

int maxX =5000 ;
int maxY = 5000 ;
int maxZ = 5000 ;
int maxi[2] ={0,0};
int mini[2] ={0,0};
/*
 * f1,1,0
 */
char strbx[80] ;
char strcy[80] ;
using namespace std;
//解析uwb的数据
int get_info(char *buf,int len,char *mac,int *bx,int *cy){
	if(len>0){
		char *str = NULL;
		str = strstr(buf,"@R") ;
		if(str==NULL)
			return -1 ;
		char *st ;
		if((st = strstr(str," F1 "))!=NULL){
			get_mac_x(st,0xf1,mac,bx,cy) ;//下面get_mac_x函数获取到mac地址，bx是距离，cy是信号强度（暂时没用）
			return 0xf1 ;
		}else if((st = strstr(str," 1 "))!=NULL){
			get_mac_x(st,1,mac,bx,cy) ;
			return 1 ;
		}else if((st = strstr(str," 0 "))!=NULL){
			get_mac_x(st,0,mac,bx,cy) ;
			return 0 ;
		}
		return -1 ;
	}
	return -1 ;
}

//获取数据中的x,y
int get_mac_x(char *st,int type/*f1,0,1*/,char *mac,int *bx,int *cy) {
	int i=0 ;
	int j=0 ;
	if(type==0xf1){
		i=3 ;
	}else
		i=2 ;
	while(st[i]!='\n'&&st[i]!='\0'){
		if(st[i]==' '){
			i++;
			if(i>80)
				return 11 ;
			continue ;
		}
		while(st[i]!=' '&&st[i]!='\0'){
			mac[j]=st[i] ;
			i++ ;
			j++ ;
			if(i>80)
				return 11 ;
		}
		mac[j]='\0' ;
		break ;
	}
	//bx
	j=0 ;
	while(st[i]!='\n'&&st[i]!='\0'){
		if(st[i]==' '){
			i++;
			if(i>80)
				return 11 ;
			continue ;
		}
		while(st[i]!=' '&&st[i]!='\0'){
			strbx[j]=st[i] ;
			i++ ;
			j++ ;
			if(i>80)
				return 11 ;
		}
		strbx[j]='\0' ;
		break ;
	}
	*bx = ((int)(atof(strbx)*1000)+1)/10 ;
	//cy
	j=0 ;
	while(st[i]!='\n'&&st[i]!='\0'){
		if(st[i]==' '){
			i++;
			if(i>80)
				return 11 ;
			continue ;
		}
		while(st[i]!=' '&&st[i]!='\r'&&st[i]!='\n'&&st[i]!='\0'){
			strcy[j]=st[i] ;
			i++ ;
			j++ ;
			if(i>80)
				return 11 ;
		}
		strcy[j]='\0' ;
		break ;
	}
	*cy = ((int)(atof(strcy)*1000)+1)/10 ;
	return type ;
}



int read_file_info() {
	read_data() ;
	point_num_current = 0 ;
	return 0 ;
}

//把读到的数据放到inputdata中
int set_info(int type,char *mac,float bx,int *out) {
	int i = 0,ret=0 ;
	int cx = 2;//2 ;
//	printf("type %d\n",type) ;
	output[2] = 2.0 ;
	if(type == 0xf1){
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
		}
		if(point_num_current>=3){
			if((ret=get_postionxyz(inputData,output,point_num_current,cx))==0){
				*out = 0xff ;
			}else{
				printf("error\n") ;
			}
		}
		point_num_current = 0 ;
		return 0 ;
	}
	else if(type == 1){
//		printf("-----------------------------------------------%d\n",point_num_current) ;
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
		}
		//每次都先判断一下当前记录的点数有没有大于等于3
		if(point_num_current>=3){
//			point_num_current = 3 ;
			if((ret=get_postionxyz(inputData,output,point_num_current,cx))==0){
				*out = 0xff ;
			}else{
				printf("error\n") ;
			}
		}
		point_num_current = 0 ;
		for(i=0;i<point_num_all;i++){//遍历所有基站点记录设置input函数
				if(!memcmp(mac,gpoint[i].mac,8)){
					inputData[point_num_current][0] = gpoint[i].x ;
					inputData[point_num_current][1] = gpoint[i].y ;
					inputData[point_num_current][2] = gpoint[i].z ;
					inputData[point_num_current][3] = bx ;
					point_num_current++ ;
					break ;
				}
		}
	}
	else if(type == 0){
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
		}
		for(i=0;i<point_num_all;i++){
			if(!memcmp(mac,gpoint[i].mac,8)){
				inputData[point_num_current][0] = gpoint[i].x ;
				inputData[point_num_current][1] = gpoint[i].y ;
				inputData[point_num_current][2] = gpoint[i].z ;
				inputData[point_num_current][3] = bx ;
				point_num_current++ ;
//				printf("point_num_current %d \n",point_num_current) ;
				break ;
			}
		}
		if(point_num_current>3){
			if((ret=get_postionxyz(inputData,output,point_num_current,cx))==0){
				*out = 0xff ;
			}else {
				printf("error\n") ;
			}
			point_num_current = 0 ;
		}
	}
	return -1 ;
}
int init_position() {
//    int ret = 0;
	std::cout<<"try to"<<std::endl;
	char bb[200] ={0};
	printf("bb success\n");
    location_lib = NULL;
	std::cout<<"try to open dl"<<endl;
    location_lib = dlopen(LOCATION_LIB, RTLD_NOW);
	std::cout<<"dlopen success"<<endl;
    if (!location_lib) {

        sprintf(bb,"%s ----------open lib:%s failed for (%s)\n", __func__, LOCATION_LIB, dlerror());
		std::cout<<"dlopen fail"<<endl;
//        com_send(bb,strlen(bb)) ;
        printf(bb);
        exit(0) ;
        return -1;
    }

    get_position =(int (*)(float (*)[4], float*, int, int))dlsym(location_lib, "GetPosition");//(获取链接库中的函数地址)功能是根据动态链接库操作句柄与符号，返回符号对应的地址，不但可以获取函数地址，也可以获取变量地址
    if (!get_position) {
		std::cout<<"getposition fail"<<endl;
        sprintf(bb,"%s ----------dlsym \"GetPosition\" failed for (%s)\n", __func__, dlerror());
        dlclose(location_lib);
//        com_send(bb,strlen(bb)) ;
        exit(0) ;
        return -1;
    }
    return 1 ;
}
int get_postionxyz(float input[][4],float output[3],int n,int c) {
	int ret ;
	ret = get_position(input, output, n, c);
	return ret ;
}
int close_position() {
	dlclose(location_lib) ;
	return 0 ;
}
//MAC1=33254040,1,126,100,110
#define STRLEN 300
char mac[20] ;
char id[10] ;
char x[10] ;
char y[10] ;
char z[10] ;

//mac id xyz清零
void init_xyz(){
	memset(mac,'\0',20) ;
	memset(id,'\0',10) ;
	memset(x,'\0',10) ;
	memset(y,'\0',10) ;
	memset(z,'\0',10) ;
}
//读取配置文件中基站的配置数据
int read_data() {
	FILE *file ;
	char *name ="[DATA]";
	char *key = "NUM";
	file = fopen("./ini/data.ini","rb") ;
    //data.ini里面是基站的MAC地址和位置信息，不知道1,2是什么意思

	if(file == NULL)
		return -1 ;
	char buf[1024] ;
	char str[STRLEN] ;
	memset(buf,'\0',1024) ;
	while(fgets(buf,1024,file)){
		if(!memcmp(buf,name,6)){
			break ;
		}
		memset(buf,'\0',1024) ;
	}
	memset(buf,'\0',1024) ;
	while(fgets(buf,124,file)){
		if(!memcmp(buf,key,3)){
			break ;
		}
		memset(buf,'\0',1024) ;
	}
	int num ;
	int i=0,j=0 ;
	i=4 ;
	memset(str,'\0',STRLEN) ;
	j=0 ;
	while(buf[i]!='\n'){
		str[j] = buf[i] ;
		i++,j++ ;
	}
	point_num_all = atoi(str) ;

/*
 * maxX,maxY,maxZ
 */
	memset(buf,'\0',1024) ;
	init_xyz() ;
	if(fgets(buf,124,file)){//X
		i = 0 ;
		while(buf[i]!='\n'){
			if(buf[i]==':'){
				i++ ;
				break ;
			}
			i++ ;
		}
		j=0;
		while(buf[i]!='\n'){
			x[j]=buf[i];
			i++ ,j++ ;
		}
		maxX = atoi(x) ;
	}

	memset(buf,'\0',1024) ;
	if(fgets(buf,124,file)){//Y
		i = 0 ;
		while(buf[i]!='\n'){
			if(buf[i]==':'){
				i++ ;
				break ;
			}
			i++ ;
		}
		j=0;
		while(buf[i]!='\n'){
			y[j]=buf[i];
			i++ ,j++ ;
		}
		maxY = atoi(y) ;
	}

	memset(buf,'\0',1024) ;
	if(fgets(buf,124,file)){//Z
		i = 0 ;
		while(buf[i]!='\n'){
			if(buf[i]==':'){
				i++ ;
				break ;
			}
			i++ ;
		}
		j=0;
		while(buf[i]!='\n'){
			z[j]=buf[i];
			i++ ,j++ ;
		}
		maxZ = atoi(z) ;
	}
	int no=0 ;
	while(fgets(buf,124,file)){
		if(!memcmp(buf,"MAC",3)){
			init_xyz() ;
			//mac
			i=4 ;

			while(buf[i]!='\n'){
				if(buf[i]=='='){
					i++ ;
					break ;
				}
				i++ ;
			}
			j=0 ;
			while(buf[i]!=','){
				mac[j]=buf[i];
				i++ ,j++ ;
			}

			i++ ;
			//id
			j=0;
			while(buf[i]!=','){
				id[j]=buf[i];
				i++ ,j++ ;
			}
			i++ ;
			//x
			j=0 ;
			while(buf[i]!=','){
				x[j]=buf[i];
				i++ ,j++ ;
			}
			i++ ;
			//y
			j=0 ;
			while(buf[i]!=','){
				y[j]=buf[i];
				i++ ,j++ ;
			}
			i++ ;
			//z
			j=0 ;
			while(buf[i]!='\n'&&buf[i]!='\0'){
				z[j]=buf[i];
				i++ ,j++ ;
			}
			memcpy(gpoint[no].mac,mac,9) ;
			gpoint[no].id= atoi(id) ;
			gpoint[no].x = atoi(x)/100.0 ;
			gpoint[no].y = atoi(y)/100.0 ;
			gpoint[no].z = atoi(z)/100.0 ;
			gpoint[no].xx = atoi(x) ;
			gpoint[no].yy = atoi(y) ;
			gpoint[no].zz = atoi(z) ;
			no++ ;
		}
		memset(buf,'\0',1024) ;
	}

	fclose(file) ;
	return 0 ;
}


int read_ip_port() {
	FILE *file ;
	file = fopen("./ini/ip.ini","rb") ;
	if(file == NULL)
		return -1 ;
	char buf[50] ;
	char str[STRLEN] ;
	memset(str,'\0',STRLEN);
	memset(buf,'\0',50) ;

	if(fgets(buf,50,file)){
		memcpy(pt_ip,buf,strlen(buf));
		if(pt_ip[strlen(pt_ip)-1]=='\n')
			pt_ip[strlen(pt_ip)-1]='\0';
	}else
		return -1 ;
	memset(buf,'\0',50) ;
	if(fgets(buf,50,file)){
		pt_port = atoi(buf) ;
	}else
		return -1 ;
	return 0 ;
}


//int now_num = 0;
//short ax=0,ay=0,az=0 ;
//short avg_num[10]={0,0,0} ;
//int last_xyz(short x,short y,short z,short dix) {
//	int i=0 ;
//	now_num++ ;
//	ax = 0 ;
//	ay = 0 ;
//	az = 0 ;
//	int cur_num = 0 ;
//	if(last_num<=9){
//		last_point[last_num][0] = x ;
//		last_point[last_num][1] = y ;
//		last_point[last_num][2] = z ;
//		last_num++ ;
//		return 0 ;
//	}else{
//		get_maxi(maxi) ;
//		get_mini(mini) ;
//		get_anum(avg_num) ;
//		for(i=0;i<10 ;i++){
//			ax += last_point[i][0] ;
//			ay += last_point[i][1] ;
//			az += last_point[i][2] ;
//			cur_num++ ;
//		}
//		for(i=0;i<avg_num[0];i++){
//			ax -= last_point[avg_num[i+1]][0] ;
//			ay -= last_point[avg_num[i+1]][1] ;
//			az -= last_point[avg_num[i+1]][2] ;
//			cur_num-- ;
//		}
//
//		ax = ax/cur_num ;
//		ay = ay/cur_num ;
//		az = az/cur_num ;
//			last_point[now_num%10][0]=x ;
//			last_point[now_num%10][1]=y ;
//			last_point[now_num%10][2]=z ;
//		if(abs(ax-x)<dix&&abs(ay-y)<dix){
//			return 1;
//		}else
//			last_point[now_num%10][0]=ax ;
//			last_point[now_num%10][1]=ay ;
//			last_point[now_num%10][2]=az ;
//			return 3 ;
//	}
//	return 0 ;
//}



//int get_maxi(int maxi[]) {
//	int i=0 ,x1,y1 ;
//	int x2=0,y2=0 ;
//	x1 = last_point[0][0] ;
//	y1 = last_point[0][1] ;
//	for(i=1;i<10;i++){
//		if(x1<last_point[i][0])
//			x2 = i ;
//		if(y1<last_point[i][1])
//			y2 = i ;
//	}
//	maxi[0]=x2 ;
//	maxi[1]=y2 ;
//	return 1 ;
//}
//
//
//int get_mini(int mini[]) {
//	int i=0 ,x1,y1 ;
//	int x2=0,y2=0 ;
//	x1 = last_point[0][0] ;
//	y1 = last_point[0][1] ;
//	for(i=1;i<10;i++){
//		if(x1>last_point[i][0])
//			x2 = i ;
//		if(y1>last_point[i][1])
//			y2 = i ;
//	}
//	mini[0]=x2 ;
//	mini[1]=y2 ;
//	return 1 ;
//}
//int set_xyz(short x,short avgx,short y,short avgy,short z) {
//	int l=0,m=0;
//	l = x-avgx ;
//	m = y-avgy ;
//	if(abs(l)>abs(m)){
//		if(l>0){
//			last_point[mini[0]][0] = x ;
//			last_point[mini[0]][1] = y ;
//		}else{
//			last_point[maxi[0]][0] = x ;
//			last_point[maxi[0]][1] = y ;
//		}
//		return 1 ;
//	}else{
//		if(m>0){
//			last_point[mini[1]][0] = x ;
//			last_point[mini[1]][1] = y ;
//		}else{
//			last_point[maxi[1]][0] = x ;
//			last_point[maxi[1]][1] = y ;
//		}
//		return 2 ;
//	}
//	return 0 ;
//}
//int get_anum(short num[]) {
//	int i=0,j=0 ;
//	i = 1;
//	num[i] = maxi[0] ;
//	if(num[i]!=maxi[1]){
//		i++ ;
//		num[i] = maxi[1] ;
//	}
//	for(j=1;j<=i;j++){
//		if(num[j]==mini[0]){
//			break ;
//		}
//	}
//	if(i<j){
//		i++ ;
//		num[i] = mini[0] ;
//	}
//	for(j=1 ;j<=i;j++){
//		if(num[j]==mini[1]){
//			break ;
//		}
//	}
//	if(i<j){
//		i++ ;
//		num[i]=mini[1] ;
//	}
//	num[0] = i ;
//	return i ;
//}

int set_one_dimension(int type,char *mac,float bx,int *out)  {
//	printf("----------------type=%d,%d,one_point %d\n",type,point_num_current,one_point_num) ;
	int i = 0,ret=0 ;
	int cx = 11;//2 ;
	output[2] = 2.0 ;
	if(type == 0xf1){
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
		if(point_num_current+one_point_num>=2){
			if((ret=get_postionxyz(inputData,output,point_num_current,cx))==0){
				*out = 0xff ;
			}
		}
		point_num_current = 0 ;
		one_point_num = 0 ;
		return 0 ;
	}else if(type == 1){
//		printf("-----------------------------------------------%d\n",point_num_current) ;
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
		if(point_num_current+one_point_num>=2){
//			point_num_current = 3 ;
			if((ret=get_postionxyz(inputData,output,point_num_current,cx))==0){
				*out = 0xff ;
			}
		}
		point_num_current = 0 ;
		one_point_num = 0 ;
		for(i=0;i<point_num_all;i++){
	//		printf("gpoint id %d\n",gpoint[i].id) ;
			if(gpoint[i].id != 2){
				if(!memcmp(mac,gpoint[i].mac,8)){
					if(gpoint[i].id == 1){
						inputData[point_num_current][0] = gpoint[i].x ;
						inputData[point_num_current][1] = gpoint[i].y ;
						inputData[point_num_current][2] = gpoint[i].z ;
						inputData[point_num_current][3] = bx ;
						point_num_current++ ;
//						printf("point_num_current %d-----------\n",point_num_current) ;
						break ;
					}else if(gpoint[i].id == 3){
						one_inputData[10][0] = gpoint[i].xx ;
						one_inputData[10][1] = gpoint[i].yy ;
						one_inputData[10][2] = gpoint[i].zz ;
						one_inputData[10][3] = bx ;
						one_point_num = 1 ;
	//					printf("one_point_num %d-----------\n",one_point_num) ;
						break ;
					}
				}
			}
		}
	}else if(type == 0){
		if(point_num_current>20&&point_num_current<0){
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
		for(i=0;i<point_num_all;i++){
	//		printf("gpoint id %d\n",gpoint[i].id) ;
			if(gpoint[i].id != 2){
				if(!memcmp(mac,gpoint[i].mac,8)){
					if(gpoint[i].id == 1){
						inputData[point_num_current][0] = gpoint[i].x ;
						inputData[point_num_current][1] = gpoint[i].y ;
						inputData[point_num_current][2] = gpoint[i].z ;
						inputData[point_num_current][3] = bx ;
						point_num_current++ ;
	//					printf("point_num_current %d-----------\n",point_num_current) ;
						break ;
					}else if(gpoint[i].id == 3){
						one_inputData[10][0] = gpoint[i].xx ;
						one_inputData[10][1] = gpoint[i].yy ;
						one_inputData[10][2] = gpoint[i].zz ;
						one_inputData[10][3] = bx ;
						one_point_num = 1 ;
	//					printf("one_point_num %d-----------\n",one_point_num) ;
						break ;
					}
				}
			}
		}
		if(point_num_current>=3){
//			printf("--#############---%d,%d\n",point_num_current,one_point_num) ;
			if((ret=get_postionxyz(inputData,output,point_num_current,cx))==0){
				*out = 0xff ;
			}
//			printf("ret %d\n",ret) ;
			point_num_current = 0 ;
			one_point_num = 0 ;
		}
	}
	return -1 ;
}
