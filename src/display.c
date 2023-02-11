//���ء���������ʾģ��
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include "solver2.h"
#include "Sudoku.h"


//���д�ӡcnf�ļ������cnf�ļ������ڣ��򷵻�ʧ��
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
        printf("-----------����SAT��������Ϸ������˵�-----------\n");
        printf("----------------------CNF----------------------\n");
        printf("��ǰ�����ļ�: %s\n",filepath);
        if(ClauseSet) {
            printf("��Ԫ��: %4d\n",variableNumber);
            printf("�Ӿ���: %4d\n",clauseNumber);
            printf("------------------------------------------------\n");
        }
        printf("1. ����CNF�ļ�\n");
        printf("2. ���CNF\n");
        printf("3. ��ӡCNF\n");
        printf("4. ������\n");
        printf("5. ����\n");
        printf("0. �˳�\n");
        printf("------------------------------------------------\n");
        printf("��ѡ����Ĳ���[0~5]:");
        scanf("%d",&op);
        switch(op) {
        case 1:
            if(ClauseSet) {//��ʼ�������ڽӱ�ָ��
                removeClauseSet();
                removeLiteralSet();
                free(LookupValue);
//                free(canChangeList);
                ClauseSet=NULL;
                LiteralSet=NULL;
                LookupValue=NULL;
//                canChangeList=NULL;
            }
            printf("������CNF�ļ�·��: ");
            scanf("%s",filepath);
            if(!readfile(filepath)){//����cnf�ļ�))
                printf("�ļ���ʧ�ܣ�\n");
            }
            else
                printf("�ļ����سɹ�!\n");
            getchar();
            getchar();
            break;
        case 2:
            if(!ClauseSet) {
                printf("CNF�����ڣ�\n");
            }
            else {
                printf("\n����ʹ����������...\n");
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
                printf("DPLL��ʱ %d ms\n",runtime);
                printf("������RES�ļ�·��:\n");
                scanf("%s",savepath);
                if(!writeSolution(savepath,result,runtime))//����cnf�ļ�))
                    printf("�ļ�����ʧ�ܣ�\n");
                else
                    printf("�ļ�����ɹ�!\n");
            }
            getchar();
            getchar();
            break;
        case 3:
            if(!ClauseSet)
                printf("CNF�����ڣ�\n");
            else{
                printf("���ڴ�ӡCNF...\n");
                printCnf();
                printf("��ӡ��ɣ�\n");
            }
            getchar();
            getchar();
            break;
        case 4:
            if(result==2)
                printf("RES�����ڣ�\n");
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
    if(ClauseSet) {//��ʼ�������ڽӱ�ָ��
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
    printf("�������Ѷȵȼ�(1~5):\n");
    scanf("%d",&hard_level);
    while(hard_level<1||hard_level>5){
        printf("���벻�Ϸ�\n");
        scanf("%d",&hard_level);
    }
    generatefinalsoduko(17+hard_level*10);
    printf("���������ɹ�\n");
    while(op){

        system("cls");
        printf("\n\n");
        printf("-----------����SAT��������Ϸ������˵�-----------\n");
        printf("----------------------����----------------------\n");
        if(ClauseSet) {
            printf("��Ԫ��: %4d\n",variableNumber);
            printf("�Ӿ���: %4d\n",clauseNumber);
            printf("------------------------------------------------\n");
        }
        printf("1. ������\n");
        printf("2. ʹ��sat�����������\n");
        printf("0. ����\n");
        printf("------------------------------------------------\n");
        printf("��ѡ����Ĳ���[0~3]: ");
        scanf("%d",&op);
        switch(op) {
        case 1:
            playsoduko();
            getchar();
            getchar();
            break;
        case 2:

            if(ClauseSet) {//��ʼ�������ڽӱ�ָ��
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
            printf("��ʱ %d ms\n",sudoku_runtime);
            writeSolution("sudoku.res",sudoku_result,sudoku_runtime);
            readsodukofile("sudoku.res");
            playwithcnf();
            printsoduko();//��ӡ����
            getchar();
            getchar();
            break;
        case 0:
            break;
        }
    }
}
