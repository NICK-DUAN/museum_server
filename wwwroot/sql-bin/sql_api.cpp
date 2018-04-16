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
    cout<<INSERT<<endl;
    int r=mysql_query(_conn,INSERT.c_str());
    if(r!=0){
        cout<<"insert failed"<<endl;
		ret=-1;
    }else{
		cout<<"insert success!"<<endl;
	}
	return ret;
}

int SQL::show()
{
	int ret=0;
	string show="select * from museum";
	if(mysql_query(_conn,show.c_str())==0){
		res=mysql_store_result(_conn);
		if(res){
			int nums=mysql_num_rows(res);
			int col =mysql_num_fields(res);
			cout<<"nums="<<nums<<" "<<"col="<<col<<endl;
			MYSQL_FIELD *fd;
			for(;fd=mysql_fetch_field(res);){
                cout<<fd->name<<" ";
			}
			cout<<endl<<endl;
			int i=0;
			int j=0;
			for(;i<nums;i++){
				MYSQL_ROW row=mysql_fetch_row(res);
				j=0;
				for(;j<col;j++){
					cout<<row[j]<<"    ";
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
