#ifndef SOLVER_H_INCLUDED
#define SOLVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DEBUG 0
#define SATISFIABLE 1
#define UNSATISFIABLE -1
#define UNCERTAIN 0
int clauseNumber, variableNumber;
int * valuation; // 在递归期间访问，全局评估数组global valuation array for ease of access during recursion
struct Literal {//文字节点
  struct Literal * next; // 指向子句的下一文字结点
  int index;//索引
};

struct Clause {
  struct Literal * head; // 指向子句的第一个文字
  struct Clause * next; // 指向集合中的下一个子句
  int Literal_number;// 子句中文字数量
};

struct LiteralIndex{//标记文字在哪些字句出现
    struct Clause *p_occur;
    struct Clause *n_occur;
};

//struct LiteralIndex **;

//创建，初始化并返回一个空子句
struct Clause * createClause();
// 创建，初始化并返回一个空文字
struct Literal * createLiteral();
// signal function
int sign(int num);
// 找到单子句并返回它的单位传播的文字索引
int findUnitClause(struct Clause *root);
// 实现单子句传播算法
int unitPropagation(struct Clause *root);
// 通过遍历所有子句找到纯文字(只以一种形态出现）
int findPureLiteral(struct Clause * root);
//实现纯文字消除算法
int pureLiteralElimination(struct Clause * root);
//用电脑玩数独

int areAllClausesUnit(struct Clause * root);
int containsEmptyClause(struct Clause * root);
int checkSolution(struct Clause * root);
int chooseLiteral(struct Clause * root);
struct Clause * cloneClause(struct Clause * origin);
struct Clause * branch(struct Clause * root, int literalIndex);
void removeLiteral(struct Literal * literal);
void removeClause(struct Clause * root);
int dpll(struct Clause * root);
int writeSolution(struct Clause * root, char * filename,int result,int runtime);
#endif // SOLVER_H_INCLUDED
