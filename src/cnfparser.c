
//解析cnf文件读取cnf算例文件，
//解析文件，基于一定的物理结构，建立公式的内部表示；
//并实现对解析正确性的验证功能，即遍历内部结构逐行输出与显示每个子句，
//与输入算例对比可人工判断解析功能的正确性。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

struct Clause *readfile(char*filename){//读取cnf文件
    FILE *fp;
    char line[300];
    fp=fopen(filename,"r");
    if(fp==NULL)
    {
        printf("File open error!\n ");
        return 0;
    }

    struct Clause *root=NULL,*cClause=NULL,*pClause=NULL;
    struct Literal *cLiteral=NULL,*pLiteral=NULL;
    while(fgets(line,sizeof(line),fp)){
        //忽略注释
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){
            sscanf(line,"p cnf %d %d",&variableNumber,&clauseNumber);

            valuation=(int*)calloc(variableNumber+1,sizeof(int));
            int i;
            for(i=0;i<variableNumber+1;i++)
                valuation[i]=-1;
        }
        //为每一行子句创建链表
        else{

            cClause=createClause();
            if(root==NULL)
                root=cClause;
            if(pClause!=NULL)
                pClause->next=cClause;
            char *t;
            t=strtok(line," ");//strtok为分解字符串的函数（以空格为分割）
            while(t!=NULL){
                int literalIndex=atoi(t);//atoi把字符串转换成整形数
               //// printf("%6d",literalIndex);
                cLiteral=createLiteral();
                cLiteral->index=literalIndex;
                if(literalIndex!=0){//读到0的时候结束
                    if(cClause->head==NULL)
                        cClause->head=cLiteral;
                    if(pLiteral!=NULL)
                        pLiteral->next=cLiteral;
                    cClause->Literal_number++;
                }
                pLiteral=cLiteral;
                t=strtok(NULL," ");
            }
            ////printf("\n");

            pClause=cClause;
        }
    }

/*
    int i=0;
    for(i=0;i<=variableNumber;i++){
        printf("%d  %d",i+1,Literal_num[i+1]);
    }
*/
    fclose(fp);
    return root;
}













