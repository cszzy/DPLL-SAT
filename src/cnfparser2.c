//解析cnf文件读取cnf算例文件，
//解析文件，基于一定的物理结构，建立公式的内部表示；
//并实现对解析正确性的验证功能，即遍历内部结构逐行输出与显示每个子句，
//与输入算例对比可人工判断解析功能的正确性。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver2.h"

struct Clause *readfile(char*filename){//读取cnf文件
    FILE *fp;
    char line[600];
    fp=fopen(filename,"r");
    int debug=1;
    if(fp==NULL)
    {
        return NULL;
    }

    struct Literal *cLiteral=NULL,*pLiteral=NULL;
    int i,clauseorder=1;
    while(fgets(line,sizeof(line),fp)){
        //忽略注释
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){
            sscanf(line,"p cnf %d %d",&variableNumber,&clauseNumber);
            valuation=(int*)calloc(variableNumber+1,sizeof(int));
            LiteralSet=(struct LiteralLink *)calloc(variableNumber+1,sizeof(struct LiteralLink));
            LookupValue=(struct LiterValueList*)calloc(variableNumber+1,sizeof(struct LiterValueList));
            sord=(int *)calloc(variableNumber+1,sizeof(int));
//            canChangeList=(struct LiterValueList*)calloc(variableNumber+1,sizeof(struct LiterValueList));
            ClauseSet=(struct Clause *)calloc(clauseNumber+1,sizeof(struct Clause));
            if((valuation==NULL)||(LiteralSet==NULL)||(LookupValue==NULL)||(ClauseSet==NULL))
                exit(1);

            InitStack(&var_stack);

            for(i=0;i<variableNumber+1;i++){//初始值都赋值-1，表示不确定
                valuation[i]=-1;//赋值表初始值都赋值-1，表示不确定
                LookupValue[i].negative=0;//检查表初始值都赋值0
                LookupValue[i].positive=0;
                sord[i]=0;
//                canChangeList[i].negative=0;
//                canChangeList[i].positive=0;
            }
        }
        //为每一行子句创建链表
        else{
            char *t;
            t=strtok(line," ");//strtok为分解字符串的函数（以空格为分割）
            while(t!=NULL){
                int literalIndex=atoi(t);//atoi把字符串转换成整形数
               //// printf("%6d",literalIndex);
                cLiteral=createLiteral();
                cLiteral->index=literalIndex;
                if(literalIndex!=0){//读到0的时候结束
                    if(literalIndex>0){
                        LookupValue[abs(literalIndex)].positive++;
//                        canChangeList[abs(literalIndex)].positive++;
                    }
                    else{
                        LookupValue[abs(literalIndex)].negative++;
//                        canChangeList[abs(literalIndex)].negative++;
                    }
                    if(ClauseSet[clauseorder].head==NULL)
                        ClauseSet[clauseorder].head=cLiteral;
                    if(pLiteral!=NULL){//创建链表
                        //cLiteral->prev=pLiteral;
                        pLiteral->next=cLiteral;
                    }
                    ClauseSet[clauseorder].Literal_number++;
                }

                pLiteral=cLiteral;
                t=strtok(NULL," ");
            }
            ////printf("\n");
            //printf("%d  %d\n",ClauseSet[clauseorder].Literal_number,debug++);
            clauseorder++;
        }
    }

    for(i=1;i<variableNumber+1;i++){//排列文字频度
        sord[i]=LookupValue[i].positive+LookupValue[i].negative;
    }
    //sordLiteral();

    fclose(fp);
    //printf("11111  ");
    //system("pause");
    return ClauseSet;
}













