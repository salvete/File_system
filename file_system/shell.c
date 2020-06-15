#include"headers.h"
#include<stdio.h>
#define MAX_FUN 8

static struct orders res;

void shell()
{
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
        else
            continue;

    }
}
