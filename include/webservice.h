#ifndef __WEB_SERVICE
#define __WEB_SERVICE
#include <pthread.h>

#ifdef __cplusplus 
extern "C" { 
#endif


// 挖矿结果发送至主链服务端
//bool json_ws_call(const char* url, const char *rpc_req);

void ws_service(void *ws_lock, char *payload);

// 挖矿结果发送至主链服务端
bool json_ws_call(const char* url, const char *rpc_req);

#ifdef __cplusplus 
} 
#endif

#endif

