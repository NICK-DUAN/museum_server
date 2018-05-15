ROOTPATH=$(shell pwd)
MYSQL_INCLUDE=$(ROOTPATH)/lib/include
MYSQL_LIB=$(ROOTPATH)/lib/lib


.PHONY:all
all:show_cgi register_cgi login_cgi

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
	rm -f show_cgi register_cgi login_cgi *.o
.PHONY:debug
debug:
	echo $(MYSQL_INCLUDE)
	echo $(MYSQL_LIB)
