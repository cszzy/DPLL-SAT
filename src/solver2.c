//����DPLL�㷨��ܣ�ʵ��SAT��������⡣
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver2.h"
#include "Sudoku.h"

//��������ʼ��������һ�����Ӿ�
struct Clause * createClause(){
    struct Clause * instance = malloc(sizeof(struct Clause));
    if(!instance)
        exit(1);
    instance->head = NULL;
    //instance->next = NULL;
    instance->Cvalue=0;//��ʼΪδȷ��
    //instance->Pvalue=0;
    instance->Literal_number=0;
    return instance;
}

// ��������ʼ��������һ��������
struct Literal * createLiteral(){
    struct Literal * instance = malloc(sizeof(struct Literal));
    if(!instance)
        exit(1);
    //instance->prev =NULL;
    instance->next = NULL;
    instance->index = 0;//����ֵ
    //instance->c_flag=0;
    //instance->p_flag=0;
    return instance;
}

int InitLiteralSet(){//��ʼ�������ڽӱ�
    int i;
    for(i=1;i<variableNumber+1;i++){//����ռ�
        if(LookupValue[i].positive>0)
            LiteralSet[i].posOcccur=(int*)calloc(LookupValue[i].positive,sizeof(int));
        else
            LiteralSet[i].posOcccur=NULL;//û��������Ϊ��ָ��
        if(LookupValue[i].negative>0)
            LiteralSet[i].negOccur=(int*)calloc(LookupValue[i].negative,sizeof(int));
        else
            LiteralSet[i].negOccur=NULL;
        LiteralSet[i].chooseflag=0;
    }

    struct Literal *tpNode;
    int var;
    for(var=1;var<variableNumber+1;var++){//����ÿһ������
        int j=0,k=0;
        for(i=1;i<clauseNumber+1;i++){//���������ڽӱ�
            //printf("%d .",debug++);
            tpNode=ClauseSet[i].head;
            while(tpNode){
                if(tpNode->index==var){//������
                    LiteralSet[abs(tpNode->index)].posOcccur[j]=i;
                    //printf("-----��Ԫ%d�ڵ�%d��������---",tpNode->index,LiteralSet[abs(tpNode->index)].posOcccur[j]);
                    j++;
                }
                else if(tpNode->index==-var){
                    LiteralSet[abs(tpNode->index)].negOccur[k]=i;
                    //printf("-----��Ԫ%d�ڵ�%d��������---",tpNode->index,LiteralSet[abs(tpNode->index)].negOccur[k]);
                    k++;
                }
                tpNode=tpNode->next;
            }
        }
    }
    //printf("%d;;;",debug++);
    //system("pause");
    return true;
}

int InitStack(struct Literal_stack* L_stack){
    L_stack->base=(int*)malloc((variableNumber+1)*sizeof(int));
    if(!L_stack->base)
        exit(1);
    L_stack->top=L_stack->base;//��ʼΪ��
    return true;
}

int Push(struct Literal_stack* L_stack,int var){//��ջ����
    *L_stack->top=var;
    L_stack->top++;
    return true;
}

