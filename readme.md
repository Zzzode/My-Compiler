# Simple C Compiler

![Author](https://img.shields.io/badge/Author-zxhcho-blue.svg?style=flat)
![Author](https://img.shields.io/badge/Author-Zzzcode-blue.svg?style=flat)

<!-- TOC -->
- [Simple C Compiler](#Simple-C-Compiler)
  - [Introduction](#Introduction)
  - [Environment](#Environment)
  - [Instruction](#Instruction)
  - [Architecture](#Architecture)
  - [ChangeLog](#ChangeLog)
    - [v0.0(2019-05-23)](#v002019-05-23)
    - [v0.0(2019-05-25)](#v002019-05-25)
    - [v0.1(2019-06-16)](#v012019-06-16)
    - [v0.2(2019-06-18)](#v022019-06-18)
    - [v0.3(2019-06-19)](#v032019-06-19)
  - [Declaration](#Declaration)
  - [FAQ](#FAQ)
  - [Thanks](#Thanks)
  - [More](#More)

## Introduction

本项目为编译原理课程项目，目的是完成一个C语言编译器。

## Environment

- 系统：win10/linux
- IDE：Jerbrain Clion
- 编译器：MinGW_w64/g++
- 工作语言：C++
- 目标语言：C-
- 实现目标语言编译器：C-
- 编译器目标语言：x86汇编(有改动，不完整) + 自己实现的x86虚拟机(不完整)

## Instruction

1. 生成汇编代码:
`My_Complier.exe -s /.../helloworld.c`

2. 直接运行:
`My_Complier.exe /.../helloworld.c`

## Architecture

暂无

## ChangeLog

### v0.0(2019-05-23)

创建了任务，开始开发。

### v0.0(2019-05-25)

添加了词法分析的部分内容。

### v0.1(2019-06-16)

完成了整体项目的开发，不过有些语法问题，有待修改，程序并不能正常运行。

### v0.2(2019-06-18)

完成了汇编代码生成的所有模块，虚拟机还存在部分问题。

### v0.3(2019-06-19)

修复了一些bug，添加了几个测试用例，虚拟机可以运行。

## Declaration

暂无

## FAQ

暂无

## Thanks

暂无

## More

1. 实现函数栈（参数和指令）
2. 函数局部变量栈
3. 递归函数活动记录
4. 数组声明和参数传递
