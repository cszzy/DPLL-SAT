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
int * valuation; // �ڵݹ��ڼ���ʣ�ȫ����������global valuation array for ease of access during recursion
struct Literal {//���ֽڵ�
  struct Literal * next; // ָ���Ӿ����һ���ֽ��
  int index;//����
};

struct Clause {
  struct Literal * head; // ָ���Ӿ�ĵ�һ������
  struct Clause * next; // ָ�򼯺��е���һ���Ӿ�
  int Literal_number;// �Ӿ�����������
};

struct LiteralIndex{//�����������Щ�־����
    struct Clause *p_occur;
    struct Clause *n_occur;
};

//struct LiteralIndex **;

//��������ʼ��������һ�����Ӿ�
struct Clause * createClause();
// ��������ʼ��������һ��������
struct Literal * createLiteral();
// signal function
int sign(int num);
// �ҵ����Ӿ䲢�������ĵ�λ��������������
int findUnitClause(struct Clause *root);
// ʵ�ֵ��Ӿ䴫���㷨
int unitPropagation(struct Clause *root);
// ͨ�����������Ӿ��ҵ�������(ֻ��һ����̬���֣�
int findPureLiteral(struct Clause * root);
//ʵ�ִ����������㷨
int pureLiteralElimination(struct Clause * root);
//�õ���������

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
