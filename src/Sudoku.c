//
// Created by littleadd on 2019-02-08.
//
#include "Sudoku.h"
#include <stdio.h>
//int soduko[9][9]={0};//��ʼ������,��������cnf�ļ�
//int knockout_flag[9][9]={0};//��¼�ڿպ�������Ϊ0�ĸ��ӣ��ȿ����޸ĵĸ��ӣ�
//int count=0;

int readsodukofile(char *filename) //���ļ�
{
    FILE *fp;
    fp=fopen(filename,"r");
    if (fp==NULL)   //���ļ�ʧ��
    {
        printf("File open error!\n ");
        return false;
    }

    char c_result,gettt;
    int num_result;
    fscanf(fp,"%c %d\n%c ",&c_result,&num_result,&gettt);

    int num_value;
    int i=1;
    for(i=1;i<=729;i++){
        fscanf(fp,"%d ",&num_value);
        if(num_value>0)
            sodukoanswer[(abs(num_value)-1)/9+1]=(abs(num_value)-1)%9+1;
    }
/*
    for(i=1;i<=81;i++)
        printf("%d: %d ",i,sodukoanswer[i]);
    //������ļ��������ȡ����Ԫ�ػָ�˳������ڲ�ͬ������ṹ����ͨ����ȡ������Ԫ�ػָ��ڴ��е�����ṹ��
    printf("\n");
*/
    fclose(fp);  //�ر��ļ�
    return true;
}

void setcolor(int color)
{
	HANDLE hConsoleWnd;
    hConsoleWnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsoleWnd,color);
}

int generateRandNum(){//���������
    return rand()%9+1;//����1-9�������
}

int printsoduko(){
    int i=0,j=0;
    printf("  ");
    for(i=0;i<9;i++){
        printf("%2d",i+1);
    }
    printf("\n\n");
    for(i=0;i<9;i++){
        printf("%-2d",i+1);
        for(j=0;j<9;j++){
            if(soduko[i][j]&&knockout_flag[i][j]==0)//ԭ����
                printf("%2d",soduko[i][j]);
            else if(soduko[i][j]==0){//0Ϊ��ɫ
                setcolor(RED);
                printf("%2d",soduko[i][j]);
            }
            else{
                setcolor(GREEN);
                printf("%2d",soduko[i][j]);
            }
            setcolor(WHITE);//��ԭϵͳɫ
        }
        printf("\n");
    }
    printf("\n");
    return true;
}

int issodukoFull(){//�ж������Ƿ���ȫ������
	int flag[81] = {0};
    int i,j;
	for(i = 0; i < 9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if(soduko[i][j] != 0)
                flag[i*9 + j] = 1;
		}
	}

	for(i = 0; i < 81; i++)
	{
		if(flag[i] == 0)
			return false;//û����
	}

	return true;
}

int checkChecklist(int checklist[9]){
    int i = 0;
    for(i = 0; i < 9; i++)
	{
		if(checklist[i] == 0)
		{
			return false;//��0
		}
	}
	return true;

}

void generateFirstLine(){
    int checklist[9]={0},randnum,addnum=0;
    srand((unsigned)time(NULL));
    while(!checkChecklist(checklist)){
        randnum=generateRandNum();
        if(checklist[randnum - 1]==0)
		{
			checklist[randnum - 1] = 1;
			soduko[addnum/9][addnum % 9] = randnum;
			addnum++;
		}
    }
}


int check()
{
    int i,j,m,n;
	//�����
	for(i = 0; i < 9; i++){
		int tempArr[9] = {0};
		for( j = 0; j < 9; j++){
			if(soduko[i][j] != 0){
				if(tempArr[soduko[i][j] - 1] == 0)
					tempArr[soduko[i][j] - 1] = 1;
				else
					return false;
			}
		}
	}

	//�����
	for(j = 0; j < 9; j++){
		int tempArr[9] = {0};
		for(i = 0; i < 9; i++){
			if(soduko[i][j] != 0){
				if(tempArr[soduko[i][j] - 1] == 0)
					tempArr[soduko[i][j] - 1] = 1;
				else
					return false;
			}
		}
	}

	//���Ź���
	for(m = 0; m < 9; m += 3){
		for(n = 0; n < 9; n += 3){
			int tempArr[9] = {0};
			for(i = 0; i < 3; i++){
				for(j = 0; j < 3; j++){
					if(soduko[m + i][n + j] != 0){
						if(tempArr[soduko[m + i][n + j] - 1] == 0)
							tempArr[soduko[m + i][n + j] - 1] = 1;
						else
							return false;
					}
				}
			}
		}
	}

	return true;
}

