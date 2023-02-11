//����DPLL�㷨��ܣ�ʵ��SAT��������⡣
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"
#include "Sudoku.h"
//��������ʼ��������һ�����Ӿ�
struct Clause * createClause(){
    struct Clause * instance = malloc(sizeof(struct Clause));
    if(!instance)
        exit(1);
    instance->head = NULL;
    instance->next = NULL;
    instance->Literal_number=0;
    return instance;
}

// ��������ʼ��������һ��������
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

// �ҵ����Ӿ䲢�������ĵ�λ��������������
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

// ʵ�ֵ��Ӿ䴫���㷨,û�е��Ӿ䷵��0
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
            if(cL->index==up){// �ҵ���������ɾ�������Ӿ䲢���µ���ָ��
                if(tp==root){// �������ɾ�����ǵ�һ���Ӿ䣬��ָ�����ı�
                    *root=*(root->next);////////////////
                    tp=NULL;
                }
                else{
                    prev->next=tp->next;
                    tp=prev;
                }
                //if (tp->head != NULL)//�ͷ�tpָ����Ӿ�
                  //      removeLiteral(root->head);
                   // free(tp);
                break;
            }
            else if(cL->index==-up){// ɾ���������֣�������Ӿ���ɾ���� ��������Ӧ��������
                if(cL==tp->head){
                    tp->head=cL->next;//��������Ӿ��еĵ�һ�����֣���ͷָ�����ı�
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


// ����Ӿ伯����һ��û�����ֵĿ��Ӿ䣬�򷵻�0
int containsEmptyClause(struct Clause * root){
    struct Clause* tp = root;
    while (tp != NULL){
        if(tp->head == NULL)
            return 1;//�п��Ӿ䷵��1
        tp = tp->next;
    }
    return 0;
}

//�������ʣ����Ӿ��Ƿ�����ǳ�ͻ�����֣����������κ�һ�Գ�ͻ���֣���������sat
//�������Ӿ伯��ʣ���ÿ������ֵ��Ӧ�ô������������� ����������������ô�������Ծͻ�õ������
//��Ȼ�����ô����ֹ���
int areAllClausesUnit(struct Clause * root){
    int * literalLookup = (int*) calloc(variableNumber + 1, sizeof(int));
    struct Clause* tp = root;
    while (tp != NULL){
        struct Literal * L = tp->head;
        while (L != NULL){
            int seen = literalLookup[abs(L->index)];
            if (seen == 0)
                literalLookup[abs(L->index)] = sign(L->index);
      // ���������ǰ������������෴���ŵ����֣��򷵻�false
            else if ((seen == -1 && sign(L->index) == 1)||(seen == 1 && sign(L->index) == -1))
                return 0;
            L = L->next;
        }
    tp = tp->next;
    }

  // ������ǵ����������ζ���Ӿ伯��������ͻ������
  // ���һ�ε����Ӿ伯���������ǵĹ�ֵ
    tp = root;
    while (tp != NULL){
        struct Literal * L = tp->head;
        while (L != NULL){
            valuation[abs(L->index)] = L->index > 0 ? 1 : 0;
            L = L->next;
        }
        tp = tp->next;
    }

  // ����true����ֹdpll
    return 1;
}

// ����Ӿ伯�ĵ�ǰ״̬�Ƿ����������
int checkSolution(struct Clause * root){
    if (containsEmptyClause(root))//�Ӿ�Ϊ�գ�������
        return UNSATISFIABLE;
    if (areAllClausesUnit(root))//�Ӿ伯��������ͻ������
        return SATISFIABLE;
    return UNCERTAIN;
}

// �����������������ִ�з�֧
int chooseLiteral(struct Clause * root){
    //ֻ���ص�һ������ֵ��������ı����������ʹ�ø����ܵķ���������ٶ�
    // (e.g. ѡ��Ƶ����ߵ�����)
    return root->head->index;
}

// ��ȿ�¡�������Ӿ�����ֽṹ���Ӿ�deep clones a clause constructing a new clause and literal structs
struct Clause * cloneClause(struct Clause * origin){
    struct Clause * cloneClause = createClause();
    struct Literal * iteratorLiteral = origin->head;
    struct Literal * previousLiteral = NULL;

  // �����Ӿ��¡��������iterate over the clause to clone literals as well
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

// ��ȿ�¡һ���Ӿ伯��ʹ�ø�������������ע��һ���µĵ��Ӿ�
// ����Ƿ�֧��ִ�з�ʽ
struct Clause * branch(struct Clause * root, int literalIndex){
  // �������ֵĹ�ֵ
  //���ǿ��ܻ���ݲ��������ֵ���ܻ��ʱ�����Ⲣ����Ҫ����Ϊ���ݵķ�֧�����µĹ�ֵ������
    valuation[abs(literalIndex)] = literalIndex > 0 ? 1 : 0;
    struct Clause * newClone = NULL,
                  * pClause = NULL,
                  * tp = root;
  // ���ο�¡�����Ӿ�
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
  //������Ϊ�µĸ���ӵ���һ��λ�ã���Ϊ����Ҫȷ�������µĵ�λ������ѡ����ͬ��literalIndex
    struct Clause * aClause = createClause();
    struct Literal * aLiteral = createLiteral();
    aLiteral->index = literalIndex;
    aClause->head = aLiteral;
    aClause->next = newClone;
    return aClause;
}

void removeLiteral(struct Literal * literal){//�Ƴ���������
    while (literal != NULL) {
        struct Literal * next = literal->next;
        free(literal);
        literal = next;
    }
}

void removeClause(struct Clause * root){//�Ƴ��Ӿ伯
    while (root != NULL) {
        struct Clause * next = root->next;
        if (root->head != NULL)
            removeLiteral(root->head);
        free(root);
        root = next;
    }
}

// ���еݹ���ݵ�DPLL�㷨
int dpll(struct Clause * root){

  // ���ȼ�������Ƿ��Ѿ����ڽ��״̬
  int solution = checkSolution(root);
  if (solution != UNCERTAIN){
    removeClause(root);
    return solution;
  }

  // ֻҪ�Ӿ伯�����ͽ��е�λ����
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
  // ֻҪ�Ӿ伯�����ͽ��д���������
  while(1){
    int solution = checkSolution(root);
    if (solution != UNCERTAIN) {
      removeClause(root);
      return solution;
    }
    if (!pureLiteralElimination(root)) break;
  }
*/
  // ������Ǳ���ס�ˣ���ôѡ��һ��������ֲ���������Ϸ�֧���л���
  int literalIndex = chooseLiteral(root);
  //  - ʹ�ô�ѡ�������ֲ����µĵ�Ԫ�Ӿ䣬Ȼ��ݹ�
  struct Clause *tp=branch(root, literalIndex);
  if (dpll(tp)== SATISFIABLE)
    return SATISFIABLE;
  //
  //removeClause(tp);
  //   - �����û�в�������������볢��ʹ�÷񶨵�����
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
