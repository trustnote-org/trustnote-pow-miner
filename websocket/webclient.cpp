#include "webservice.h"
#include "easywsclient.hpp"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <string>


using easywsclient::WebSocket;


// 挖矿结果发送至主链服务端
bool json_ws_call(const char* url, const char *rpc_req)
{

	WebSocket::pointer ws = WebSocket::from_url( url );
	if(!ws){ return NULL; }

	ws->send( rpc_req );
	std::string message;
	while (ws->getReadyState() != WebSocket::CLOSED) {
        bool gotMessage = false;
        ws->poll();
        ws->dispatch([gotMessageOut=&gotMessage, messageOut=&message, ws=ws](const std::string& message) {
            *gotMessageOut = true;
            *messageOut = message;
        });
        if (gotMessage) {
            break;
        }
		ws->close();
    }
	printf(">>> RECEIVED %s\n", message.c_str());

	return true;
}


