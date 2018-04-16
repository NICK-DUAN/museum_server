#include "httpd.h"

void echo_string(int sock){//该函数用于当服务器出错时，给客户端返回错误信息
	const char* echo_line="HTTP/1.0 404 NotFound\r\n";//响应行
	send(sock,echo_line,strlen(echo_line),0);
	const char *answer="\r\n";//空行
	send(sock,answer,strlen(answer),0);
	const char* show="Request Wrong,Please Try Again...";//响应正文,即出错信息
	send(sock,show,strlen(show),0);
}

void print_log(char* str,int level)
{
#if 0
	int fd = open("/log/file",O_WRONLY);
	write(fd,strerror,strlen(strerror));
#else
	const char* level_msg[]={//出错等级
		"SUCCESS",
		"NOTICE",
		"WAWRNING",
		"ERROR",
		"FATAL",
	};
	printf("%s--%s\n",str,level_msg[level%5]);//打印错误消息，调试版本，以后可以直接往文件里面打
#endif 
}

int startup(char* ip,char* port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);//创建一个套接字
	if(sock<0){
		print_log(strerror(errno),FATAL);
		exit(1);
	}

	int flag=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));//使该套接字可以服用，不出现timewait

	struct sockaddr_in local;
	socklen_t len=sizeof(local);
	local.sin_family=AF_INET;
	local.sin_port=htons(atoi(port));
	local.sin_addr.s_addr=inet_addr(ip);
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0){//绑定该套接字
		print_log(strerror(errno),FATAL);
		exit(2);
	}

	if(listen(sock,10)<0){//坚挺该套接字
		print_log(strerror(errno),FATAL);
		exit(3);
	}

	return sock;
}

static int get_line(int sock,char line[],int size)//该函数每次读取一行，size为line的大小
{
	char c='\0';
	int total=0;
	int one=-1;
	while(total<size-1 && c!='\n'){//没有读到换行符并当前缓冲区还能存放
		one=recv(sock,&c,1,0);//每次读取一个字符
		if(one>0){//读取成功
			if(c=='\r'){//当读到'\r'时，证明接下来可能会读到'\n'，需要进行判断处理
				int ret=recv(sock,&c,1,MSG_PEEK);//窥探读法，只看下一个字符是什么，而不从缓冲区中读出来
				if(ret>0){
					if(c=='\n'){//读到'\n'时，判断成功，则继续读一个字符，把'\n'读出来
						recv(sock,&c,1,0);
					}else{//没有读到'\n',但是某些平台的'\r'也做'\n'用，所以将当前的'\r'置为'\n'，便于判断
						c='\n';
					}//4fi
				}//3fi
			}//2fi
			// '\n' '\r''\n' '\r'--->'\n'
			line[total++]=c;//如果不是'\r'，则当前字符写入缓冲区中
		}else{//one<=0,证明没有读到数据，表示读到文件结尾，此时将最后一个字符变为'\n'，跳出循环
			c='\n';//没有读成功，可能sock里面没有数据或出错，将当前字符置为'\n'，跳出循环
		}
	}
	line[total]='\0';//最后一个位置设为'\0'
	return total;
}


static int echo_www(int sock,char *path,int size){//该函数用于访问静态网页
	int fd=open(path,O_RDONLY);//打开请求的路径中的网页文件
	if(fd<0){
		printf("fd\n");
		echo_string(sock);
		print_log(strerror(errno),FATAL);
		return 8;
	}
	const char* echo_line="HTTP/1.0 200 OK\r\n";//报头
	send(sock,echo_line,strlen(echo_line),0);
	const char *answer="\r\n";//空行
	send(sock,answer,strlen(answer),0);

	if(sendfile(sock,fd,NULL,size)<0){//将fd文件中的信息写入sock套接字中
		printf("sendfile\n");
		echo_string(sock);
		print_log(strerror(errno),FATAL);
		return 9;
	}
	close(fd);//一次访问完成，关闭套接字
	return 0;
}

static void drop_header(int sock)//该函数用于一次性将头部信息读取完，以便处理后续的正文信息
{
	int ret=-1;
	char line[SIZE];
	do{
		ret=get_line(sock,line,SIZE);
	}while(ret>0 && strcmp(line,"\n"));
}

