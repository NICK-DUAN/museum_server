#!/bin/bash

get_pid(){ 
    pid=`ps aux | grep "httpd" | grep -v 'grep' | awk '{print $2}'`
    echo $pid
}

mypid=$(get_pid)

MYPATH=$(pwd)

MUSEUM=$MYPATH/httpd
CONF=$MYPATH/conf/start.conf
CONF_SET=$MYPATH/conf/setconf.sh
IP=`grep 'IP' $CONF | awk '{print $2}'`
PORT=`grep 'PORT' $CONF | awk '{print $2}'`

watch_httpd(){
    _pid=`ps aux | grep "httpd" | grep -v 'grep' | awk '{print $2}'`
    if [ -z "$_pid"  ]
    then
        $MUSEUM $IP $PORT
    fi
}

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
    $MUSEUM $IP $PORT
    watch_httpd
    echo "museum server execute success"
}

stop_server(){
    if [ -z "$mypid" ]
    then
        echo "museum server stopping..."
        make clean >& /dev/null
        cd wwwroot/sql-bin
        make clean >& /dev/null
        cd ../android
        make clean >& /dev/null
        cd ../../
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