int generatesoduko(int rownum){//������������
    if(issodukoFull()){//�ж������Ƿ���ȫ������
            return true;
    }
    int i;
    srand((unsigned int)time(0));
    for(i=1;i<10;i++){
        //printf("%3d",rownum/9);
        soduko[(int)(rownum/9)][rownum%9] = i;
        //if(isNumberFill((rownum/9),rownum%9))
        //printf("%3d",rownum/9);
        if(check()){
            if(generatesoduko(rownum+1))
                goto mdzz;
            else
                soduko[(int)(rownum/9)][rownum%9] = 0;
        }
        else
            soduko[(int)(rownum/9)][rownum%9] = 0;
    }

    return false;
    mdzz:
        return true;
}

int knockout_soduko(int k_soduko[9][9],int hard_level){//�ڿ�
    int rand_row,rand_col;//����кź��к�
    int k=0,knockout_num=hard_level;//knockout_numΪ�ڿ�����
    while(k<knockout_num){
        rand_row=generateRandNum()-1;
        rand_col=generateRandNum()-1;
        if(checksodukolist[rand_row][rand_col]==0){
            checksodukolist[rand_row][rand_col]=1;
            k_soduko[rand_row][rand_col]=0;
            knockout_flag[rand_row][rand_col]=1;//�ڹ��ĸ��Ӽ�Ϊ1
            k++;
        }
    }
    return true;
}

int playsoduko(){//������
    int row=0,col=0,num=0,tep,p;
    while(issodukoFull()==0){
        printsoduko();//��ӡ����
        printf("�������кţ��кţ�Ҫ�����.\n");
        scanf("%d %d %d",&row,&col,&num);
        if(row>0&&row<10&&col>0&&col<10&&num>0&&num<10&&knockout_flag[row-1][col-1]==1){
            tep=soduko[row-1][col-1];
            soduko[row-1][col-1]=num;
            if(!check()){
                printf("�������ֲ�����!\n");
                soduko[row-1][col-1]=tep;
            }
            else
                printf("�޸ĳɹ�!\n");
        }
        else{
            printf("����Ƿ�!\n");
        }
        printf("��Ҫ������ô������1�����棬����0�˳�.\n");
        scanf("%d",&p);
        if(!p){
            printf("��Ϸ����!\n");
            goto mdzz;
        }
        system("cls");
    }
    printf("��Ϸ����!\n");
    mdzz:

    return true;
}

int generatefinalsoduko(int hard_level){//�����ڿ�����
    generateFirstLine();
    generatesoduko(9);
    knockout_soduko(soduko,hard_level);//�ڿ�
    return true;
}

int playwithcnf(){//����������
    int i,j,k=1;
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(knockout_flag[i][j]==1)
                soduko[i][j]=sodukoanswer[k];
            k++;
        }
    }
    return true;
}

