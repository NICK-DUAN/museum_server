#include "sql_api.h"
#include <unistd.h>

void check_database(char* account, char* pwd)
{
    SQL s("127.0.0.1",3306,"root","123456","data");
    s.connect();
    int ret = s.login(account, pwd);
    if(ret == 0){
    cout<<"<html><center><meta charset=\"utf-8\">";
    cout<<"<body>";
    cout<<"<div id=\"Layer1\" style=\"position:absolute; width:100%; height:100%; z-index:-1\">";
    cout<<"<img src=\"museum.jpg\" height=\"100%\" width=\"100%\"/>";
    cout<<"</div>";
    cout<<"<h1>博物馆环境检测系统</h1>";
    cout<<"<form>";
    cout<<"博物馆1<input type=\"radio\" name=\"museum_choice\" value=\"1\" checked=\"checked\">";
    cout<<"<br/>博物馆2<input type=\"radio\" name=\"museum_choice\" value=\"2\"><br/>";
    cout<<"博物馆3<input type=\"radio\"name=\"museum_choice\" value=\"3\"><br/>";
    cout<<"<p align=\"center\"><a href=\"../work.html\"><input type=\"button\" value=\"确认选择\"></a></p>";
    cout<<"</form></body></center></html>";
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
