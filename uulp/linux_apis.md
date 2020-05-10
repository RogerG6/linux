1. IO
	1. open
	2. read
	3. write
	4. close

	5. fcntl : 操纵文件描述符
	6. ioctl : 控制设备

2. 目录
	1. opendir
	2. readdir : 读取目录项, struct dirent
	3. colsedir

3. 文件
	1. stat : 获取文件信息, struct stat
	2. utime : 修改文件的atime, mtime

4. tty
	1. tcgetattr : 获取终端控制信息, struct termios
	2. tcsetattr : 设置终端控制信息
	3. getttyent : 

5. 时间
	1. time : 获得`time_t`的时间
	2. ctime : 由`time_t`转换成固定字符串格式 

	/* 由time_t 转换成struct tm */
	3. gmtime
	4. localtime

	/* 由struct tm 转换成time_t */
	5. astime
	6. mktime

6. 用户
	1. crypt : 加密
	2. getpwent : get a password file entry
	3. putpwent : write a password file entry
