//基于DPLL算法框架，实现SAT算例的求解。
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"
#include "Sudoku.h"
//创建，初始化并返回一个空子句
struct Clause * createClause(){
    struct Clause * instance = malloc(sizeof(struct Clause));
    if(!instance)
        exit(1);
    instance->head = NULL;
    instance->next = NULL;
    instance->Literal_number=0;
    return instance;
}

// 创建，初始化并返回一个空文字
struct Literal * createLiteral(){
    struct Literal * instance = malloc(sizeof(struct Literal));
    if(!instance)
        exit(1);
    instance->next = NULL;
    instance->index = 0;
    return instance;
}

// signal function
int sign(int num){
  return (num > 0) - (num < 0);
}

// 找到单子句并返回它的单位传播的文字索引
int findUnitClause(struct Clause *root){
    struct Clause *fu=root;
    while(fu!=NULL){
        if(fu->head==NULL)
            continue;
        if(fu->head->next==NULL)
            return fu->head->index;
        fu=fu->next;
    }
    return 0;
}

// 实现单子句传播算法,没有单子句返回0
int unitPropagation(struct Clause *root){
    int up=findUnitClause(root);
    if(up==0)
        return 0;
    valuation[abs(up)]=up>0?1:0;
    struct Clause *tp=root,*prev=root;
    while(tp!=NULL){
        struct Literal *cL=tp->head;
        struct Literal *pL=createLiteral();
        while(cL!=NULL){
            if(cL->index==up){// 找到单个文字删除整个子句并重新调整指针
                if(tp==root){// 如果我们删除的是第一个子句，根指针必须改变
                    *root=*(root->next);////////////////
                    tp=NULL;
                }
                else{
                    prev->next=tp->next;
                    tp=prev;
                }
                //if (tp->head != NULL)//释放tp指向的子句
                  //      removeLiteral(root->head);
                   // free(tp);
                break;
            }
            else if(cL->index==-up){// 删除单个文字，将其从子句中删除。 其他文字应该留下来
                if(cL==tp->head){
                    tp->head=cL->next;//如果它是子句中的第一个文字，则头指针必须改变
                }
                else{
                    pL->next=cL->next;

                }
                //free(cL);
                cL=pL;
                continue;
            }
            pL=cL;
            cL=cL->next;
        }
        prev=tp;
        tp=tp==NULL?root:tp->next;
    }
    return 1;
}


// 如果子句集包含一个没有文字的空子句，则返回0
int containsEmptyClause(struct Clause * root){
    struct Clause* tp = root;
    while (tp != NULL){
        if(tp->head == NULL)
            return 1;//有空子句返回1
        tp = tp->next;
    }
    return 0;
}

//检查所有剩余的子句是否包含非冲突的文字，若不包含任何一对冲突文字，则已满足sat
//即对于子句集中剩余的每个文字值，应该存在正或负索引。 如果是这种情况，那么可满足性就会得到解决。
//仍然是利用纯文字规则
int areAllClausesUnit(struct Clause * root){
    int * literalLookup = (int*) calloc(variableNumber + 1, sizeof(int));
    struct Clause* tp = root;
    while (tp != NULL){
        struct Literal * L = tp->head;
        while (L != NULL){
            int seen = literalLookup[abs(L->index)];
            if (seen == 0)
                literalLookup[abs(L->index)] = sign(L->index);
      // 如果我们以前看过这个带有相反符号的文字，则返回false
            else if ((seen == -1 && sign(L->index) == 1)||(seen == 1 && sign(L->index) == -1))
                return 0;
            L = L->next;
        }
    tp = tp->next;
    }

  // 如果我们到达这里，那意味着子句集不包含冲突的文字
  // 最后一次迭代子句集来决定他们的估值
    tp = root;
    while (tp != NULL){
        struct Literal * L = tp->head;
        while (L != NULL){
            valuation[abs(L->index)] = L->index > 0 ? 1 : 0;
            L = L->next;
        }
        tp = tp->next;
    }

  // 返回true以终止dpll
    return 1;
}

// 检查子句集的当前状态是否代表解决方案
int checkSolution(struct Clause * root){
    if (containsEmptyClause(root))//子句为空，不满足
        return UNSATISFIABLE;
    if (areAllClausesUnit(root))//子句集不包含冲突的文字
        return SATISFIABLE;
    return UNCERTAIN;
}

