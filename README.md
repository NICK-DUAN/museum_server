运行环境搭建：
Ubuntu：
	1.安装mysql
	用户名：root，密码：123456
	sudo apt-get install mysql-clinet		 
	sudo apt-get install mysql-server		  
	sudo apt-get install libmysqlclient-dev
	
	2.安装Python2.7
	sudo apt-get install python2.7 python2.7-dev
	
命令行执行：
	mysql -uroot -p123456
	create database data；
	use data；
创建表museum_user：
	CREATE TABLE `museum_user` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `account` varchar(16) NOT NULL,
    `password` varchar(16) NOT NULL,
    PRIMARY KEY (`id`)
);
创建表museum：
	CREATE TABLE `museum` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `Location` varchar(32) NOT NULL,
    `Temperature` varchar(8) NOT NULL,
    `PM2.5` varchar(8) NOT NULL,
    `Hunidity` varchar(8) NOT NULL,
    `Co2` varchar(8) DEFAULT 'No Data',
    `Fromaldehtde` varchar(8) DEFAULT 'No Data',
    `Light` varchar(8) DEFAULT 'No Data',
    `Time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (`id`)
);
创建表CHOICE：
	CREATE TABLE `CHOICE` (
    `id` int(11) NOT NULL AUTO_INCREMENT,
    `choice` varchar(8) NOT NULL,
    PRIMARY KEY (`id`)
)


代码中需修改以下几个文件：
main.c   41行
	if(chdir("/home/nick/museum_server/")<0){
--->路径修改为本地的museum_user
http.c 32行
	char log_path[64]="/home/nick/museum_server/logs/";
---->路径修改为本地的museum_user/logs/
conf/setconf.sh 6行
	local_host=`ifconfig | grep 'inet 地址' | head -1 | awk '{print $2}' | tr -d "地址:"` # get local_host now
---->系统文字若是中文则直接进行，若是英文则将上面代码中的“地址”二字均修改为“addr”

