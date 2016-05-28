#! /bin/sh

#本脚本用于生成构造vos.img的makefile文件,同时处理build目录结构

#工作目录
genPath=$(cd `dirname $0`; pwd)
cd $genPath

kernelPath="kernel"
buildPath="build"

#创建目录
function CreateDirIfNotExist() 
{
    if [ ! -d $1 ]
    then 
        mkdir $1
    fi 
}

#遍历$1目录结构并在$2下创建相同目录结构
function ForeachPath()
{
    for file in $1/*
    do
        if [ -d $file ]
        then
            CreateDirIfNotExist $2/$file
            ForeachPath $file $2
        fi
        
    done
} 

#创建build目录
CreateDirIfNotExist $buildPath
#创建build/kernel目录
CreateDirIfNotExist $buildPath/$kernelPath
#将kernel的目录结构复制到build/kernel
ForeachPath $kernelPath $buildPath

#生成Makefile
fileName="Makefile"
#if [ -f $fileName ]
#then
#    mv $fileName $fileName".old"
#fi

#输出头
(
cat <<'sdaf435ewrsdfvue98wj534io5j'
cc = x86_64-elf-gcc -std=gnu99 -ffreestanding -O2 -Wall -Wextra -mcmodel=large -ggdb -c 
ld = x86_64-elf-gcc -z max-page-size=0x1000 -ffreestanding -O2 -nostdlib -lgcc  
asm = nasm -f bin
asme = nasm -f elf64
md = mkdir
kp = kernel
bp = build
bkp = $(bp)/$(kp)
src = \
sdaf435ewrsdfvue98wj534io5j
) > $fileName

#输出src
function ForeachSrc()
{
    for file in $1/*
    do
    
        if [ -d $file ]
        then
            ForeachSrc $file
        elif [[ $file =~ .*\.[cs]$ ]]
        then
            echo $file '\' >> $fileName 
        fi
        
    done
} 

ForeachSrc $kernelPath

#输出编译部分
(
cat <<'sdaf435ewrsdfvue98wj534io5j'

#自动转换obj
objs = $(patsubst %.c,$(bp)/%.o,$(patsubst %.s,$(bp)/%.o,$(src))) 

#合并MBR和内核,内核从第二个扇区开始存放,最终构建为1440KB的软盘
vos: build/mbr.img build/kernel.img
	cd build	;\
	touch vos.img	;\
	dd if=mbr.img of=vos.img	;\
	dd if=/dev/zero of=vos.img seek=1 bs=512 count=2879 ;\
	dd if=kernel.img of=vos.img seek=1 conv=notrunc
    
#编译MBR
build/mbr.img:boot/boot.s
	$(asm) -o $@ $<

#编译内核
build/kernel.img: $(objs)
	$(ld) -T link.lds -o $@ $^

#objs
sdaf435ewrsdfvue98wj534io5j
) >> $fileName


#输出obj部分
function ForeachObj()
{
    for file in $1/*
    do
        if [ -d $file ]
        then
            ForeachObj $file
        elif [[ $file =~ .*\.c$ ]]
        then
            objFile=build/${file%c}o        
            echo $objFile:$file >> $fileName
(
cat <<'sdaf435ewrsdfvue98wj534io5j'
	$(cc) -o $@ $<
sdaf435ewrsdfvue98wj534io5j
) >> $fileName
        elif [[ $file =~ .*\.s$ ]]
        then
            objFile=build/${file%s}o        
            echo $objFile:$file >> $fileName
(
cat <<'sdaf435ewrsdfvue98wj534io5j'
	$(asme) -o $@ $<
sdaf435ewrsdfvue98wj534io5j
) >> $fileName
        fi
        
    done
} 

ForeachObj $kernelPath


#clean
(
cat <<'sdaf435ewrsdfvue98wj534io5j'


#清理build
.PHONY: clean
clean:
	rm -f $(objs)
	rm -f build/*.img
sdaf435ewrsdfvue98wj534io5j
) >> $fileName