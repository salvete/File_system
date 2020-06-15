#ifndef ORDERS_H_INCLUDED
#define ORDERS_H_INCLUDED

typedef struct orders
{
    int Order_Num;
    char Para[10][100];//存放参数
    int Len;//参数的个数
}Orders;
#endif // ORDERS_H_INCLUDED
