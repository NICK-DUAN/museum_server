#include "sql_api.h"

void show_database()
{
	//cout<<"name----------"<<name<<endl;
	//cout<<"sex-----------"<<sex<<endl;
	//cout<<"birthday------"<<birthday<<endl;
	//cout<<"school--------"<<school<<endl;
    SQL s("127.0.0.1",3306,"root","123456","data");
    s.connect();
	s.show();
}

int main()
{
	show_database();
	return 0;
}
