#include"headers.h"
#include<stdlib.h>

char TXT[100][100];
int Lengthofhead=0;
char **cut_string(char *input)//�и������ַ���
{
    char **head;
	int sum = 0, word = 0;
	int num=0;
    int n=strlen(input);
    int i,k;
    for( i=0;i<n;i++)
    {
		if (input[i] == ' ')
			word = 0;
		else if (word == 0)
		{
			word = 1;
			sum++;
		}
    }
    head=(int**)malloc(sizeof(int*)*sum);
    Lengthofhead=sum;
    for( i=0;i<sum;i++)
    {
        head[i]=(int*)malloc(sizeof(int)*30);
        int j;
        for( j=0;j<30;j++)
            head[i][j]='\0';
    }

    for( i=0,k=0;i<n;i++,k++)
    {
        int j;
        for( j=0;input[i]!=' '&&i<n;j++)
        {
            head[k][j]=input[i];
            i++;

        }
    }
     return head;
}
void Read_TXT()
{
    FILE* fp;
    int Raw=0;
    fp=fopen("�������.txt","r");
    while (!feof(fp))
    {
        fscanf(fp, "%s",TXT[Raw++]);
    }
    fclose(fp);
}
void Return_Orders(char *input,Orders *order)//�����и�õ��ַ�������һ���ṹ��
{
    int i,j;
    for(i=0;i<10;i++)
        for(j=0;j<100;j++)
        order->Para[i][j]='\0';
    memset(order->Para,0,100*sizeof(char));
    char Para[100]={'\0'};
    char Ord[100]={'\0'};
    char **head=cut_string(input);//���������cut_string�и��ַ���
    for( i=0,j=0;i<Lengthofhead;i++)
    {
        if(i==0)
        memcpy(Ord,head[i],sizeof(char)*(strlen(head[i])));
        else
        {
            memcpy(order->Para[j],head[i],sizeof(char)*(strlen(head[i])));
            j++;
        }
    }
    for( i=0;i<100;i++)
    {
        if(strcmp(TXT[i],Ord)==0)
        {
            order->Order_Num=atoi(TXT[i-1]);
            break;
        }
    }
    order->Len=Lengthofhead-1;
}
//int main()
//{
//    Read_TXT();//��TXT�ļ������ȡ��Ϣ�����浽char TXT[100][100]�У����ִ��һ�ξ���
//    char b[100]="ls /a/a/a ca/wd/d a 4�� 6awd5";
//    Orders a;
//    Return_Orders(b,&a);
//    printf("����ţ�%d\n��������%d\n",a.Order_Num,a.Len);
//    for(int i=0;i<a.Len;i++)
//    {
//        printf("��%d������Ϊ�� %s\n",i,a.Para[i]);
//    }
//}
