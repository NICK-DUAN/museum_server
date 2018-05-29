#include <iostream>
#include <string>
#include <mysql.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#ifndef _SQL_
#define _SQL__

class SQL
{
public:
    SQL(const string &host,const int& port,const string& user="root",const string& password="",const string& database="data");
    int connect();
    int insert(const string& temperature,const string& light,const string& water);
	int select(const string &id);
	int shown();
	int showh();
	int Delete(const string& id);
    int Register(const string& account, const string& pwd);
    int login(const string& account, const string& pwd);
    ~SQL();
private:
    SQL();
    MYSQL* _conn;
    MYSQL_RES* res; 
    string _host;
    string _user;
    string _password;
    string _database;
    int _port;
};
#endif
