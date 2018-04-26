#include "sql_api.h"
#include <unistd.h>

void check_database(char* account, char* pwd)
{
    SQL s("127.0.0.1",3306,"root","123456","data");
    s.connect();
    int ret = s.login(account, pwd);
    if(ret == 0){
        cout<<"<html><center><meta charset=\"UTF-8\">";
        cout<<"<body background=\"museum.jpg\">";
        cout<<"<h1>博物馆环境检测系统</h1>";
        cout<<"<form action=\"./insert_cgi\">";
        cout<<"温度";
        cout<<"<input type=\"text\" name=\"wendu\" value=\"\"><br><br>";
        cout<<"光照";
        cout<<"<input type=\"text\" name=\"guangzhao\" value=\"\"><br><br>";
        cout<<"湿度";
        cout<<"<input type=\"text\" name=\"shidu\" value=\"\"><br><br>";
        cout<<"<br>";
        cout<<"<input type=\"submit\" value=\"插入\">";
        cout<<"</form><form action=\"./show_cgi\">";
        cout<<"<input type=\"submit\" value=\"显示\"></form>";
        cout<<"</center></body></html>";
    }else if(ret == -1){
        cout<<"<html><meta charset=\"utf-8\"><body background=\"museum.jpg\"><center><h3>503 server busy, please try later</h3>";
        //cout<<"<p align=\"center\"><a href=\"../reg_log.html\">重新登录</a></p>";
        cout<<"<p align=\"center\"><a href=\"../register.html\"><input type=\"button\" value=\"去注册\"></a></p>";
        cout<<"<p align=\"center\"><a href=\"../reg_log.html\"><input type=\"button\" value=\"重新登录\"></a></p>";
        cout<<"</center></body></html>";

    }else if(ret == -2){
        cout<<"<html><meta charset=\"utf-8\"><body background=\"museum.jpg\"><center><h3>please check your password</h3>";
        //cout<<"<p align=\"center\"><a href=\"../reg_log.html\"><input type=\"button\" value=\"重新登录\"></a></p>";
        cout<<"<p align=\"center\"><a href=\"../register.html\"><input type=\"button\" value=\"去注册\"></a></p>";
        cout<<"<p align=\"center\"><a href=\"../reg_log.html\"><input type=\"button\" value=\"重新登录\"></a></p>";
        cout<<"</center></body></html>";
    }
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

	check_database(data[0],data[1]);
	
    return 0;
}
