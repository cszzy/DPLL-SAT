//基于DPLL算法框架，实现SAT算例的求解。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver2.h"
#include "Sudoku.h"

//创建，初始化并返回一个空子句
struct Clause * createClause(){
    struct Clause * instance = malloc(sizeof(struct Clause));
    if(!instance)
        exit(1);
    instance->head = NULL;
    //instance->next = NULL;
    instance->Cvalue=0;//初始为未确定
    //instance->Pvalue=0;
    instance->Literal_number=0;
    return instance;
}

// 创建，初始化并返回一个空文字
struct Literal * createLiteral(){
    struct Literal * instance = malloc(sizeof(struct Literal));
    if(!instance)
        exit(1);
    //instance->prev =NULL;
    instance->next = NULL;
    instance->index = 0;//文字值
    //instance->c_flag=0;
    //instance->p_flag=0;
    return instance;
}

int InitLiteralSet(){//初始化文字邻接表
    int i;
    for(i=1;i<variableNumber+1;i++){//分配空间
        if(LookupValue[i].positive>0)
            LiteralSet[i].posOcccur=(int*)calloc(LookupValue[i].positive,sizeof(int));
        else
            LiteralSet[i].posOcccur=NULL;//没有正文字为空指针
        if(LookupValue[i].negative>0)
            LiteralSet[i].negOccur=(int*)calloc(LookupValue[i].negative,sizeof(int));
        else
            LiteralSet[i].negOccur=NULL;
        LiteralSet[i].chooseflag=0;
    }

    struct Literal *tpNode;
    int var;
    for(var=1;var<variableNumber+1;var++){//遍历每一个文字
        int j=0,k=0;
        for(i=1;i<clauseNumber+1;i++){//创建文字邻接表
            //printf("%d .",debug++);
            tpNode=ClauseSet[i].head;
            while(tpNode){
                if(tpNode->index==var){//正文字
                    LiteralSet[abs(tpNode->index)].posOcccur[j]=i;
                    //printf("-----变元%d在第%d个句子中---",tpNode->index,LiteralSet[abs(tpNode->index)].posOcccur[j]);
                    j++;
                }
                else if(tpNode->index==-var){
                    LiteralSet[abs(tpNode->index)].negOccur[k]=i;
                    //printf("-----变元%d在第%d个句子中---",tpNode->index,LiteralSet[abs(tpNode->index)].negOccur[k]);
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
    L_stack->top=L_stack->base;//初始为空
    return true;
}

int Push(struct Literal_stack* L_stack,int var){//入栈函数
    *L_stack->top=var;
    L_stack->top++;
    return true;
}

int Pop(struct Literal_stack* L_stack){//出栈函数，栈不空返回栈顶元素，栈为空返回false
    if(L_stack->top==L_stack->base)//栈为空
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
int chooseLiteral(){//选择变量
    //只返回第一个字面值，它不会改变结果，但最好使用更智能的方法来提高速度
    // (e.g. 选择频率最高的文字)
    int i,j;
    for(i=1;i<clauseNumber+1;i++){//先找单子句
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

/*9的最快版本
    for(i=1;i<variableNumber+1;i--){//此版本适应9
        if(valuation[i]==-1)
            return i;
    }
*/
//8的最快版本413ms
////*
/*
    for(i=1;i<variableNumber+1;i++){//按频度选
        if(valuation[sord[i]]==-1){
            if(LookupValue[sord[i]].positive>=LookupValue[sord[i]].negative)
                return sord[i];
            else
                return -sord[i];
        }

    }
*/

    //for(i=1;i<variableNumber+1;)
    for(i=1;i<variableNumber+1;i++){//此版本适应8
        if(valuation[i]==-1)
            return i;
    }
//*/

}

void removeClause(struct Literal * literal){//移除一个句子
    while (literal != NULL) {
        struct Literal * next = literal->next;
        free(literal);
        literal = next;
    }
}

void removeClauseSet(){//移除子句集
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


// 实现单子句传播算法,传入子句集和经决策后的文字值，出现冲突返回UNSATISFIABLE
int Bcp(int unit_var){

    valuation[abs(unit_var)]=unit_var>0?1:0;//根据传入的值的正负改变赋值

    int i,j;
    if(unit_var>0){//传入的为正文字
        for(i=1;i<(LookupValue[unit_var].positive)+1;i++){//含正文字的子句Cvalue记为1
            if(LiteralSet[unit_var].posOcccur){
                //ClauseSet[(LiteralSet[abs(unit_var)].posOcccur[i-1])].Pvalue=ClauseSet[(LiteralSet[abs(unit_var)].posOcccur[i-1])].Cvalue;
                ClauseSet[(LiteralSet[unit_var].posOcccur[i-1])].Cvalue=1;
                /////////////////////////////////////printf("BCP传入的为正文字%d ,第%d个子句值赋值1,有%d个文字\n",unit_var,LiteralSet[abs(unit_var)].posOcccur[i-1],ClauseSet[(LiteralSet[abs(unit_var)].posOcccur[i-1])].Literal_number);
            }
        }
        for(i=1;i<(LookupValue[unit_var].negative)+1;i++){//含负文字的子句文字假删，
            if(LiteralSet[unit_var].negOccur){
                j=LiteralSet[unit_var].negOccur[i-1];
                //printf(" -%d--%d-- ",i,LiteralSet[abs(unit_var)].negOccur[i-1]);
                /*
                tpNode=ClauseSet[j].head;

                while(tpNode){
                    if(tpNode->index==-unit_var){//假删
                        //tpNode.c_flag=1;
                        canChangeList[abs(unit_var)].negative--;
                        break;
                    }
                    tpNode=tpNode->next;
                }
                */
                ClauseSet[j].Literal_number--;//文字数Literal_number减1
                if(ClauseSet[j].Literal_number==0)
                    ClauseSet[j].Cvalue=CONFLICT;
                /////////////////////////////////////////////printf("BCP传入的为正文字%d ,第%d个句子文字数减1，有%d个文字,值为%d\n",unit_var,j,ClauseSet[j].Literal_number,ClauseSet[j].Cvalue);
            }
        }
    }
    else if(unit_var<0){//传入的为负文字
        for(i=1;i<(LookupValue[-unit_var].negative)+1;i++){//含负文字的子句Cvalue记为1
            if(LiteralSet[-unit_var].negOccur){
                //ClauseSet[(LiteralSet[abs(unit_var)].negOccur[i-1])].Pvalue=ClauseSet[(LiteralSet[abs(unit_var)].negOccur[i-1])].Cvalue;
                ClauseSet[(LiteralSet[-unit_var].negOccur[i-1])].Cvalue=1;
                //////////////////////////////////printf("BCP传入的为负文字%d ,第%d个子句值赋值1,有%d个文字\n",unit_var,LiteralSet[abs(unit_var)].negOccur[i-1],ClauseSet[(LiteralSet[abs(unit_var)].negOccur[i-1])].Literal_number);
            }
        }

        for(i=1;i<(LookupValue[-unit_var].positive)+1;i++){//含正文字的子句文字假删
            if(LiteralSet[-unit_var].posOcccur){
                j=LiteralSet[-unit_var].posOcccur[i-1];
                /*
                tpNode=ClauseSet[j].head;
                while(tpNode){
                    if(tpNode->index==-unit_var){//假删
                        //tpNode.c_flag=1;
                        canChangeList[abs(unit_var)].positive--;
                        break;
                    }
                    tpNode=tpNode->next;
                }
                */
                ClauseSet[j].Literal_number--;//文字数Literal_number减1
                if(ClauseSet[j].Literal_number==0)
                    ClauseSet[j].Cvalue=CONFLICT;
                ///////////////////////////////////////printf("BCP传入的为负文字%d ,第%d个句子文字数减1，有%d个文字,值为%d\n",unit_var,j,ClauseSet[j].Literal_number,ClauseSet[j].Cvalue);
            }
        }
    }
    return true;
}

int backtrack(int backtrackIndex){//相当于单子句传播的反过程
    //将valution数组值改变，
    //将修改的子句的Cvalue状态和文字数量还原并还原canChangeList
    valuation[abs(backtrackIndex)]=-1;
    int i,j,Cvalue_flag;
    if(backtrackIndex>0){//正文字
        for(i=1;i<LookupValue[backtrackIndex].positive+1;i++){//将含正文字的字句值还原
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].posOcccur){
                j=LiteralSet[abs(backtrackIndex)].posOcccur[i-1];

                struct Literal* Literaltp=ClauseSet[j].head;
                while(Literaltp){
                    if(Literaltp->index>0&&Literaltp->index!=backtrackIndex){
                        if(valuation[Literaltp->index]==1){
                            //printf("回溯 传入%d 第%d个子句值还原为1",backtrackIndex,j)
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

        for(i=1;i<LookupValue[backtrackIndex].negative+1;i++){//将含负文字的字句文字数恢复
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].negOccur){
                j=LiteralSet[abs(backtrackIndex)].negOccur[i-1];
                ClauseSet[j].Literal_number++;
                //canChangeList[abs(backtrackIndex)].negative++;//还原canchhangelist
                struct Literal* Literaltp=ClauseSet[j].head;
                while(Literaltp){
                    if(Literaltp->index>0&&Literaltp->index!=backtrackIndex){
                        if(valuation[abs(Literaltp->index)]==1){
                            //printf("回溯 传入%d 第%d个子句值还原为1",backtrackIndex,j)
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
            //恢复假删
            //.........
        }
    }


    if(backtrackIndex<0){//负文字
        for(i=1;i<LookupValue[abs(backtrackIndex)].negative+1;i++){//将含负文字的字句值
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
        for(i=1;i<LookupValue[abs(backtrackIndex)].positive+1;i++){//将含正文字的字句文字数恢复，将负文字的假删解除
            Cvalue_flag=0;
            if(LiteralSet[abs(backtrackIndex)].posOcccur){
                j=LiteralSet[abs(backtrackIndex)].posOcccur[i-1];
                ClauseSet[j].Literal_number++;
                //canChangeList[abs(backtrackIndex)].positive++;//还原canchhangelist

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
            //恢复假删
            //.........
        }
    }
    return true;
}


// 检查子句集的当前状态是否代表解决方案
int checkSolution(){
    int i;
    for(i=1;i<clauseNumber+1;i++){
        //printf("%d\n",clauseNumber);
        //printf("checkSolution 第%d个句子有%d个文字  ",i,ClauseSet[i].Literal_number);
        if (ClauseSet[i].Cvalue==CONFLICT)//bcp后子句为空，不满足
            return CONFLICT;
    }

    for(i=1;i<clauseNumber+1;i++){
        if(ClauseSet[i].Cvalue==UNCERTAIN)
            return UNCERTAIN;
    }
    return SATISFIABLE;
}



int dpll2(){
    //int Set_status=preprocess();//预处理,c_status存放当前子句集的状态
    /*
    if(Set_status!=UNCERTAIN)
        return Set_status;
    */
    int Set_status,debug=1;
    int literalIndex,conflictIndex,i;//用来bcp的索引
    literalIndex=chooseLiteral();
    Push(&var_stack,literalIndex);//入栈,bcp
    LiteralSet[abs(literalIndex)].chooseflag=1;
    while(1){
        while(1){
            //////////////////////////////////printf("---------------%d-------------------\n",debug++);
            ////////////////////////////////////printf("literalIndex: %d  ",literalIndex);
            Set_status=Bcp(literalIndex);//进行BCP过程

            //printf(" --%d** ",LiteralSet[2].negOccur[0]);
            Set_status=checkSolution();//判断子句集状态

            //////////////////////////////////printf("\nSet_status: %d \n\n",Set_status);
/*
            int debug2;
            for(debug2=1;debug2<variableNumber+1;debug2++){
                printf("%d值为%d. ",debug2,valuation[debug2]);
            }
            printf("\n");
*/
             /*
            if(Set_status==CONFLICT){//存在冲突
                blevel=analyze_conflict();//诊断引擎分析冲突blevel存放的是回溯的目的决策层次
                if(blevel==0)//如果根决策层上一层冲突，则公式不满足
                    return UNSATISFIABLE;
                else
                    backtrack(blevel);
            }
            */
            while(Set_status==CONFLICT){//存在冲突

                    conflictIndex=Pop(&var_stack);
                    ///////////////////////////printf("回溯 %d\n",conflictIndex);
                    backtrack(conflictIndex);

                    if(LiteralSet[abs(conflictIndex)].chooseflag==1){
                        literalIndex=-conflictIndex;//用相反文字
                        LiteralSet[abs(literalIndex)].chooseflag=2;
                        Push(&var_stack,literalIndex);//入栈,bcp
                        break;
                    }
                    else if(LiteralSet[abs(conflictIndex)].chooseflag==2){
                        if(!(var_stack.base==var_stack.top)){//栈非空
                            LiteralSet[abs(conflictIndex)].chooseflag=0;
                        }
                        else//栈空,unsat
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

                literalIndex = chooseLiteral();//选一个未入栈的变元
                //////////////
                ///////////////////////printf("literalIndex: %d bcphelp: %d\n",literalIndex,bcphelp);
                LiteralSet[abs(literalIndex)].chooseflag=1;
                Push(&var_stack,literalIndex);//入栈

                break;
            }
        }
    }
}