int Pop(struct Literal_stack* L_stack){//��ջ������ջ���շ���ջ��Ԫ�أ�ջΪ�շ���false
    if(L_stack->top==L_stack->base)//ջΪ��
        return false;
    L_stack->top--;
    int var=*L_stack->top;
    return var;
}
/*
int destroyStack(struct Literal_stack* L_stack){
    if(L_stack!=NULL){
        free(L_stack->base);
        L_stack->base=NULL;
        L_stack->top=NULL;
        return true;
    }
    return false;
}
*/
/*
void sordLiteral(){
    int j,k,a;
    for(j=1;j<variableNumber+1;j++){
        for(k=j+1;k<variableNumber+1;k++)
            if(sord[k]>sord[j]){
            a=sord[j];
            sord[j]=sord[k];
            sord[k]=a;
        }
    }
}
*/
int chooseLiteral(){//ѡ�����
    //ֻ���ص�һ������ֵ��������ı����������ʹ�ø����ܵķ���������ٶ�
    // (e.g. ѡ��Ƶ����ߵ�����)
    int i,j;
    for(i=1;i<clauseNumber+1;i++){//���ҵ��Ӿ�
        if(ClauseSet[i].Literal_number==1&&ClauseSet[i].Cvalue==UNCERTAIN){
            struct Literal *tp=ClauseSet[i].head;
            while(tp){
                if(valuation[abs(tp->index)]==-1){
                    j= tp->index;
                    return j;
                }
                tp=tp->next;
            }
        }
    }
    /*
    struct *Literal Ltp;
    int tp=*(var_stack->top-1);
    if(tp>0){
        if(LiteralSet[tp].posOcccur){
            for(j=1;j<LookupValue[j].negative;j++){

            }
        }
    }

    */
    for(i=1;i<clauseNumber+1;i++){
        if(ClauseSet[i].Literal_number<3&&ClauseSet[i].Cvalue==UNCERTAIN){
            struct Literal *tp=ClauseSet[i].head;
            while(tp){
                if(valuation[abs(tp->index)]==-1){
                    j= tp->index;

                    return j;
                }
                tp=tp->next;
            }
        }
    }

/*9�����汾
    for(i=1;i<variableNumber+1;i--){//�˰汾��Ӧ9
        if(valuation[i]==-1)
            return i;
    }
*/
//8�����汾413ms
////*
/*
    for(i=1;i<variableNumber+1;i++){//��Ƶ��ѡ
        if(valuation[sord[i]]==-1){
            if(LookupValue[sord[i]].positive>=LookupValue[sord[i]].negative)
                return sord[i];
            else
                return -sord[i];
        }

    }
*/

    //for(i=1;i<variableNumber+1;)
    for(i=1;i<variableNumber+1;i++){//�˰汾��Ӧ8
        if(valuation[i]==-1)
            return i;
    }
//*/

}

void removeClause(struct Literal * literal){//�Ƴ�һ������
    while (literal != NULL) {
        struct Literal * next = literal->next;
        free(literal);
        literal = next;
    }
}

void removeClauseSet(){//�Ƴ��Ӿ伯
    int i;
    if(ClauseSet){
        for(i=0;i<clauseNumber+1;i++){
            if (ClauseSet[i].head != NULL)
                removeClause(ClauseSet[i].head);
        }
        free(ClauseSet);
    }
}

void removeLiteralSet(){
    int i;
    if(LiteralSet){
        for(i=0;i<variableNumber+1;i++){
            free(LiteralSet[i].posOcccur);
            free(LiteralSet[i].negOccur);
        }
        free(LiteralSet);
    }
}

// writes the solution to the given file
int writeSolution(char * filename,int result,int runtime){
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return false;
    }
    if(result==SATISFIABLE)
        fprintf(f, "%c %d\n",'s',1);
    else if(result==UNSATISFIABLE)
        fprintf(f, "%c %d\n",'s',0);
    else if(result==UNCERTAIN)
        fprintf(f, "%c %d\n",'s',-1);
  // iterate over valuation array to print the values of each literal
    int i;
    if(result==SATISFIABLE){
        fprintf(f, "%c ",'v');
        for (i = 1; i < variableNumber + 1; i++) {
                if(valuation[i])
                    fprintf(f, "%d ",i);
                else
                    fprintf(f, "%d ",-i);
        }
    }
    fprintf(f, "\n%c %d\n",'t',runtime);
    fclose(f);
    return true;
}


