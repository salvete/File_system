#include"headers.h"
#include<stdio.h>
#define MAX_FUN 8

//#define LARGE_INFO

static struct orders res;
#ifdef LARGE_INFO
char large_info[10000];
#endif // LARGE_INFO

void shell()
{
    #ifdef LARGE_INFO
    int i;
    for ( i=0; i<9999; i++)
        large_info[i]='x';
    large_info[9999]='\0';
    #endif // LARGE_INFO

    char input[100];
    Read_TXT();
    while (1)
    {
        printf("%s@PC:",cur_name);
        pwd(cur_inode);
        printf("$ ");
        memset(input,0,sizeof(input));

        gets(input);

        Return_Orders(input,&res);

        if (res.Order_Num == 2)
            ls(cur_inode);
        else if (res.Order_Num == 3)
        {
            if (res.Len == 2 && namecmp("-f",res.Para[0]) == 0)
                mkdir(res.Para[1],T_FILE);
            else
                mkdir(res.Para[0],T_DIR);
        }
        else if (res.Order_Num == 4)
            cd(res.Para[0]);
        else if (res.Order_Num == 5 && res.Len == 2)
            #ifdef LARGE_INFO
            echo(large_info,res.Para[1]);
            #else
            echo(res.Para[0],res.Para[1]);
            #endif
        else if (res.Order_Num == 6 && res.Len == 1)
            cat(res.Para[0]);
        else
            continue;

    }
}
