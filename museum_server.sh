#!/bin/bash

get_pid(){ 
    pid=`ps aux | grep "httpd" | grep -v 'grep' | awk '{print $2}'`
    echo $pid
}

mypid=$(get_pid)

MUSEUM="/home/dlm/server/httpd"
CONF="/home/dlm/server/conf/start.conf"
CONF_SET="/home/dlm/server/conf/setconf.sh"



start_server(){
    echo "museum server executing..."
    $CONF_SET
    make clean >& /dev/null
    make >& /dev/null
    cd wwwroot/sql-bin
    make clean >& /dev/null
    make >& /dev/null
    cd ..
    cd android
    make clean >& /dev/null
    make >& /dev/null
    cd ../..
    sudo cp -r wwwroot /
    echo "museum server execute success"
    IP=`grep 'IP' $CONF | awk '{print $2}'`
    PORT=`grep 'PORT' $CONF | awk '{print $2}'`
    $MUSEUM $IP $PORT
}

stop_server(){
    if [ -z "$mypid" ]
    then
        echo "stop success"
    else
        echo "museum server stopping..."
        make clean >& /dev/null
        cd wwwroot/sql-bin
        make clean >& /dev/null
        cd ../android
        make clean >& /dev/null
        cd ../../
        kill -9 $mypid
        if [ 0 -eq $? ]
        then 
            echo "stop success"
        else
            echo "stop failed"
        fi
    fi
}

main(){
    if [ $# -ne 1 ]
    then
        echo $#
        echo "usage: ./museum_server.sh [stop or start or restart]"
    elif [ $1 == "start" ]
    then
        start_server
    elif [ $1 == "stop" ]
    then
        stop_server
    else
        stop_server
        start_server
    fi
}

main $@
