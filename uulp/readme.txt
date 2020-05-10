这个文件夹主要存放学习Understand Unix/Linux Programming Bruce Molay的代码

uulp/
	1. more: 不能显示进度
	2. cat
	3. cp: 不能递归复制
	4. head
	5. tail
	6. ls
	7. who
	8. whoami
	9. chown
	10. chmod
	11. file: 不能像file输出一样
	12. chgrp
	13. mkdir -p
	14. mv
	15. stty
		1. echostat.c: 显示回显状态
		2. setecho.c :  设置回显状态
		3. showtty.c : 显示当前tty的一些属性
	16. write: 不能显示当前终端的用户名
	17. passwd: 如何写入密码
	18. uppercase: 跟踪struct termios中o_flags中的OLCUC位的状态

	test/
		1. lseek
		2. onFileOneProcess
		3. winsize: 获得当前终端的尺寸
		4. demondir

	else/
		1. char: 打印出ASCII表中的字符
		2. ltm: 将rwxr-x--x 转换成 111101001
		3. rd_nord_perm: 关闭读权限后能不能继续读文件

