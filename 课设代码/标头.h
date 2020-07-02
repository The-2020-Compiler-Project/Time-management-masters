#include<fstream>
#include<stdlib.h>
#include<iostream>
using namespace std;
#include<stdio.h>
#include<string.h>
#include<conio.h>
#include<math.h>
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "adoEOF") 
#pragma once
char* tokenaly();
void recursive();
void translate2();
void analysis_table();
void translate1();
void optimize_s();
bool traverse(char tra[100][15], char cmp[15]);/*遍历终结符和非终结符表，查看是否存在要加入元素*/
void targetcode();
void range(char c[30]);