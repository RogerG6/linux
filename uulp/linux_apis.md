### 1. IO
	1. open
	2. read
	3. write
	4. close

	5. fcntl : 操纵文件描述符
	6. ioctl : 控制设备
	
	7. aio_read : 异步读，用于非阻塞读取设备

	8. dup/dup2 : 复制文件描述符 
	9. pipe     : 创建一个管道

	10. fdopen  : 将fd转换成FILE *
	11. popen   : 打开一个指向进程的带缓冲的连接

### 2. 目录
	1. opendir
	2. readdir : 读取目录项, struct dirent
	3. colsedir

### 3. 文件
	1. stat : 获取文件信息, struct stat
	2. utime : 修改文件的atime, mtime

### 4. tty
	1. tcgetattr : 获取终端控制信息, struct termios
	2. tcsetattr : 设置终端控制信息
	3. getttyent : 

### 5. 时间
	1. time : 获得`time_t`的时间
	2. ctime : 由`time_t`转换成固定字符串格式 

	/* 由time_t 转换成struct tm */
	3. gmtime
	4. localtime

	/* 由struct tm 转换成time_t */
	5. astime
	6. mktime

### 6. 用户
	1. crypt : 加密
	2. getpwent : get a password file entry
	3. putpwent : write a password file entry
	
### 7. 库
#### 7.1 curse
	1. initscr ： 初始化curses库和tty
	2. endwin ： 关闭 curses 并重置tty
	3. refresh ： 刷新屏幕
	4. move(row, col)  :  移到row， col位置
	5. addstr ： 在当前位置画字符串
	6. clear    ： 清屏
	7. standout ： 启动standout模式， 一般为反色
	8. standend ：关闭standout模式 

### 8. timer
	1. getitimer : 取得间隔计时器
	2. setitimer : 设置间隔计时器
	3. alarm       : 设置一个闹钟

### 9. signal
	1. signal	    : Unix 中提供软件中断
	2. sigaction	: POSIX 中替代signal
	3. sigprocmask  : 修改当前信号挡板
	4. sigsetops       : 构造信号集
	5. kill                    : 向一个进程发送信号
	
### 10. 进程
	1. pause : 挂起一个进程直到一个信号到达
	2. sleep  : 挂起一个进程 n 秒
	3. usleep : 挂起一个进程 n 微秒

	4. execvp : 在指定路径中查找并执行一个文件
	5. fork   : 创建进程
	6. wait(&statusp)   : 等待进程结束, status是一个16bit的整数，其中：
						  8~15 bit : exit(n)中的n，即退出值
						  7    bit : core dump
						  0~6  bit : signal number
	7. exit

### 11. environ
	1. getenv : 获取环境变量的值

### 12. socket
	1. server
 			1. 获取电话线				socket
 			2. 分配号码					bind
 			3. 允许接入调用				listen
 			4. 等待电话					accept
 			5. 传送数据					read/write
 			6. 挂断电话					close

	2. client
		 	1. 获取电话线				socket
 			2. 呼叫服务器				connect
 			3. 传送数据			 		read/write
 			4. 挂断电话					close
		















