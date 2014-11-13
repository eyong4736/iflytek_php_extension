#ifndef __SCHEME_H__
#define __SCHEME_H__
 
//#define _XOPEN_SOURCE 600 // -- 关键定义
// -- >>>>>>>>>>>>>>> 体系结构部分的配置

#define __LINUX__

// -- #include "sheme_port.h"
// -- #error hello world
#include<stdlib.h>                                                                                         
#include<stdio.h>                                                                                          
#include<sys/types.h>                                                                                      
#include<sys/sysinfo.h>                                                                                    
#include <unistd.h>                                                                                         
                                                                                     
#include <sched.h>                                                                                          
#include <ctype.h>                                                                                          
#include <string.h>   
#include <sys/time.h>
#include <ctype.h>                                                                                          
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/if_ether.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// -- #include <linux/in.h>
 
#include <errno.h>
 
#include <pthread.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define DEBUG_PHP_EXT   1
#define DEBUG
#define DEBUG_APP    1

#ifdef DEBUG_PHP_EXT
#define DbgPrint(fmt, arg...)                                   \
 do {                                                            \
	char file_path_str[1024] = {0};							\
	FILE *fp;												\
	time_t t = time(NULL);                                  \
	struct tm * now = localtime(&t);                        \
	printf("[%.4d-%.2d-%.2d %.2d:%.2d:%.2d]",               \
	1900 + now->tm_year,                                    \
	now->tm_mon+1,                                          \
	now->tm_mday,                                           \
	now->tm_hour,                                           \
	now->tm_min,                                            \
	now->tm_sec);                                           \
	if((fp=fopen("/home/extension_logs/iflytek_funcs.txt","at+"))==NULL) \
	{ \
		printf("Cannot open file strike any key exit!");	\
		exit(1);											\
	}														\
	sprintf(file_path_str, "["__FILE__":%d] \n" fmt "\n", __LINE__, ##arg);        \
	fputs(file_path_str,fp);										\
	fclose(fp);												\
} while (0)
#else
#define DbgPrint printf("\n"); printf
#endif

#endif

