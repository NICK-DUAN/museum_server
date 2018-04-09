#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE 1024

void func(int num1,int num2)
{
	printf("num1:%d\nnum2:%d\n",num1,num2);
	printf("%d+%d=%d\n",num1,num2,num1+num2);
	printf("%d-%d=%d\n",num1,num2,num1-num2);
	printf("%d*%d=%d\n",num1,num2,num1*num2);
	printf("%d/%d=%d\n",num1,num2,num1/num2);
	printf("%d%%%d=%d\n",num1,num2,num1%num2);
}

int main()
{
#if 0
    printf("hello,world!\n");
	char *method=getenv("METHOD");
	char *query=getenv("QUERY_STRING");
	int content_length=getenv("CONTENT_LEN");
    printf("method:%s\n",method);
    printf("query:%s\n",query);
    printf("content_len:%d\n",content_length);
#else

	char *query=NULL;
	char *content_length=NULL;
	int content_len=-1;
	char *data[10]={0};

	char *method=getenv("METHOD");
	if(NULL==method){
		exit(1);
	}
	char c;
	if(strcasecmp(method,"GET")==0){//GET 方法
		query=getenv("QUERY_STRING");
		if(NULL==query){
			exit(2);
		}
	}else{//POST方法
		content_length=getenv("CONTENT_LEN");
		if(NULL==content_length){
			exit(3);
		}
		content_len=atoi(content_length);
		int m=0;
		while(content_len--){
			int ret=read(0,&c,1);
			if(ret>0){
				query[m++]=c;
			}
		}
		*query='\0';
	}

    //printf("%s\n",query);
	int i=0;
	int j=0;
	int k=0;

	while (query[i]){
		if (query[i++] == '=' && query[i]){
			if (query[i] != '&' &&query[i])
				data[j] = &query[i];
		}
		if (query[i] == '&'){
			query[i] = '\0';
			i++,j++;
		}
	}

	for(i=0;data[i];i++){
		printf("data:%s\n",data[i]);
	}

	//printf("data:%s-------data:%s\n",data[0],data[1]);
	//printf("data[0]:%d-----------data[1]:%d\n",atoi(data[0]),atoi(data[1]));
		////int num1=atoi(data[0]);
		////int num2=atoi(data[1]);
	//printf("what you input:\nnum1:%d\nnum2%d\n",num1,num2);
	//printf("%d+%d=%d\n",num1,num2,num1+num2);
	//printf("%d-%d=%d\n",num1,num2,num1-num2);
	//printf("%d*%d=%d\n",num1,num2,num1*num2);
	//printf("%d/%d=%d\n",num1,num2,num1/num2);
	//printf("%d%%%d=%d\n",num1,num2,num1%num2);
		////func(num1,num2);
#endif
	return 0;
}