// ʵ�ֵ��Ӿ䴫���㷨,�����Ӿ伯�;����ߺ������ֵ�����ֳ�ͻ����UNSATISFIABLE
int Bcp(int unit_var){

    valuation[abs(unit_var)]=unit_var>0?1:0;//���ݴ����ֵ�������ı丳ֵ

    int i,j;
    if(unit_var>0){//�����Ϊ������
        for(i=1;i<(LookupValue[unit_var].positive)+1;i++){//�������ֵ��Ӿ�Cvalue��Ϊ1
            if(LiteralSet[unit_var].posOcccur){
                //ClauseSet[(LiteralSet[abs(unit_var)].posOcccur[i-1])].Pvalue=ClauseSet[(LiteralSet[abs(unit_var)].posOcccur[i-1])].Cvalue;
                ClauseSet[(LiteralSet[unit_var].posOcccur[i-1])].Cvalue=1;
                /////////////////////////////////////printf("BCP�����Ϊ������%d ,��%d���Ӿ�ֵ��ֵ1,��%d������\n",unit_var,LiteralSet[abs(unit_var)].posOcccur[i-1],ClauseSet[(LiteralSet[abs(unit_var)].posOcccur[i-1])].Literal_number);
            }
        }
        for(i=1;i<(LookupValue[unit_var].negative)+1;i++){//�������ֵ��Ӿ����ּ�ɾ��
            if(LiteralSet[unit_var].negOccur){
                j=LiteralSet[unit_var].negOccur[i-1];
                //printf(" -%d--%d-- ",i,LiteralSet[abs(unit_var)].negOccur[i-1]);
                /*
                tpNode=ClauseSet[j].head;

                while(tpNode){
                    if(tpNode->index==-unit_var){//��ɾ
                        //tpNode.c_flag=1;
                        canChangeList[abs(unit_var)].negative--;
                        break;
                    }
                    tpNode=tpNode->next;
                }
                */
                ClauseSet[j].Literal_number--;//������Literal_number��1
                if(ClauseSet[j].Literal_number==0)
                    ClauseSet[j].Cvalue=CONFLICT;
                /////////////////////////////////////////////printf("BCP�����Ϊ������%d ,��%d��������������1����%d������,ֵΪ%d\n",unit_var,j,ClauseSet[j].Literal_number,ClauseSet[j].Cvalue);
            }
        }
    }
    else if(unit_var<0){//�����Ϊ������
        for(i=1;i<(LookupValue[-unit_var].negative)+1;i++){//�������ֵ��Ӿ�Cvalue��Ϊ1
            if(LiteralSet[-unit_var].negOccur){
                //ClauseSet[(LiteralSet[abs(unit_var)].negOccur[i-1])].Pvalue=ClauseSet[(LiteralSet[abs(unit_var)].negOccur[i-1])].Cvalue;
                ClauseSet[(LiteralSet[-unit_var].negOccur[i-1])].Cvalue=1;
                //////////////////////////////////printf("BCP�����Ϊ������%d ,��%d���Ӿ�ֵ��ֵ1,��%d������\n",unit_var,LiteralSet[abs(unit_var)].negOccur[i-1],ClauseSet[(LiteralSet[abs(unit_var)].negOccur[i-1])].Literal_number);
            }
        }

        for(i=1;i<(LookupValue[-unit_var].positive)+1;i++){//�������ֵ��Ӿ����ּ�ɾ
            if(LiteralSet[-unit_var].posOcccur){
                j=LiteralSet[-unit_var].posOcccur[i-1];
                /*
                tpNode=ClauseSet[j].head;
                while(tpNode){
                    if(tpNode->index==-unit_var){//��ɾ
                        //tpNode.c_flag=1;
                        canChangeList[abs(unit_var)].positive--;
                        break;
                    }
                    tpNode=tpNode->next;
                }
                */
                ClauseSet[j].Literal_number--;//������Literal_number��1
                if(ClauseSet[j].Literal_number==0)
                    ClauseSet[j].Cvalue=CONFLICT;
                ///////////////////////////////////////printf("BCP�����Ϊ������%d ,��%d��������������1����%d������,ֵΪ%d\n",unit_var,j,ClauseSet[j].Literal_number,ClauseSet[j].Cvalue);
            }
        }
    }
    return true;
}

