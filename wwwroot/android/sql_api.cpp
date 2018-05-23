#include "sql_api.h"
#include <iomanip>
#include <stdio.h>
SQL::SQL(const string &host,const int& port,const string& user,const string& password,const string& database)
{
    _host=host;
    _user=user;
    _password=password;
    _database=database;
    _port=port;
    _conn=mysql_init(NULL);
}

int SQL::connect()
{
	int ret=0;
    if(mysql_real_connect(_conn,_host.c_str(),_user.c_str(),_password.c_str(),_database.c_str(),_port,NULL,0)){
    }else{
		ret=-1;
        cout<<"connect failed"<<endl;
    }
	return ret;
}

int SQL::insert(const string& temperature, const string& light, const string& water)
{
	int ret=0;
    string INSERT="insert into museum(temperature,light,water) values('";
    INSERT+=temperature;
    INSERT+="','";
    INSERT+=light;
    INSERT+="','";
    INSERT+=water;
    INSERT+="')";
    cout<<"<html><center><body background=\"museum.jpg\"><h4>"<<INSERT<<"</h4>";
    int r=mysql_query(_conn,INSERT.c_str());
    if(r!=0){
        cout<<"<h5>insert failed</h5></html>"<<endl;
		ret=-1;
    }else{
		cout<<"<h5>insert success!</h5></center></body></html>"<<endl;
	}
	return ret;
}

string formatstr(string str){
    if(str.length() >= 15){
        return str;
    }
    while(str.length() < 15){
        str += " ";
    }
    return str;
}

int SQL::show()
{
	int ret=0;
	string show="select * from museum order by createtime desc";
	if(mysql_query(_conn,show.c_str())==0){
		res=mysql_store_result(_conn);
		if(res){
			int col =mysql_num_fields(res);
			MYSQL_FIELD *fd;
			int i=0;
			int j=0;
				MYSQL_ROW row=mysql_fetch_row(res);
				j=0;
				for(j=1;j<col-1;j++){
                    cout<<formatstr(row[j])<<",";
				}
		}else{
			ret=-1;
		}
	}else{
		ret=-2;
	}
	return ret;
}

int SQL::Register(const string& account, const string& pwd){
	int ret = 0;
	string reg="insert into museum_user(account,password) values('";
	reg+=account;
    reg+="','";
    reg+=pwd;
    reg+="')";
    int r=mysql_query(_conn, reg.c_str());
    if(r!=0){
		ret=-1;
        cout<<"1";
    }else{
        cout<<"0";
	}
}

int SQL::login(const string& account, const string& pwd){
	int ret = 0;
	string select="select password from museum_user where account=\"";
	select+=account;
    select+="\"";
    if(mysql_query(_conn,select.c_str())==0){
		res=mysql_store_result(_conn);
		if(res){
			int col=mysql_num_fields(res);
			MYSQL_ROW msg=mysql_fetch_row(res);
			if(msg){
                if(pwd == msg[0]){
                    cout<<"0";
                }else{
                    cout<<"1";
                }
            }else{
                cout<<"2";
            }
        }else{
            cout<<"2";
        }
    }else{
        cout<<"2";
    }
}

SQL::~SQL()
{
    mysql_close(_conn);
}
