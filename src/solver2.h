#ifndef SOLVER2_H_INCLUDED
#define SOLVER2_H_INCLUDED

#define SATISFIABLE 1
#define UNSATISFIABLE -1
#define UNCERTAIN 0
#define CONFLICT -2
int *valuation;//��¼��������ֵ�����飬��ֵ��ʼ��Ϊ-1����ʾ��ȷ��
int variableNumber,clauseNumber;//��¼��Ԫ�����Ӿ�����
int *sord;
struct Literal {//���ֽڵ�
    //struct Literal *prev;//ָ���Ӿ����һ���ֽ��
    struct Literal * next; // ָ���Ӿ����һ���ֽ��
    int index;//����
    //int c_flag;//1��ʾ���ֱ���ɾ��,��ǰ���ݲ�
//    int p_flag;//1��ʾ���ֱ���ɾ����ǰһ���ݲ�
};

struct Clause {
    struct Literal * head; // ָ���Ӿ�ĵ�һ������
    //struct Clause * next; // ָ�򼯺��е���һ���Ӿ�
    int Cvalue;//��ǰ���ݲ��Ӿ�״̬����ʼΪuncertain
    //int Pvalue;//��һ���ݲ��Ӿ�״̬����ʼΪuncertain
    int Literal_number;// �Ӿ�����������
    //int Clause_order;//����Ӿ����
};
struct Clause *ClauseSet;//����һ���Ӿ伯�ڽӱ�

struct LiterValueList{//����
    int positive;//ֵΪ������Ŀ
    int negative;//ֵΪ������Ŀ
};

struct LiterValueList *LookupValue;//��¼���б�������������Ŀ�ļ���
//struct LiterValueList *canChangeList;//�ɱ仯�ļ���

struct LiteralLink{//�������ӱ�
    int * posOcccur;//��������ֳ��ֵ��Ӿ�����
    int * negOccur;//��Ǹ����ֳ��ֵ��Ӿ�����
    int chooseflag;//��Ǹò���ݴ�������ʼΪ0
};

struct LiteralLink * LiteralSet;

struct Literal_stack{
    int *base;
    int *top;//ջ��ָ�룬ָ��ջ��Ԫ����һ��λ��
};

struct Literal_stack var_stack;
#endif // SOLVER2_H_INCLUDED
