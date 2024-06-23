#!/bin/bash
set -ev
go tool compile -trimpath="`pwd`=>" -l -p gom point.go

# 查看符号表里面所有方法名称
go tool nm point.o | grep T

# 查看某个方法（值类型）具体实现
go tool objdump -S -s '^gom.Point.X$' point.o

# 查看某个方法(指针类型）具体实现
go tool objdump -S -s '^gom.\(\*Point\).SetX$' point.o

# 查看自动生成方法具体实现
go tool objdump -S -s  '^gom.\(\*Point\).X$' point.o

# 查看继承的方法具体实现
go tool objdump -S -s '^gom.Point2d.X$' point.o


# 查看继承的方法(自动实现)具体实现
go tool objdump -S -s '^gom.\(\*Point2d\).SetX$' point.o

# 查看直接调用类型方法的实现
go tool objdump -S -s '^gom.ReadFile$' point.o

