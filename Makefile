ROOT_PATH=$(shell pwd)
CONF=$(ROOT_PATH)/conf
PLUGIN=$(ROOT_PATH)/plugin
WEBROOT=$(ROOT_PATH)/webroot
FLAGS=#-D_DEBUG_#
LDFLAGS=-lpthread #-static
cc=gcc
bin=httpd
src=httpd.c main.c
obj=$(shell echo $(src) | sed 's/\.c/\.o/g') #??

$(bin):$(obj)
	$(cc) -o $@ $^ $(LDFLAGS)
#$(obj):$(src)
%.o:%.c
	$(cc) -c $< $(FLAGS)
.PHONY:clean
clean:
	rm -f *.o $(bin)
.PHONY:debug
debug:
	echo $(src)
	echo $(obj)
