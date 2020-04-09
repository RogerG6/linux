### 记录在ubuntu16.04中装的一些软件的过程（备忘）

1. goldendict：一款翻译软件
		步骤：1. `sudo apt-get install goldendict`
			  2. 设置：
				在 编辑->词典来源->程序，添加：
                html  有道	/home/roger/tools/trans/youdao_get.py %GDWORD%
			  3. 将youdao_get.py脚本放在上述目录中（前提是已下载python及其
			     需要的模块），此脚本可在
				 https://github.com/easeflyer/gd_plugin/tree/master/youdao
				 中下载