static int exe_cgi(int sock,char* method,char* path,char* query_string)//该函数用于处理客户端发来的需要运行可执行文件等的请求
{
	char line[SIZE];
	int content_len=-1;
	int ret=-1;
	//进入此函数，则证明是GET方法必有参数，或者是POST方法
	if(strcasecmp(method,"GET")==0){//如果是GET方法
		drop_header(sock);//读完所有的报头信息
	}else{//POST方法
		do{
			ret=get_line(sock,line,SIZE);
			if(ret>0){
				if(strncasecmp(line,"Content-Length ",16)==0){//当报头信息中出现"Contlen-Length "时，其后面的参数表示POST的参数共有多长
					content_len=atoi(&line[16]);//content_len保存POST方法的参数的长度
					drop_header(sock);
				}
				//如果不为"Content-Length "字段的话，则继续读，一直读到空行为止，即读完报头信息
			}
		}while(ret>0 && strcmp(line,"\n"));
		if(content_len==-1){
			printf("content_len\n");
			echo_string(sock);
			return 10;
		}
	}//end of else

	const char* head_line="HTTP/1.0 200 OK\r\n\r\n";
	send(sock,head_line,strlen(head_line),0);//发送响应行和空行

	int input[2];
	int output[2];
	if(pipe(input)!=0){//创建一个管道in
		printf("in\n");
		echo_string(sock);
		return 11;
	}
	if(pipe(output)!=0){//创建一个管道out
		printf("out\n");
		echo_string(sock);
		return 12;
	}
	pid_t id=fork();//fork子进程
	if(id<0){
		printf("fork\n");
		echo_string(sock);
		return 13;
	}else if(id==0){//child
		char method_env[SIZE/10];
		char query_env[SIZE/10];
		char content_env[SIZE/10];

		sprintf(method_env,"METHOD=%s",method);
		putenv(method_env);

		if(strcmp(method,"GET")==0){//如果是GET方法，则将参数设为环境变量
			sprintf(query_env,"QUERY_STRING=%s",query_string);
			putenv(query_env);
		}else{//如果是POST方法，则将POST参数逇长度设为环境变量，便于后续读取与使用
			sprintf(content_env,"CONTENT_LEN=%d",content_len);
			putenv(content_env);
		}

		close(input[1]);//关闭in管道的写端，in管道此时只能读
		close(output[0]);//关闭out管道的读端，out管道此时只能写

		dup2(input[0],0);//重定向子进程的标准输入变为in管道的读端，
		dup2(output[1],1);//重定向子进程的标准输出变为out管道的写端

		//此时子进程读取数据时，会使用input[0]从管道的读端读取————>即从父进程中读取
		//子进程写数据时，会使用output[1]写入管道————>即写入父进程中
        printf("---path:%s---\n",path);
		execl(path,path,NULL);//execl程序替换，相当于执行path路径中的path命令，即执行path路径中的名为"path"的程序
		exit(1);
	}else{//father
		close(input[0]);//父进程关闭in管道的读端，in管道只能写
		close(output[1]);//父进程关闭out管道的写端，out官道只能读
		//如此一来，和子进程刚好错开，避免冲突
		char c;
		if(strcasecmp(method,"POST")==0){//若是POST方法
			int i=0;
			for(i=0;i<content_len;i++){//则从sock中读取content_len个字符，读取到的都为POST的参数
				                       //刚才已经读完所有的报头信息
									   //从请求正文一开始就直接读congtent-len个字符，这些就是参数
				recv(sock,&c,1,0);
				write(input[1],&c,1);//用input写入子进程中，告诉子进程POST的参数
			}
		}

		//char buff[SIZE];
		//ssize_t s=read(output[0],buff,sizeof(buff)-1);
		//if(s>0){
		//	printf("buff:%s\n",buff);
		//}
		while(read(output[0],&c,1)==1){//从子进程中得到子进程执行的结果
			send(sock,&c,1,0);//并写入到sock中，即传给客户端
		}

		waitpid(id,NULL,0);
	}
	
}

void* handler_request(void* arg)
{
	int sock=(int)arg;
#ifdef _DEBUG_
	int ret=0;
	char line[1024];

	do{
		ret=get_line(sock,line,sizeof(line));
		if(ret<0){
			printf("request done...\n");
			break;
		}else{
			printf("%s",line);
		}
	}while(1);
#else
	int cgi=0;
	int ret=0;
	int i=0,j=0;
	char line[SIZE];
	char method[SIZE/10];
	char url[SIZE/10];
	char path[SIZE/10];
	char *query_string=NULL;
	if(get_line(sock,line,sizeof(line))<=0){
		printf("get_line\n");
		echo_string(sock);
		ret=6;//没有读到数据，或当前语法出错,直接退出返回
		goto end;
	}
	while(!isspace(line[i]) && i<SIZE && j<sizeof(method)){//因为HTTP是请求行根据空格分隔的，所以第一个空格前是获取的方法
		method[j]=line[i];
		i++,j++;
	}
	i++;
	method[j]=0;//给保存方法的字符串后面添加'\0'

	if(strcasecmp(method,"GET") && strcasecmp(method,"POST") ){//如果不为这两种方法，则出错，关闭文件描述符
		printf("judge method\n");
		echo_string(sock);
		ret=7;
		goto end;
	}

	if(strcasecmp(method,"POST")==0){//如果是POST方法，则为cgi模式
		cgi=1;
	}

	j=0;
	while(!isspace(line[i]) && i<SIZE){//提取路径
		url[j]=line[i];
		i++,j++;
	}
	url[j]=0;
    printf("url=%s\n",url);
	//若是GET方法的话，提取路径中参数
	i=0,j=0;
	if(strcasecmp(method,"GET")==0){
		query_string=url;
		while(*query_string!='\0'){
			if(*query_string=='?'){
				*query_string='\0';//url为\0前面的字符串
				query_string++;//query_string为\0后面的字符串
				cgi=1;//有参数时为CGI模式
				break;
			}
			query_string++;
		}
	}

	sprintf(path,"wwwroot%s",url);//将真实路径保存在path中
	if(path[strlen(path)-1]=='/'){//如果路径只有一个主目录的话
		strcat(path,"index.html");//添加上HTML文件
	}

	struct stat st;
	printf("path=%s\n",path);
	if(stat(path,&st)!=0){//判断path路径是何种文件出错
		printf("judge document\n");
		echo_string(sock);
		ret=8;
		goto end;
	}else{
		if(S_ISDIR(st.st_mode)){//是一个不带'/'的目录
			strcat(path,"/index.html");
		}
		else if( (st.st_mode & S_IXUSR) || (st.st_mode & S_IXGRP) || (st.st_mode & S_IXOTH)){//是一个可执行程序
			cgi=1;
		}
	}

	if(cgi){
		printf("method:%s-----url:%s-----path:%s-----query_string:%s\n",method,url,path,query_string);
		exe_cgi(sock,method,path,query_string);//请求可执行程序
	}else{
		printf("method:%s-----url:%s-----path:%s-----query_string:%s\n",method,url,path,query_string);
		drop_header(sock);
		echo_www(sock,path,st.st_size);//请求网页信息
	}

#endif
end:
	printf("client quit...\n");
	close(sock);
	return (void*)ret;
}