// 返回随机文字索引以执行分支
int chooseLiteral(struct Clause * root){
    //只返回第一个字面值，它不会改变结果，但最好使用更智能的方法来提高速度
    // (e.g. 选择频率最高的文字)
    return root->head->index;
}

// 深度克隆构造新子句和文字结构的子句deep clones a clause constructing a new clause and literal structs
struct Clause * cloneClause(struct Clause * origin){
    struct Clause * cloneClause = createClause();
    struct Literal * iteratorLiteral = origin->head;
    struct Literal * previousLiteral = NULL;

  // 迭代子句克隆所有文字iterate over the clause to clone literals as well
    while (iteratorLiteral != NULL){
        struct Literal * literalClone = createLiteral();
        literalClone->index = iteratorLiteral->index;
        if (cloneClause->head == NULL) {
        cloneClause->head = literalClone;
        }
        if (previousLiteral != NULL) {
        previousLiteral->next = literalClone;
        }
        previousLiteral = literalClone;
        iteratorLiteral = iteratorLiteral->next;
    }
    return cloneClause;
}

// 深度克隆一个子句集并使用给定的文字索引注入一个新的单子句
// 这就是分支的执行方式
struct Clause * branch(struct Clause * root, int literalIndex){
  // 设置文字的估值
  //我们可能会回溯并且这个估值可能会过时，但这并不重要，因为回溯的分支将用新的估值覆盖它
    valuation[abs(literalIndex)] = literalIndex > 0 ? 1 : 0;
    struct Clause * newClone = NULL,
                  * pClause = NULL,
                  * tp = root;
  // 依次克隆所有子句
    while (tp  != NULL){
        struct Clause * clone = cloneClause(tp );
        if (newClone == NULL) {
            newClone = clone;
        }
        if (pClause != NULL) {
            pClause->next = clone;
        }
        pClause = clone;
        tp = tp ->next;
    }
  //将它作为新的根添加到第一个位置，因为我们要确保在以下的单位传播中选择相同的literalIndex
    struct Clause * aClause = createClause();
    struct Literal * aLiteral = createLiteral();
    aLiteral->index = literalIndex;
    aClause->head = aLiteral;
    aClause->next = newClone;
    return aClause;
}

void removeLiteral(struct Literal * literal){//移除单个句子
    while (literal != NULL) {
        struct Literal * next = literal->next;
        free(literal);
        literal = next;
    }
}

void removeClause(struct Clause * root){//移除子句集
    while (root != NULL) {
        struct Clause * next = root->next;
        if (root->head != NULL)
            removeLiteral(root->head);
        free(root);
        root = next;
    }
}

// 具有递归回溯的DPLL算法
int dpll(struct Clause * root){

  // 首先检查我们是否已经处于解决状态
  int solution = checkSolution(root);
  if (solution != UNCERTAIN){
    removeClause(root);
    return solution;
  }

  // 只要子句集允许，就进行单位传播
  while(1){
    solution = checkSolution(root);
    if (solution != UNCERTAIN){
      removeClause(root);
      return solution;
    }
    if (!unitPropagation(root)) break;
  }
  //unitPropagation(root);
/*
  // 只要子句集允许，就进行纯文字消除
  while(1){
    int solution = checkSolution(root);
    if (solution != UNCERTAIN) {
      removeClause(root);
      return solution;
    }
    if (!pureLiteralElimination(root)) break;
  }
*/
  // 如果我们被卡住了，那么选择一个随机文字并添加在其上分支进行回溯
  int literalIndex = chooseLiteral(root);
  //  - 使用此选定的文字插入新的单元子句，然后递归
  struct Clause *tp=branch(root, literalIndex);
  if (dpll(tp)== SATISFIABLE)
    return SATISFIABLE;
  //
  //removeClause(tp);
  //   - 如果它没有产生解决方案，请尝试使用否定的文字
  return dpll(branch(root,-literalIndex));
}

// writes the solution to the given file
int writeSolution(struct Clause * root, char * filename,int result,int runtime){
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening file!\n");
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
}
