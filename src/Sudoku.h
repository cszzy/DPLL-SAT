#ifndef soduko_H_INCLUDED
#define soduko_H_INCLUDED
//
// Created by littleadd on 2019-01-23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include <curses.h>



#include <time.h>
#define false 0
#define true 1

#include <windows.h>
#include <winnt.h>
//定义颜色
#define GREEN 2	//绿色
#define RED 4	//红色
#define WHITE 7	//白色
int soduko[9][9];//初始化数独,用来生成cnf文件
int knockout_flag[9][9];//记录挖空后数组中为0的格子（既可以修改的格子）
int count;
int sodukoanswer[82];//记录数独文件解的数组
int checksodukolist[9][9];//检查表用来标志此位置是否挖过空,1为挖过

void setcolor(int color);//设置字的颜色
int generatesoduko();//生成完整数独
int issodukoFull();//判断数独是否已全部填满
int generateRandNum();//生成随机数
int printsoduko();//打印数独
void generateFirstLine();//随机生成数独的第一行
int checkChecklist(int checklist[9]);//检查checklist[9]是否全部非0
int check();//检查填入的数字是否符合条件
int knockout_soduko(int k_soduko[9][9],int hard_level);//挖空
int playsoduko();//人玩数独
int main_g();
int playwithcnf();//电脑用cnf文件解数独
int readsodukofile(char *filename);//读计算机解出的数独文件
int generatefinalsoduko(int hard_level);//生成挖空数独
int sodukoToCnf(char *sodukotocnffilename);


#endif // soduko_H_INCLUDED
