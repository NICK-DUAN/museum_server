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
			int nums=mysql_num_rows(res);
			int col =mysql_num_fields(res);
			MYSQL_FIELD *fd;
            for(;fd=mysql_fetch_field(res);){
                cout<<formatstr(fd->name)<<"   ";
			}
            cout<<endl<<endl;
			int i=0;
			int j=0;
			for(;i<nums;i++){
				MYSQL_ROW row=mysql_fetch_row(res);
				j=0;
				for(;j<col;j++){
                    cout<<formatstr(row[j])<<"   ";
				}
                cout<<endl<<endl;
			}
		}else{
			ret=-1;
		}
	}else{
		ret=-2;
	}
	return ret;
}

int SQL::select(const string &id)
{
	int ret=0;
	string select="select * from museum where id in(";
	select+=id;
	select+=")";
	if(mysql_query(_conn,select.c_str())==0){
		res=mysql_store_result(_conn);
		if(res){
			int col=mysql_num_fields(res);
			MYSQL_FIELD *fd;
			for(;fd=mysql_fetch_field(res);){
				cout<<fd->name<<" ";
			}
			cout<<endl<<endl;
			MYSQL_ROW msg=mysql_fetch_row(res);
			if(msg){
				int i=0;
				for(i=0;i<col;i++){
					cout<<msg[i]<<"   ";
				}
				cout<<endl<<endl;
			}else{
				ret=-1;
				cout<<"Please Make Sure You Entered A Correct ID"<<endl;
		        cout<<"OR This Table Doesn't Include This Message!'"<<endl;
			}
		}
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
    cout<<"<html><center><meta charset=\"utf-8\"><body background=\"museum.jpg\">";
    int r=mysql_query(_conn, reg.c_str());
    if(r!=0){
        cout<<"<h2>register failed</h2><br>";
        cout<<"<p align=\"center\"><a href=\"../register.html\"><input type=\"button\" value=\"重新注册\"></a></p>";
        cout<<"</center></body></html>";
		ret=-1;
    }else{
		cout<<"<h5>register success!</h5>";
        cout<<"<p align=\"center\"><a href=\"../reg_log.html\"><input type=\"button\" value=\"返回登录界面\"></a></p>";
        cout<<"</center></body></html>";
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
                if(pwd != msg[0]){
                    ret = -2;
                }
            }else{
                ret = -1;
            }
		}
    }else{
        ret = -1;
    }
	return ret;
}

int SQL::Delete(const string& id)
{
	int ret=0;
	if(select(id)<0){
		ret=-1;
	}
	string Delete="delete from museum where id='";
	Delete+=id;
	Delete+="'";
	cout<<Delete<<endl;
	int r=mysql_query(_conn,Delete.c_str());
    //int r=mysql_query(_conn,INSERT.c_str());
	if(r!=0){
		ret=-1;
		cerr<<"Delete Failed"<<endl;
	}else{
		cout<<"delete success!"<<endl;
	}
	return ret;
}

SQL::~SQL()
{
    mysql_close(_conn);
}
