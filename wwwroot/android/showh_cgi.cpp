#include "sql_api.h"

void show_database()
{
    SQL s("127.0.0.1",3306,"root","123456","data");
    s.connect();
	s.showh();
}

int main()
{
	show_database();
	return 0;
}