int backtrack(int backtrackIndex){//�൱�ڵ��Ӿ䴫���ķ�����
    //��valution����ֵ�ı䣬
    //���޸ĵ��Ӿ��Cvalue״̬������������ԭ����ԭcanChangeList
    valuation[abs(backtrackIndex)]=-1;
    int i,j,Cvalue_flag;
    if(backtrackIndex>0){//������
        for(i=1;i<LookupValue[backtrackIndex].positive+1;i++){//���������ֵ��־�ֵ��ԭ
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].posOcccur){
                j=LiteralSet[abs(backtrackIndex)].posOcccur[i-1];

                struct Literal* Literaltp=ClauseSet[j].head;
                while(Literaltp){
                    if(Literaltp->index>0&&Literaltp->index!=backtrackIndex){
                        if(valuation[Literaltp->index]==1){
                            //printf("���� ����%d ��%d���Ӿ�ֵ��ԭΪ1",backtrackIndex,j)
                            Cvalue_flag=1;
                            break;
                        }
                    //ClauseSet[j].Cvalue=UNCERTAIN;
                    }
                    else if(Literaltp->index<0&&Literaltp->index!=backtrackIndex){
                        if(valuation[-Literaltp->index]==0){
                            Cvalue_flag=1;
                            break;
                        }
                    }
                    Literaltp=Literaltp->next;
                }
                if(Cvalue_flag==0)
                    ClauseSet[j].Cvalue=UNCERTAIN;
            }
        }

        for(i=1;i<LookupValue[backtrackIndex].negative+1;i++){//���������ֵ��־��������ָ�
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].negOccur){
                j=LiteralSet[abs(backtrackIndex)].negOccur[i-1];
                ClauseSet[j].Literal_number++;
                //canChangeList[abs(backtrackIndex)].negative++;//��ԭcanchhangelist
                struct Literal* Literaltp=ClauseSet[j].head;
                while(Literaltp){
                    if(Literaltp->index>0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==1){
                            //printf("���� ����%d ��%d���Ӿ�ֵ��ԭΪ1",backtrackIndex,j)
                            Cvalue_flag=1;
                            break;
                        }
                    //ClauseSet[j].Cvalue=UNCERTAIN;
                    }
                    else if(Literaltp->index<0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==0){
                            Cvalue_flag=1;
                            break;
                        }
                    }
                    Literaltp=Literaltp->next;
                }
                if(Cvalue_flag==0)
                    ClauseSet[j].Cvalue=UNCERTAIN;
            }
            //�ָ���ɾ
            //.........
        }
    }


    if(backtrackIndex<0){//������
        for(i=1;i<LookupValue[abs(backtrackIndex)].negative+1;i++){//���������ֵ��־�ֵ
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].negOccur){
                j=LiteralSet[abs(backtrackIndex)].negOccur[i-1];
                struct Literal* Literaltp=ClauseSet[j].head;
                while(Literaltp){
                    if(Literaltp->index>0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==1){
                            Cvalue_flag=1;
                            break;
                        }
                    //ClauseSet[j].Cvalue=UNCERTAIN;
                    }
                    else if(Literaltp->index<0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==0){
                            Cvalue_flag=1;
                            break;
                        }
                    }
                    Literaltp=Literaltp->next;
                }
                if(Cvalue_flag==0)
                    ClauseSet[j].Cvalue=UNCERTAIN;
            }
        }
        for(i=1;i<LookupValue[abs(backtrackIndex)].positive+1;i++){//���������ֵ��־��������ָ����������ֵļ�ɾ���
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].posOcccur){
                j=LiteralSet[abs(backtrackIndex)].posOcccur[i-1];
                ClauseSet[j].Literal_number++;
                //canChangeList[abs(backtrackIndex)].positive++;//��ԭcanchhangelist

                struct Literal* Literaltp=ClauseSet[j].head;
                while(Literaltp){
                    if(Literaltp->index>0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==1){
                            Cvalue_flag=1;
                            break;
                        }
                    //ClauseSet[j].Cvalue=UNCERTAIN;
                    }
                    else if(Literaltp->index<0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==0){
                            Cvalue_flag=1;
                            break;
                        }
                    }
                    Literaltp=Literaltp->next;
                }
                if(Cvalue_flag==0)
                    ClauseSet[j].Cvalue=UNCERTAIN;
            }
            //�ָ���ɾ
            //.........
        }
    }
    return true;
}


