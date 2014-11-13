#include <php.h>
#include <TSRM.h>
#include "scheme.h"
#include "export_func.h"
TSRMLS_FETCH();

//--导出函数入口
zend_function_entry iflytek_funcs_entry[] = {
	ZEND_FE(iflytek_ita, NULL)
	ZEND_FE(iflytek_tts, NULL)
	{NULL,NULL,NULL}
};


//--扩展程序模块入口
zend_module_entry iflytek_funcs_module_entry = {
	STANDARD_MODULE_HEADER,
	"iflytek_funcs",
	iflytek_funcs_entry,
	NULL,	// module_startup_func
	NULL,	// module_shutdown_func
	NULL,			// request_startup_func
	NULL,			// request_shutdown_func
	NULL,			// info_func
	NULL,			// version
	STANDARD_MODULE_PROPERTIES	
};


ZEND_GET_MODULE(iflytek_funcs);