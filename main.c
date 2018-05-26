#include "httpd.h"

void usage(const char* pro)
{
	printf("PLEASE ENTER# %s [local_ip]:[local_port]\n",pro);
}

int main(int argc,char* argv[])
{
    if(argc!=3){
        usage(argv[0]);
        print_log("no ip and port",FATAL);
    }
	int listen_sock=startup(argv[1],argv[2]);

	struct sockaddr_in client;
	socklen_t len=sizeof(client);
    
    if(daemon(1,1)<0){
        print_log(strerror(errno),FATAL);
        return 5;
    }
    pid_t pid=fork();
    if(pid<0){
        print_log(strerror(errno),FATAL);
        return 6;
    }else if (pid>0){
        exit(0);
    }else{
        pid_t ppid=fork();
        if(ppid<0){
            print_log(strerror(errno),FATAL);
            return 6;
        }else if(ppid>0){
            exit(0);
        }else{
            if(setsid()<0){
                print_log(strerror(errno),FATAL);
                return -2;
            }
            if(chdir("/home/nick/museum_server/")<0){
                print_log(strerror(errno),FATAL);
                return -3;
            }
            umask(0);
            close(0);
            close(1);
            close(2);
            open("/dev/null",O_RDONLY);
            open("/dev/null",O_RDWR);
            open("/dev/null",O_RDWR);



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
        }
    }
    return 0;
}
