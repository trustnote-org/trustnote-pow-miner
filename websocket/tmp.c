#include "webservice.h"
#include <stdio.h>
#include <pthread.h>

int main(){

	pthread_mutex_t lock;
	pthread_mutex_init(&lock, NULL);
	char buff[2048];
	ws_service((void*)&lock, buff);
	//aaa( 1 );
}

