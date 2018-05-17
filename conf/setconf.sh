#!/bin/bash

MYPATH=$(pwd)
CONF=$MYPATH/start.conf
PORT="8080"

local_host=`ifconfig | grep 'inet 地址' | grep '192.168' | awk '{print $2}' | tr -d "地址:"` # get local_host now
old_host=`grep "IP" $CONF | awk '{print $2}' ` # get old_host

sed -i "s/$old_host/$local_host/g" $CONF  #exchange host ip
