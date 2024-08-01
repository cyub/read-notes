# Mac安装bochs指南

## 安装bochs

```bash
brew install bochs
```

## 创建软盘

在安装bochs时，还会自动安装一些辅助工具，其中包括虚拟磁盘镜像创建工具bximage。

- 使用bximage命令直接创建磁盘镜像，选择1创建磁盘
- 输入fd，表示创建软盘
- 选择软盘的大小，默认的1.44M就可以了
- 输入镜像的名称，可以使用默认的镜像名a.img，也可以自定义名称boot.img，只要能跟配置文件中的名称对应即可

![](https://static.cyub.vip/images/202309/bximage_usage.png)

## 编写Boot引导程序

创建一个boot.asm文件，编写如下代码：

```assembly
org 7c00h; 告诉编译器程序加载到7c00处
mov ax, cs
mov ds, ax
mov es, ax
call DispStr; 调用显示字符串例程
jmp $; 无限循环

DispStr:
    mov ax, BootMessage
    mov bp, ax; ES:BP = 字符串地址
    mov cx, 10; CX = 字符串长度
    mov ax, 1301h; AH = 13h, AL = 01h
    mov bx, 0002h; BH = 00 页号为0,  BL =02 黑底绿字，高亮
    mov dl, 0
    int 10h; 10h号中断
    ret

BootMessage db "Hello, OS!"
times 510 - ($-$$) db 0; times是重复指令，该语句含义是重复执行db 0指令510-($-$$)次
dw 0xaa55; bootloader magic number
```

## 编译程序，写入软盘

首先，使用NASM编译器进行程序编译，生成“操作系统”的二进制代码。如果没有nasm，依然使用brew进行安装：

```
brew install nasm
nasm boot.asm -o boot.bin
```

编译结束后，将生成的二进制程序boot.bin写入到boot.img中。使用系统自带的dd命令，把Boot程序强制写入到虚拟软盘的固定扇区中：

```
dd if=boot.bin of=boot.img bs=512 count=1 conv=notrunc
```

在这行命令中，if指定输入源文件名，of指定输出文件名，count=1指定传输的块大小为512B，conv=notrunc规定在写入数据后不截断输出文件的尺寸大小。

## 配置启动信息

### 配置BXSHARE环境变量

BXSHARE环境会用到启动的配置文件中，我们配置下：

```shell
export BXSHARE="/opt/homebrew/Cellar/bochs/2.7/share/bochs"
```

从bochs的安装包中的示例文件bochsrc-sample.txt拷贝一份创建bochsrc.txt，做如下修改：

```shell
#图形模式设置，这两个一起开启  
magic_break: enabled=1
display_library: sdl2

#bios 和 vga bios
romimage: file=$BXSHARE/BIOS-bochs-latest 
vgaromimage: file=$BXSHARE/VGABIOS-lgpl-latest

#1.44=磁盘镜像位置  boot.img 跟刚才创建磁盘时的磁盘名字保持一致
floppya: 1_44=boot.img, status=inserted

#从软盘启动
boot: floppy

#输出日志
log: bochsout.txt

#置鼠标不可用
mouse: enabled=0

#键盘配置
keyboard: keymap=#$BXSHARE/keymaps/x11-pc-us.map

#内存容量
megs: 32

#注释掉下面这两个
#sound: driver=default, waveout=/dev/dsp. wavein=, midiout=
#ata0-master: type=disk, mode=flat, path="30M.sample"
```


## 启动bochs

```
bochs -f bochsrc.txt
```

然后选择以什么方式启动，这里默认就行，即选择模拟器:

![](https://static.cyub.vip/images/202309/bochs_run.png)

在虚拟机刚启动，它只完成了硬件平台的初始化，还为执行引导程序。在出现<bochs:1>后输入字符串c或者cont或者continue，即可使虚拟机运行。

引导程序的运行效果如下所示：

![](https://static.cyub.vip/images/202309/bochs_ui.png)

注意boot.img需要放在当前目录下，要不然bochs找不到。

## 附1. Bochs调试命令

指令 | 说明 | 举例
--- | --- | ---
b address | 在某物理地址上设置断点 | b 0x7c00
C | 继续执行，直到遇到断点 | C
S | 单步执行 | S
info cpu | 查看寄存器信息 | info cpu
r | 查看寄存器信息  | r
sreg | 查看寄存器信息 | sreg
creg | 查看寄存器信息 | creg
Xp /nuf addr |  查看内存物理地址内容 | xp /10bx 0x100000
X /nuf addr | 查看线性地址内容 | X /40wd 0x90000
u start end | 反汇编一段内存 | u 0x100000 0x100010


**注：** 
- n代表显示单元个数；
- u代表显示单元大小[b：Byte、 h: Word、 w: DWord、 g：Qwrod（四字节）]；
- f代表显示格式（x：十六进制、d：十进制、七：二进制、c：字符）。

## 附2. 安装和启动qemu

### 安装qemu：


```shell
brew install qemu
brew info qemu # 检查是否安装成功
```

### 启动 img 镜像：

```
qemu-system-i386 -L [BIOS、VGA BIOS、keymaps 的目录] -m 32m -rtc base=localtime -vga std -hda [img路径]
```

参数解释：
- -L：设置 BIOS、VGA BIOS、keymaps 的目录。
- -m：设置客户机的内存。
- -rtc：设置时间。
- -vga：显示器相关。
- -hda：指定镜像的路径，最好是绝对路径，使用绝对路径麻烦相对少一些。

使用示例：

```
qemu-system-i386 -L /opt/homebrew/Cellar/bochs/2.7/share/bochs -m 32m -rtc base=localtime -vga std -hda /opt/homebrew/Cellar/bochs/2.7/bin/boot.img
```

## 参考资料

- [在Mac中安装bochs以及遇到的问题](https://juejin.cn/post/7076052275600293918)
- [Mac M2 安装bochs](https://www.jianshu.com/p/6cde129d4bbe)