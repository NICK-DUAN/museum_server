#include "httpd.h"

void usage(const char* pro)
{
	printf("PLEASE ENTER# %s [local_ip]:[local_port]\n",pro);
}

int main(int argc,char* argv[])
{
#if 1
	if(argc!=3){
		usage(argv[0]);
	}
#else
	argv[1]="192.168.145.130";
	argv[2]="8080";
#endif

	int listen_sock=startup(argv[1],argv[2]);

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
	while(1){
		int new_sock=accept(listen_sock,(struct sockaddr*)&client,&len);
		if(new_sock<0){
			print_log(strerror(errno),FATAL);
			exit(4);
		}

		printf("get a client:%s--%d\n",inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		pthread_t id;
		int ret=pthread_create(&id,NULL,handler_request,(void*)new_sock);
		if(ret!=0){
			print_log(strerror(errno),FATAL);
			close(new_sock);
		}else{
			pthread_detach(id);
		}
	}

	return 0;
}
