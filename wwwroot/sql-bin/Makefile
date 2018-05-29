ROOTPATH=$(shell pwd)
MYSQL_INCLUDE=$(ROOTPATH)/lib/include
MYSQL_LIB=$(ROOTPATH)/lib/lib


.PHONY:all
all:insert_cgi show_cgi register_cgi login_cgi temp_cgi

temp_cgi:temp_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
insert_cgi:insert_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
show_cgi:show_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
register_cgi:register_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
login_cgi:login_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
%.o:%.cpp
	g++ -c $< -I$(MYSQL_INCLUDE) 

.PHONY:clean
clean:
	rm -f insert_cgi show_cgi register_cgi login_cgi temp_cgi *.o
.PHONY:debug
debug:
	echo $(MYSQL_INCLUDE)
	echo $(MYSQL_LIB)
