g++ -std=gnu++0x -fPIC -Wall   -c -o ./wsclient/easywsclient.o ./wsclient/easywsclient.cpp
g++ -std=gnu++0x -I. -I./webservice -I./wsclient -g -fPIC -shared -o libwebservice.so wsclient/easywsclient.o webservice.cpp webclient.cpp webservice/base64.cpp webservice/network_interface.cpp  webservice/websocket_handler.cpp  webservice/websocket_respond.cpp webservice/debug_log.cpp  webservice/sha1.cpp webservice/websocket_request.cpp
cp libwebservice.so ~/.so

#g++ -dynamiclib -I. -I./webservice -I./wsclient -g -o libwebservice.so webservice.cpp

#g++ -dynamiclib -o libwebservice.dylib webservice.cpp