// ����Ӿ伯�ĵ�ǰ״̬�Ƿ����������
int checkSolution(){
    int i;
    for(i=1;i<clauseNumber+1;i++){
        //printf("%d\n",clauseNumber);
        //printf("checkSolution ��%d��������%d������  ",i,ClauseSet[i].Literal_number);
        if (ClauseSet[i].Cvalue==CONFLICT)//bcp���Ӿ�Ϊ�գ�������
            return CONFLICT;
    }

    for(i=1;i<clauseNumber+1;i++){
        if(ClauseSet[i].Cvalue==UNCERTAIN)
            return UNCERTAIN;
    }
    return SATISFIABLE;
}



int dpll2(){
    //int Set_status=preprocess();//Ԥ����,c_status��ŵ�ǰ�Ӿ伯��״̬
    /*
    if(Set_status!=UNCERTAIN)
        return Set_status;
    */
    int Set_status,debug=1;
    int literalIndex,conflictIndex,i;//����bcp������
    literalIndex=chooseLiteral();
    Push(&var_stack,literalIndex);//��ջ,bcp
    LiteralSet[abs(literalIndex)].chooseflag=1;
    while(1){
        while(1){
            //////////////////////////////////printf("---------------%d-------------------\n",debug++);
            ////////////////////////////////////printf("literalIndex: %d  ",literalIndex);
            Set_status=Bcp(literalIndex);//����BCP����

            //printf(" --%d** ",LiteralSet[2].negOccur[0]);
            Set_status=checkSolution();//�ж��Ӿ伯״̬

            //////////////////////////////////printf("\nSet_status: %d \n\n",Set_status);
/*
            int debug2;
            for(debug2=1;debug2<variableNumber+1;debug2++){
                printf("%dֵΪ%d. ",debug2,valuation[debug2]);
            }
            printf("\n");
*/
             /*
            if(Set_status==CONFLICT){//���ڳ�ͻ
                blevel=analyze_conflict();//������������ͻblevel��ŵ��ǻ��ݵ�Ŀ�ľ��߲��
                if(blevel==0)//��������߲���һ���ͻ����ʽ������
                    return UNSATISFIABLE;
                else
                    backtrack(blevel);
            }
            */
            while(Set_status==CONFLICT){//���ڳ�ͻ

                    conflictIndex=Pop(&var_stack);
                    ///////////////////////////printf("���� %d\n",conflictIndex);
                    backtrack(conflictIndex);

                    if(LiteralSet[abs(conflictIndex)].chooseflag==1){
                        literalIndex=-conflictIndex;//���෴����
                        LiteralSet[abs(literalIndex)].chooseflag=2;
                        Push(&var_stack,literalIndex);//��ջ,bcp
                        break;
                    }
                    else if(LiteralSet[abs(conflictIndex)].chooseflag==2){
                        if(!(var_stack.base==var_stack.top)){//ջ�ǿ�
                            LiteralSet[abs(conflictIndex)].chooseflag=0;
                        }
                        else//ջ��,unsat
                            return UNSATISFIABLE;
                    }
            }
            if(Set_status==SATISFIABLE){///////////////????????
                for(i=1;i<variableNumber+1;i++){
                    if (valuation[i]==-1)
                        valuation[i]=1;
                }
                return SATISFIABLE;
            }
            else if(Set_status==UNCERTAIN){

                literalIndex = chooseLiteral();//ѡһ��δ��ջ�ı�Ԫ
                //////////////
                ///////////////////////printf("literalIndex: %d bcphelp: %d\n",literalIndex,bcphelp);
                LiteralSet[abs(literalIndex)].chooseflag=1;
                Push(&var_stack,literalIndex);//��ջ

                break;
            }
        }
    }
}
