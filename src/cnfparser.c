
//����cnf�ļ���ȡcnf�����ļ���
//�����ļ�������һ��������ṹ��������ʽ���ڲ���ʾ��
//��ʵ�ֶԽ�����ȷ�Ե���֤���ܣ��������ڲ��ṹ�����������ʾÿ���Ӿ䣬
//�����������Աȿ��˹��жϽ������ܵ���ȷ�ԡ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

struct Clause *readfile(char*filename){//��ȡcnf�ļ�
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
        //����ע��
        if(line[0]=='c')
            continue;
        if(line[0]=='p'){
            sscanf(line,"p cnf %d %d",&variableNumber,&clauseNumber);

            valuation=(int*)calloc(variableNumber+1,sizeof(int));
            int i;
            for(i=0;i<variableNumber+1;i++)
                valuation[i]=-1;
        }
        //Ϊÿһ���Ӿ䴴������
        else{

            cClause=createClause();
            if(root==NULL)
                root=cClause;
            if(pClause!=NULL)
                pClause->next=cClause;
            char *t;
            t=strtok(line," ");//strtokΪ�ֽ��ַ����ĺ������Կո�Ϊ�ָ
            while(t!=NULL){
                int literalIndex=atoi(t);//atoi���ַ���ת����������
               //// printf("%6d",literalIndex);
                cLiteral=createLiteral();
                cLiteral->index=literalIndex;
                if(literalIndex!=0){//����0��ʱ�����
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













