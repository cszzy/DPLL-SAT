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
//������ɫ
#define GREEN 2	//��ɫ
#define RED 4	//��ɫ
#define WHITE 7	//��ɫ
int soduko[9][9];//��ʼ������,��������cnf�ļ�
int knockout_flag[9][9];//��¼�ڿպ�������Ϊ0�ĸ��ӣ��ȿ����޸ĵĸ��ӣ�
int count;
int sodukoanswer[82];//��¼�����ļ��������
int checksodukolist[9][9];//����������־��λ���Ƿ��ڹ���,1Ϊ�ڹ�

void setcolor(int color);//�����ֵ���ɫ
int generatesoduko();//������������
int issodukoFull();//�ж������Ƿ���ȫ������
int generateRandNum();//���������
int printsoduko();//��ӡ����
void generateFirstLine();//������������ĵ�һ��
int checkChecklist(int checklist[9]);//���checklist[9]�Ƿ�ȫ����0
int check();//�������������Ƿ��������
int knockout_soduko(int k_soduko[9][9],int hard_level);//�ڿ�
int playsoduko();//��������
int main_g();
int playwithcnf();//������cnf�ļ�������
int readsodukofile(char *filename);//�����������������ļ�
int generatefinalsoduko(int hard_level);//�����ڿ�����
int sodukoToCnf(char *sodukotocnffilename);


#endif // soduko_H_INCLUDED
