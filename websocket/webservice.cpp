#include <stdio.h>
#include "base64.h"
#include "debug_log.h"
#include "sha1.h"
#include "websocket_handler.h"
#include "websocket_request.h"
#include "websocket_respond.h"
#include "webservice.h"
#include "network_interface.h"




void ws_service(void *ws_lock, char *payload){
	Network_Interface *m_network_interface = Network_Interface::get_share_network_interface();
	m_network_interface->run((pthread_mutex_t *)ws_lock, payload);
}