int sodukoToCnf(char *sodukotocnffilename){
    FILE *fp;
    fp=fopen(sodukotocnffilename,"w");
    if(fp==NULL)
       return FALSE;
    //generatefinalsoduko();
    //checksodukolist
    //д��ͷ��Ϣ
    int i,j,notzeronum=0;
    for(i=0;i<9;i++){ //ÿ��ΪֵvalueA��ֵvalueB����36*81=2916��
        for(j=0;j<9;j++){
            if(soduko[i][j]>0)
                notzeronum++;
        }
    }
    int total_clauses=11988+notzeronum;
    fprintf(fp,"%c %s %d %d\n",'p',"cnf",729, total_clauses);
    int row,col,putnum;

    int debug=0;
    //д��������

    for(row=1;row<10;row++){//ÿ�����Ϊ1-9����9*9=81��
        for(col=1;col<10;col++){
            for(putnum=1;putnum<10;putnum++){
                //fprintf(fp,"%d ",row*100+col*10+putnum);
                fprintf(fp,"%d ",((row-1)*9+col-1)*9+putnum);

            }
            fprintf(fp,"0\n");
            debug++;
        }
    }

    for(row=1;row<10;row++){
        for(col=1;col<10;col++){
            for(i=1;i<10;i++){
                for(j=i+1;j<10;j++){
                    //fprintf(fp,"%d %d 0\n",-(row*100+col*10+i),-(row*100+col*10+j));
                    fprintf(fp,"%d %d 0\n",-(((row-1)*9+col-1)*9+i),-(((row-1)*9+col-1)*9+j));
                    debug++;
                }
            }
        }
    }
    //дÿһ��
    for(row=1;row<10;row++){
        for(i=1;i<10;i++){
            for(col=1;col<10;col++){
                //fprintf(fp,"%d ",(row*100+col*10+i));
                fprintf(fp,"%d ",((row-1)*9+col-1)*9+i);

            }
            fprintf(fp,"0\n");
            debug++;
        }
    }

    for(row=1;row<10;row++){
        for(i=1;i<10;i++){
            for(col=1;col<10;col++){
                for(j=col+1;j<10;j++){
                    //fprintf(fp,"%d %d 0\n",-(row*100+col*10+i),-(row*100+j*10+i));
                    fprintf(fp,"%d %d 0\n",-(((row-1)*9+col-1)*9+i),-(((row-1)*9+j-1)*9+i));
                }
            }
        }
    }

    //дÿһ��
    for(col=1;col<10;col++){
        for(i=1;i<10;i++){
            for(row=1;row<10;row++){
                //fprintf(fp,"%d ",(row*100+col*10+i));
                fprintf(fp,"%d ",((row-1)*9+col-1)*9+i);

            }
            fprintf(fp,"0\n");
        }
    }

    for(col=1;col<10;col++){
        for(i=1;i<10;i++){
            for(row=1;row<10;row++){
                for(j=row+1;j<10;j++){
                    //fprintf(fp,"%d %d 0\n",-(row*100+col*10+i),-(j*100+col*10+i));
                    fprintf(fp,"%d %d 0\n",-(((row-1)*9+col-1)*9+i),-(((j-1)*9+col-1)*9+i));
                }
            }
        }
    }

    int k,l;

    //дÿһ����
    for(i=1;i<10;i++){
        for(row=1;row<10;row+=3){
            for(col=1;col<10;col+=3){
                for(k=row;k<row+3;k++){
                    for(j=col;j<col+3;j++){
                        //fprintf(fp,"%d ",(k*100+j*10+i));
                        fprintf(fp,"%d ",((k-1)*9+j-1)*9+i);

                    }
                }
                fprintf(fp,"%d\n",0);
            }
        }
    }

int m;
    for(i=1;i<10;i++){
        for(row=1;row<10;row+=3){
            for(col=1;col<10;col+=3){
                for(l=col;l<col+3;l++){
                    for(k=row;k<row+3;k++){
                        for(j=col;j<col+3;j++){
                            for(m=k;m<row+3;m++){
                            //fprintf(fp,"%d %d 0\n",-(k*100+l*10+i),-(k*100+j*10+i));
                                if(m==k&&j<=l){
                                }
                                else{
                                    fprintf(fp,"%d %d 0\n",-(((k-1)*9+l-1)*9+i),-(((m-1)*9+j-1)*9+i));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //д�Ѿ���Ŀ�
    for(i=0;i<9;i++){
        for(j=0;j<9;j++){
            if(soduko[i][j]>0){
                //fprintf(fp,"%d 0\n",(i+1)*100+(j+1)*10+soduko[i][j]);
                fprintf(fp,"%d 0\n",((i)*9+j)*9+soduko[i][j]);
            }
        }
    }

    fclose(fp);
    return true;
}
/*
for(i=1;i<10;i++){
        for(row=1;row<10;row+=3){
            for(col=1;col<10;col+=3){
                for(l=col;l<col+3;l++){
                    for(k=row;k<row+3;k++){
                        for(j=l+1;j<col+3;j++){
                            //fprintf(fp,"%d %d 0\n",-(row*100+l*10+i),-(k*100+j*10+i));
                            fprintf(fp,"%d %d 0\n",-(((row-1)*9+l-1)*9+i),-(((k-1)*9+j-1)*9+i));
                            debug++;
                        }
                    }
                }
            }
        }
    }
*/
