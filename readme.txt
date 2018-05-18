SIMENS_Airport
无人机程序结构
一、analy.cpp/.h 功能：1、读取信标数据并解析，并以二维或者一维的方式放入   								inputdata[ ]数组中。
						2、读取基站配置文件信息
						3、读取ip设置信息
二、create_graph.cpp/.h   	读取厂区图配置信息，并根据节点制成二维数组							的图
三、generate_pos.cpp/.h 定义了控制整个流程的command_thread()函数，	
							定义了靠图像微调控制的线程函数。
三、generate_route.cpp/.h 根据起点和终点和图生成最短路径
四、get_light.cpp/.h 声明和定义了图像处理定位灯管用到的函数。
五 、run_light.cpp/.h 定义了图像处理线程函数run_light()和run_light里再						开一个处理红色的线程。
六、iotfly.cpp  main函数入口
七、send_command.cpp/.h 记录了指令，定义了一些发指令的函数
八、Serial_RT.cpp/.h我写的串口通信用的函数，用来给无人机发命令。
九、serial_usb_com.cpp/.h 侯写的串口通信函数接收uwb和给uwb发切换							   分组命令（注意有时间间隔要求）。
		



get_info（） 解析uwb数据
		get_max_x() 获取数据中的xy
		read_file_info()调用readdata（）函数，读取配置文件配置基站数据
		set_info()用二维的方式把读到的数据放到inputdata[ ]中
		set_one_dimension()用一维的方式把读到的数据放到inputdata中
		init_position()打开计算定位的.so库
		get_position()  .so文件里的函数用指针去调用
