#ifndef SOLVER2_H_INCLUDED
#define SOLVER2_H_INCLUDED

#define SATISFIABLE 1
#define UNSATISFIABLE -1
#define UNCERTAIN 0
#define CONFLICT -2
int *valuation;//记录各变量赋值的数组，初值初始化为-1，表示不确定
int variableNumber,clauseNumber;//记录变元数和子句数量
int *sord;
struct Literal {//文字节点
    //struct Literal *prev;//指向子句的上一文字结点
    struct Literal * next; // 指向子句的下一文字结点
    int index;//索引
    //int c_flag;//1表示文字被假删除,当前回溯层
//    int p_flag;//1表示文字被假删除，前一回溯层
};

struct Clause {
    struct Literal * head; // 指向子句的第一个文字
    //struct Clause * next; // 指向集合中的下一个子句
    int Cvalue;//当前回溯层子句状态，初始为uncertain
    //int Pvalue;//上一回溯层子句状态，初始为uncertain
    int Literal_number;// 子句中文字数量
    //int Clause_order;//标记子句次序
};
struct Clause *ClauseSet;//创建一个子句集邻接表

struct LiterValueList{//检查表
    int positive;//值为正的数目
    int negative;//值为负的数目
};

struct LiterValueList *LookupValue;//记录所有变量正负文字数目的检查表
//struct LiterValueList *canChangeList;//可变化的检查表

struct LiteralLink{//文字链接表
    int * posOcccur;//标记正文字出现的子句索引
    int * negOccur;//标记负文字出现的子句索引
    int chooseflag;//标记该层回溯次数，初始为0
};

struct LiteralLink * LiteralSet;

struct Literal_stack{
    int *base;
    int *top;//栈顶指针，指向栈顶元素上一个位置
};

struct Literal_stack var_stack;
#endif // SOLVER2_H_INCLUDED
