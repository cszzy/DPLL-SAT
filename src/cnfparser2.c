//����cnf�ļ���ȡcnf�����ļ���
//�����ļ�������һ��������ṹ��������ʽ���ڲ���ʾ��
//��ʵ�ֶԽ�����ȷ�Ե���֤���ܣ��������ڲ��ṹ�����������ʾÿ���Ӿ䣬
//�����������Աȿ��˹��жϽ������ܵ���ȷ�ԡ�
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver2.h"

struct Clause *readfile(char*filename){//��ȡcnf�ļ�
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
        //����ע��
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

            for(i=0;i<variableNumber+1;i++){//��ʼֵ����ֵ-1����ʾ��ȷ��
                valuation[i]=-1;//��ֵ���ʼֵ����ֵ-1����ʾ��ȷ��
                LookupValue[i].negative=0;//�����ʼֵ����ֵ0
                LookupValue[i].positive=0;
                sord[i]=0;
//                canChangeList[i].negative=0;
//                canChangeList[i].positive=0;
            }
        }
        //Ϊÿһ���Ӿ䴴������
        else{
            char *t;
            t=strtok(line," ");//strtokΪ�ֽ��ַ����ĺ������Կո�Ϊ�ָ
            while(t!=NULL){
                int literalIndex=atoi(t);//atoi���ַ���ת����������
               //// printf("%6d",literalIndex);
                cLiteral=createLiteral();
                cLiteral->index=literalIndex;
                if(literalIndex!=0){//����0��ʱ�����
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
                    if(pLiteral!=NULL){//��������
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

    for(i=1;i<variableNumber+1;i++){//��������Ƶ��
        sord[i]=LookupValue[i].positive+LookupValue[i].negative;
    }
    //sordLiteral();

    fclose(fp);
    //printf("11111  ");
    //system("pause");
    return ClauseSet;
}













