//主控、交互与显示模块
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "solver2.h"
#include "Sudoku.h"


//逐行打印cnf文件，如果cnf文件不存在，则返回失败
int printCnf(){
    if(ClauseSet==NULL)
        return false;
    else{
        int i;
        for(i=1;i<clauseNumber+1;i++){
            struct Literal * printfCnf_tp=ClauseSet[i].head;
            while(printfCnf_tp){
                printf("%-5d",printfCnf_tp->index);
                printfCnf_tp=printfCnf_tp->next;
            }
            printf("%-5d\n",0);
        }
    }
    return true;
}

int printRes(){
    if(LiteralSet==NULL)
        return false;
    else{
        int i;
        for(i=1;i<variableNumber+1;i++){
            if(valuation[i]==0)
                printf("%-5d",-i);
            else if(valuation[i]==1)
                printf("%-5d",i);
            else
                printf("%-5d",i);
        }
        printf("\n");
    }
}

int main(){
    int op=1;
    char filepath[256]="\0";
    char savepath[256];
    clock_t start,end;
    int result=2;
    int runtime;
    while(op) {

        system("cls");
        printf("\n\n");
        printf("-----------基于SAT的数独游戏求解程序菜单-----------\n");
        printf("----------------------CNF----------------------\n");
        printf("当前加载文件: %s\n",filepath);
        if(ClauseSet) {
            printf("变元数: %4d\n",variableNumber);
            printf("子句数: %4d\n",clauseNumber);
            printf("------------------------------------------------\n");
        }
        printf("1. 加载CNF文件\n");
        printf("2. 求解CNF\n");
        printf("3. 打印CNF\n");
        printf("4. 输出结果\n");
        printf("5. 数独\n");
        printf("0. 退出\n");
        printf("------------------------------------------------\n");
        printf("请选择你的操作[0~5]:");
        scanf("%d",&op);
        switch(op) {
        case 1:
            if(ClauseSet) {//初始化两个邻接表指针
                removeClauseSet();
                removeLiteralSet();
                free(LookupValue);
//                free(canChangeList);
                ClauseSet=NULL;
                LiteralSet=NULL;
                LookupValue=NULL;
//                canChangeList=NULL;
            }
            printf("请输入CNF文件路径: ");
            scanf("%s",filepath);
            if(!readfile(filepath)){//读入cnf文件))
                printf("文件打开失败！\n");
            }
            else
                printf("文件加载成功!\n");
            getchar();
            getchar();
            break;
        case 2:
            if(!ClauseSet) {
                printf("CNF不存在！\n");
            }
            else {
                printf("\n正在使用求解器求解...\n");
                InitLiteralSet();
                start = clock();
                result=dpll2();
                end = clock();
                runtime=(int)(end-start)*1000/CLOCKS_PER_SEC;
                if(result== SATISFIABLE)
                    printf("SATISFIABLE\n");
                else if(result==UNSATISFIABLE)
                    printf("UNSATISFIABLE\n");
                else
                    printf("UNCERTAIN\n");
                printf("DPLL用时 %d ms\n",runtime);
                printf("请输入RES文件路径:\n");
                scanf("%s",savepath);
                if(!writeSolution(savepath,result,runtime))//读入cnf文件))
                    printf("文件保存失败！\n");
                else
                    printf("文件保存成功!\n");
            }
            getchar();
            getchar();
            break;
        case 3:
            if(!ClauseSet)
                printf("CNF不存在！\n");
            else{
                printf("正在打印CNF...\n");
                printCnf();
                printf("打印完成！\n");
            }
            getchar();
            getchar();
            break;
        case 4:
            if(result==2)
                printf("RES不存在！\n");
            else{
                if(result== SATISFIABLE){
                    printf("SATISFIABLE\n");
                    printRes();
                }
                else if(result==UNSATISFIABLE)
                    printf("UNSATISFIABLE\n");
                else
                    printf("UNCERTAIN\n");
            }
            getchar();
            getchar();
            break;
        case 5:
            system("cls");
            sudokupage();
        case 0:
            break;
        }
    }
    if(ClauseSet) {//初始化两个邻接表指针
        removeClauseSet();
        removeLiteralSet();
        free(LookupValue);
//        free(canChangeList);
        ClauseSet=NULL;
        LiteralSet=NULL;
        LookupValue=NULL;
//        canChangeList=NULL;
    }

}

void sudokupage(){
    int op=1;

    clock_t sudoku_start,sudoku_end;
    int sudoku_result=2,sudoku_runtime;
    int hard_level;
    printf("请输入难度等级(1~5):\n");
    scanf("%d",&hard_level);
    while(hard_level<1||hard_level>5){
        printf("输入不合法\n");
        scanf("%d",&hard_level);
    }
    generatefinalsoduko(17+hard_level*10);
    printf("生成数独成功\n");
    while(op){

        system("cls");
        printf("\n\n");
        printf("-----------基于SAT的数独游戏求解程序菜单-----------\n");
        printf("----------------------数独----------------------\n");
        if(ClauseSet) {
            printf("变元数: %4d\n",variableNumber);
            printf("子句数: %4d\n",clauseNumber);
            printf("------------------------------------------------\n");
        }
        printf("1. 玩数独\n");
        printf("2. 使用sat求解器解数独\n");
        printf("0. 回退\n");
        printf("------------------------------------------------\n");
        printf("请选择你的操作[0~3]: ");
        scanf("%d",&op);
        switch(op) {
        case 1:
            playsoduko();
            getchar();
            getchar();
            break;
        case 2:

            if(ClauseSet) {//初始化两个邻接表指针
                removeClauseSet();
                removeLiteralSet();
                free(LookupValue);
//                free(canChangeList);
                ClauseSet=NULL;
                LiteralSet=NULL;
                LookupValue=NULL;
//                canChangeList=NULL;
            }

            sodukoToCnf("sudoku.cnf");
            readfile("sudoku.cnf");
            InitLiteralSet();
            sudoku_start = clock();
            sudoku_result=dpll2();
            sudoku_end= clock();
            sudoku_runtime=(int)(sudoku_end-sudoku_start)*1000/CLOCKS_PER_SEC;
            printf("用时 %d ms\n",sudoku_runtime);
            writeSolution("sudoku.res",sudoku_result,sudoku_runtime);
            readsodukofile("sudoku.res");
            playwithcnf();
            printsoduko();//打印数独
            getchar();
            getchar();
            break;
        case 0:
            break;
        }
    }
}
