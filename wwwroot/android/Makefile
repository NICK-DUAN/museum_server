ROOTPATH=$(shell pwd)
MYSQL_INCLUDE=$(ROOTPATH)/lib/include
MYSQL_LIB=$(ROOTPATH)/lib/lib


.PHONY:all
all:shown_cgi showh_cgi register_cgi login_cgi

shown_cgi:shown_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
showh_cgi:showh_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
register_cgi:register_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
login_cgi:login_cgi.o sql_api.o
	g++ -o $@ $^ -L$(MYSQL_LIB) -lmysqlclient -g
%.o:%.cpp
	g++ -c $< -I$(MYSQL_INCLUDE) 

.PHONY:clean
clean:
	rm -f shown_cgi showh_cgi register_cgi login_cgi *.o
.PHONY:debug
debug:
	echo $(MYSQL_INCLUDE)
	echo $(MYSQL_LIB)
