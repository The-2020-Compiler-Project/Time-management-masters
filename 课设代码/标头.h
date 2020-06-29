#include<fstream>
#include<stdlib.h>
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<math.h>
#pragma once
using namespace std;
char* tokenaly();
int tableget(char tra[100][15], char cmp[15]);
bool traverse(char tra[100][15], char cmp[15]);//遍历终结符和非终结符表，查看是否存在要加入元素
void tableend();//完成分析表
void inittable();//分析表的初始化
void initproduct();//初始化产生式结构