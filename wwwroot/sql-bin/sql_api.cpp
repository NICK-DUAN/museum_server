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
int SQL::insert(const string& location, const string& temperature, const string& pm, const string& hunidity, const string& co2, const string fromaldehtde, const string& light)
{
	int ret=0;
    string INSERT="insert into museum(`LOCATION`,`TEMPERATURE`,`PM2.5`,`HUNIDITY`,`CO2`, `FORMALDEHTDE`,`LIGHT`) values('";
    INSERT+="M 1 ";
    INSERT+="R ";
    INSERT+=location;
    INSERT+="','";
    INSERT+=temperature;
    INSERT+="','";
    INSERT+=pm;
    INSERT+="','";
    INSERT+=hunidity;
    INSERT+="','";
    INSERT+=co2;
    INSERT+="','";
    INSERT+=fromaldehtde;
    INSERT+="','";
    INSERT+=light;
    INSERT+="')";
    int r=mysql_query(_conn,INSERT.c_str());
    cout<<"<html><meta charset=\"utf-8\"><center><body>";
    cout<<"<div id=\"Layer1\" style=\"position:absolute; width:100%; height:100%; z-index:-1\">";
    cout<<"<img src=\"museum.jpg\" height=\"100%\" width=\"100%\"/>";
    cout<<"</div>";
    if(r!=0){
        cout<<"<p><font size=\"6\" face=\"arial\" color=\"red\">存储失败!</font></p>";
		ret=-1;
    }else{
		cout<<"<p><font size=\"6\" face=\"arial\" color=\"red\">存储成功!</font></p>";
	}
    cout<<"<p align=\"center\"><a href=\"../work.html\"><input type=\"button\" value=\"返回上一界面\">";
    cout<<"</a></p></body></center></html>";
	return ret;
}


int SQL::choose(const string& choice)
{
	int ret=0;
    string INSERT="insert into CHOICE(`choice`) values('";
    INSERT+=choice;
    INSERT+="')";
    int r=mysql_query(_conn,INSERT.c_str());
    cout<<"<html><meta charset=\"utf-8\"><center><body>";
    cout<<"<div id=\"Layer1\" style=\"position:absolute; width:100%; height:100%; z-index:-1\">";
    cout<<"<img src=\"museum.jpg\" height=\"100%\" width=\"100%\"/>";
    cout<<"</div>";
    if(r!=0){
        cout<<"<p><font size=\"6\" face=\"arial\" color=\"red\">选择失败!</font></p>";
		ret=-1;
    }else{
        cout<<"<h1>博物馆环境检测系统</h1>";
        cout<<"<form action=\"./insert_cgi\">";
        cout<<"节点位置";
        cout<<"<input type=\"text\" name=\"guangzhao\" value=\"\"><br><br>";
        cout<<"馆内温度";
        cout<<"<input type=\"text\" name=\"wendu\" value=\"\"><br><br>";
        cout<<"PM__2.5";
        cout<<"<input type=\"text\" name=\"guangzhao\" value=\"\"><br><br>";
        cout<<"馆内湿度";
        cout<<"<input type=\"text\" name=\"shidu\" value=\"\"><br><br>";
        cout<<"CO2浓度";
        cout<<"<input type=\"text\" name=\"guangzhao\" value=\"\"><br><br>";
        cout<<"甲醛浓度";
        cout<<"<input type=\"text\" name=\"guangzhao\" value=\"\"><br><br>";
        cout<<"光照强度";
        cout<<"<input type=\"text\" name=\"guangzhao\" value=\"\"><br><br><br>";
        cout<<"<input type=\"submit\" value=\"插入\">";
        cout<<"</form>";
        cout<<"<form action=\"./show_cgi\">";
        cout<<"<input type=\"submit\" value=\"显示\"></form>";
	}
    cout<<"<p align=\"center\"><a href=\"../work.html\"><input type=\"button\" value=\"返回上一界面\">";
    cout<<"</a></p></body></center></html>";
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
	string show="select * from museum order by `TIME` desc";
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
				for(j=0;j<col;j++){
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

string SQL::select()
{
    string ret;
	string select="select * from `CHOICE` order by `id` desc";
	if(mysql_query(_conn,select.c_str())==0){
		res=mysql_store_result(_conn);
		if(res){
			int col=mysql_num_fields(res);
			MYSQL_ROW msg=mysql_fetch_row(res);
			if(msg){
                ret=msg[1];
			}else{
				ret="select error";
			}
		}
	}
    return ret;
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
                    ret=0;
                }
            }else{
                ret=-1;
            }
		}
    }else{
        ret=-2;
    }
    return ret;
}

//int SQL::Delete(const string& id)
//{
//	int ret=0;
//	if(select()<0){
//		ret=-1;
//	}
//	string Delete="delete from museum where id='";
//	Delete+=id;
//	Delete+="'";
//	cout<<Delete<<endl;
//	int r=mysql_query(_conn,Delete.c_str());
//    //int r=mysql_query(_conn,INSERT.c_str());
//	if(r!=0){
//		ret=-1;
//		cerr<<"Delete Failed"<<endl;
//	}else{
//		cout<<"delete success!"<<endl;
//	}
//	return ret;
//}

SQL::~SQL()
{
    mysql_close(_conn);
}
