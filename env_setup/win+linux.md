# win7+ubuntu18.04双系统

## 1. 工具
- U盘
- rufus 引导盘制作工具
- 电脑：dell latitude E7250（最好支持linux，比如thinkpad T470或dell 系列）
	   具体硬件参考
 [ubuntu hardware certification](https://certification.ubuntu.com/) 

## 2. 步骤<br>
- 先制作win7，分区时留出要装Linux的空间，必须是未使用的。
**MBR＋UEFI，NTFS，4096簇**
- 制作ubuntu18.04的启动盘
**MBR＋UEFI,NTFS, 4096簇**
- 安装
	- 插入U盘，重启，按F2或F12进入BIOS
	- 关闭secure boot，启动UEFI（win7是Legency），保存
	- 重启，按F2或F12，从U盘启动
	- Ubuntu安装
		- 选择与win7共存，点击安装即可（如果要DIY，自己手动分区，则选择自定义模式，具体分区：/boot /root /swap /home ...）
- 重启，此时会发现并没有进入Ubuntu或win，跳出来的是grub命令行界面
是一个系统引导工具，需要配置如下：
	- `set root=hd0, A`  ：此处A是安装linux那个盘，可用ls查看
	- `set prefix=(hd0,A)/boot/grub`: 如果grub在boot目录下
	- `linux /vmlinuz root=/dev/sdaA`: sda取决于kernel具体在哪个sda上可以通过命令`ls (hd0,A)/etc/fstab`查看
	- `initrd /initrd.img-xxx`: TAB补全img
	- `boot`: 启动
- 进入Ubuntu后，生成grub.cfg配置文件
	- `grub -mk grub-mkconfig -o /boot/grub/grub.cfg`
- 重启后，进入BIOS，设置为Legency，boot sequence为HDD第一
- 设置启动顺序
	- 进入ubuntu终端，`sudo chmod +w /boot/grub/grub.cfg`
	- 修改启动顺序，里面从###BEGIN.....###END 之间**块**是一个系统的启动信息，有多个系统的话，只要调整块与块之间的顺序即可，保存
	- `sudo chmod -w /boot/grub/grub.cfg`
	- 最后，重启即可看到选择界面了，可以选择进入哪个系统

## 3. 后记
- 这只是本次安装过程中踩的坑，具体机型具体分析，多百度
	



















