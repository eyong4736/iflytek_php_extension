#include <php.h>
#include <TSRM.h>
#include "scheme.h"
#include "export_func.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <errno.h>

#include "../include/qisr.h"
#include "../include/msp_cmn.h"
#include "../include/msp_errors.h"

const char* login = "";
const char* passwd = "";

void run_iat(const char* src_wav_filename ,  const char* param, char * result)
{
	char rec_result[1024*4] = {0};
	const char *sessionID;
	FILE *f_pcm = NULL;
	char *pPCM = NULL;
	int lastAudio = 0 ;
	int audStat = 2 ;
	int epStatus = 0;
	int recStatus = 0 ;
	long pcmCount = 0;
	long pcmSize = 0;
	int ret = 0 ;
	
	sessionID = QISRSessionBegin(NULL, param, &ret);
    if (ret !=0)
	{
//	    printf("QISRSessionBegin Failed,ret=%d\n",ret);
	}
	DbgPrint("QISRSessionBegin ,ret=%d\n",ret);
	f_pcm = fopen(src_wav_filename, "rb");
//	DbgPrint("filepath:%s", src_wav_filename);
	DbgPrint("I couldn't open the file [%s], %d|%s \n",src_wav_filename, errno, strerror(errno));
	DbgPrint("f_pcm:%d", f_pcm);
	if (NULL != f_pcm) {
		fseek(f_pcm, 0, SEEK_END);
		pcmSize = ftell(f_pcm);
		fseek(f_pcm, 0, SEEK_SET);
		pPCM = (char *)malloc(pcmSize);
		fread((void *)pPCM, pcmSize, 1, f_pcm);
		fclose(f_pcm);
		f_pcm = NULL;
	}else{
		DbgPrint("f_pcm is null");
		return ;
	}
	while (1) {
		unsigned int len = 6400;

		if (pcmSize < 12800) {
			len = pcmSize;
			lastAudio = 1;
		}
		audStat = 2;
		if (pcmCount == 0)
			audStat = 1;
		if (0) {
			if (audStat == 1)
				audStat = 5;
			else
				audStat = 4;
		}
		if (len<=0)
		{
			break;
		}
//		printf("csid=%s,count=%d,aus=%d,",sessionID,pcmCount/len,audStat);
		ret = QISRAudioWrite(sessionID, (const void *)&pPCM[pcmCount], len, audStat, &epStatus, &recStatus);
//		printf("eps=%d,rss=%d,ret=%d\n",epStatus,recStatus,ret);
		if (ret != 0)
			break;
		pcmCount += (long)len;
		pcmSize -= (long)len;
		if (recStatus == 0) {
			const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &ret);
			if (ret != MSP_SUCCESS)
			{
	//			printf("QISRGetResult Failed,ret=%d\n",ret);
				break;
			}
			DbgPrint("QISRGetResult,ret=%d\n",ret);
			
			if (NULL != rslt)
				strcat(result, rslt);
			DbgPrint("%s\n", rslt);
		}
		if (epStatus == MSP_EP_AFTER_SPEECH)
			break;
		usleep(150000);
	}
	ret=QISRAudioWrite(sessionID, (const void *)NULL, 0, 4, &epStatus, &recStatus);
	if (ret !=0)
	{
//		printf("QISRAudioWrite Failed,ret=%d\n",ret);
	}
	DbgPrint("QISRAudioWrite ,ret=%d\n",ret);
	free(pPCM);
	pPCM = NULL;
	while (recStatus != 5 && ret == 0) {
		const char *rslt = QISRGetResult(sessionID, &recStatus, 0, &ret);
		if (NULL != rslt)
		{
			strcat(result,rslt);
		}
		usleep(150000);
	}
    ret=QISRSessionEnd(sessionID, NULL);
	if(ret !=MSP_SUCCESS)
	{
//		printf("QISRSessionEnd Failed, ret=%d\n",ret);
	}
	DbgPrint("QISRSessionEnd , ret=%d\n",ret);

//	printf("=============================================================\n");
//	printf("The result is: %s\n",result);
	DbgPrint("The result is: %s\n",result);
//	printf("=============================================================\n");
}

ZEND_FUNCTION(iflytek_ita)
{
	int retval = 0;
	
	const char* result;
	
	char * filePath;
	char trans_result[1024*4] = {0};
	int filePathLen;
	
	retval = zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filePath, &filePathLen);
	if(retval == FAILURE){
//		printf("iflytek: parse parameters fail\n");
		return;
	}
	
	DbgPrint("%s, %d", filePath, filePathLen);
	
	const char* login_configs = "appid = , work_dir =   .  ";
	const char* param1 = "sub=iat,ssm=1,auf=audio/L16;rate=8000,aue=speex,ent=sms8k,rst=plain,rse=utf8";//直接转写，默认编码为gb2312，可以通过rse参数指定为utf8或unicode
	const char* param2 = "sub=iat,ssm=1,auf=audio/L16;rate=8000,aue=speex,ent=sms8k,rst=json,rse=utf8";//转写为json格式，编码只能为utf8
//	const char* output_file = "iat_result.txt";
	int ret = 0;
//	char key = 0;
	
	//用户登录
	ret = MSPLogin(login, passwd, login_configs);
	if ( ret != MSP_SUCCESS )
	{
//		printf("MSPLogin failed , Error code %d.\n",ret);
		return;
	}
	DbgPrint("MSPLogin , Error code %d.\n",ret);
	//开始一路转写会话
	run_iat(filePath,  param1, trans_result);                                     //iflytek09对应的音频内容“沉舟侧畔千帆过，病树前头万木春。”

	//退出登录
    MSPLogout();
	
	RETVAL_STRING(trans_result, 1);
	
	return;
}

