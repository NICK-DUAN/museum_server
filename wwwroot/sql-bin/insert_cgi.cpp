#include "sql_api.h"
#include <unistd.h>

void insert_to_database(char* temperature, char* light, char* water)
{
    SQL s("127.0.0.1",3306,"root","123456","data");
    s.connect();
    s.insert(temperature, light, water);
}

int main()
{
	char *query=NULL;
	char *content_length=NULL;
	int content_len=-1;
	char *data[8]={0};

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

	//cout<<"query-------"<<query<<endl;
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

	//for(i=0;data[i];i++){
	//	cout<<"data-------"<<data[i]<<endl;
	//}
	insert_to_database(data[0],data[1],data[2]);
	
    return 0;
}
