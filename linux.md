# linux 常用命令

### 注：所有命令详细解释可通过man查看

## 1. 常用命令

* man(使用最多)

  ```
  举例：
  	man   man  查看man手册的说明
  	man  ls    当没有指定使用那一页，默认使用第1页
  	man 1 ls
  	man 1 gcc  gcc是一个应用程序，在linux中一般使用gcc编译器来编译c/c++语言的程序
  	open/write/read/close等等都是系统调用
  	man 2 open   查看系统调用open的man手册说明
  注意：
  	man手册的9册内容的侧重点，最好记一下。
  ```

* info<br>eg.    info ls

* --help<br>eg.    ls --help

----

* pwd       ：显示当前所在的目录

* ls            ：显示当前目录下的文件

  * ls -l    ：（long的缩写），显示目录下文件的更详细的信息(文件权限、文件最后修改时间、文件大小

  * ls -a   ：（all的缩写），显示了隐藏文件

  * ls -h   ：（human able的缩写），将文件大小以K(KB)、M(MB)、G(GB)来表示

* cd           ：切换路径

  * cd ..       ：返回到上一级路径

  * cd .        ：切换到当前路径

  * cd../..     ：切换到上上级路径

  * cd~        ：切换到当前用户的家目录

  * cd -        ：切换到上一次的路径

* mkdir     ：新建目录

  * mkdir dir0   ：创建了dir0这个目录

  * mkdir -p  dir1/dir2    ：(parents的缩写)连续创建多级目录(父目录和子目录)，如果父目录不存在，则需要加入-p参数

* rmdir     ：删除目录（rmdir不能删除非空目录）

* touch     ：新建文件（1. 无法创建同名文件，2. linux中文件名不区分大小写）

* cp           ：复制文件    

  * cp File1  file2    ：将当前目录下的文件File1拷贝成file2

  * cp file2 dira/     ：将当前目录下的文件file2拷贝到dira子目录

  * cp -r dira dirb   ：复制dira目录下的所有内容到dirb（-r ：递归复制）

  * cp -i file2 dira   ：如果dira目录下面有同名的file2，加入-i参数就会要求你确认是否覆盖同名的文件

* rm          ：删除文件      

  * rm File1       ：  删除当前目录下的文件File1
       

  * rm -i file2    ：  删除文件file2，删除前要求你确认是否同
    意删除，其中y表明同意删除，n表示取消删除   

  * rm -r dira    ： 删除目录dira
        

  * rm -ir dirb   ： 删除目录dirb，删除前要求你确认是否同意删除，其中y表明同意删除，n表示取消删除  

* cat          ：将文件内容显示到终端中

  * cat file1           ： 将file1的内容打印到标准输出中(默认标准标准输出指向终端)

  * cat file1 file2  ： 将file1和file2的内容串联并依次全部打印到标准输出中

  * cat -n file1      ： 显示内容并在内容前显示行号。

* clear       ：清除屏幕的显示内容

* `export PATH=$PATH:/my/dir` ：设置PATH环境变量添加我们的目录

## 2. 压缩文件

压缩有2个参数：<br>        1. 压缩时间<br>        2. 压缩比<br>

linux下常用的压缩命令<br>        **单个文件**的压缩（解压）使用gzip 和 bzip2，且一般情况下，**小文件用gzip，大文件用bzip2**<br>        **多个文件**和目录时，使用tar<br>

1. **gzip**<br>常用选项：<br>    -l(list)                        列出压缩文件的内容<br>    -k(keep)                   在压缩或解压时，保留输入文件<br>    -d(decompress)     将压缩文件进行解压缩

2. **bzip2**<br>常用选项：<br>     -k(keep)                      在压缩或解压时，保留输入文件<br>     -d(decompress)        将压缩文件进行解压缩    

3. **tar**<br>常用选项：<br>     -c(creat)表示创建用来生成文件包<br>     -x                               表示提取，从文件包中提取文件<br>     -t                               可以查看压缩的文件<br>     -z                               使用gzip方式进行处理，它与“c”结合就表示压缩，与”x“结合就表示解压缩<br>     -j                               使用bzip2方式进行处理，它与”c“结合就表示压缩，与”x“结合就表示解压缩<br>     -v(verbose)             详细报告tar处理的信息<br>     -f(file)                       表示文件，后面接着一个文件名<br>     -C  <指定目录>        解压到指定目录

## 3. 查找

1. **find**<br>用途：查找符合条件的文件<br>格式：find 目录名 选项 查找条件<br>eg.    `find /work/001_linux/dira -name "test.txt"`：在/work/001_linux/dira目录中查找名为test.txt的文件，若未指定目录，则为当前目录<br>         `find /home -mtime -2`：查找/home目录下2天内有变动的文件

2. **grep**<br>  用途：查找文件中符合条件的字符串<br>  格式：grep   选项   查找模式   文件名<br>  eg.    `grep -n "abc" test1.txt`   :   在test1.txt中查找字符串abc<br>           `grep -rn "abc" * `                  :    在当前目录递归查找字符串abc（可以用`-w`进行全字匹配）

3. **file**<br>用途：识别文件类型<br>格式：file    文件名 <br>

   ```
   eg. 
       file ~/.bashrc     为ASCII 编码的text类型
       file ~/.vimrc      为UTF-8 Unicode 编码的text类型
       file ~/Pictures/*  如图形文件JPEG/PNG/BMP格式
       file ~/100ask/     为directory表明这是一个目录
       file /bin/pwd      出现 ELF 64-bit LSB executable，即为ELF格式的可执行文件
       file /dev/*        出现character special(字符设备文件)、 block special(块设备文件)等
   ```

4. **which 和 whereis**<br>用途：查找命令或应用程序的所在位置<br>格式：which     命令名/应用程序名<br>

   ```
   eg.
   	which pwd 定位到/bin/pwd
   	which gcc 定位到/usr/bin/gcc
   	whereis  pwd查找到可执行程序的位置/bin/pwd和手册页的位置/usr/share/man/man1/pwd.1.gz
   ```

   


