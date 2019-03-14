# 1. makefile的引入

当我们使用keil, mdk, avr等工具开发程序时点点鼠标就可以编译了，它内部机制是什么？它怎么组织管理程序？怎么决定编译哪一个文件？

`gcc -o test a.c b.c`

优点：简单

缺点：会对每个文件都处理一次，如果文件量大，效率低

**因此**，需要引入makefile，当makefile编译时，如果发现其中某个文件更新了，只编译该文件，最后再链接。

当编译内核等等大文件时，极大的节约了时间。

# 2. makefile规则

makefile的核心是规则，很简单

```
目标 : 依赖1 依赖2 ...
[TAB]命令
```

当某个依赖比目标文件“新”或目标文件不存在时，则执行相应的命令

# 3. makefile的语法

`#`                         注释

`%.o`                     通配符

`$@  `                   目标

`$<`                       第1个依赖文件

`$^ `                     所有依赖文件

`.PHONY`               假想目标

`:=`                       即时变量，在定义时即确定

`=`                         延时变量，值使用到时才确定

`?=`                       延时变量, 如果是第1次定义才起效, 如果在前面该变量已定义则忽略这句

`+=`                       附加, 它是即时变量还是延时变量取决于前面的定义

# 4. makefile的函数

`$(foreach var,list,text)`                                      把参数<list>中的单词逐一取出放到参数<var>所指定的变量中，然后再执行< text>所包含的表达式
`$(filter pattern...,text)`                                    在text中取出符合patten格式的值
`$(filter-out pattern...,text) `                           在text中取出不符合patten格式的值

`$(wildcard pattern) `                                              pattern定义了文件名的格式，wildcard取出其中存在的文件

`$(patsubst pattern,replacement,$(var))`           从列表中取出每一个值，如果符合pattern，则替换为replacement


