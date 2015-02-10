
#include <stdio.h>
#include <assert.h>

#define boolean char
#define true 1
#define false 0

int Num = 1;
int newNum = 1;
boolean flag = true;
boolean overflow = false;
int nf=0;
int nm=0;

void counter()
{

    if (nf>=nm&&overflow==false) {
        printf("OKn(P), now the n is %d, OK!\n", Num);
        newNum++;
        Num = newNum;
        nf=0;
        nm=0;
        
    }
    else
    {
        printf("Overflow at %d \n",Num);
        assert(overflow==false);
    }


}

void M()
{
    Num = Num - 1;
    nm++;
    
    if (Num < 0)
    {
        overflow = true;
    }
    
    /*
    flag = true;
    if (Num<0) {
        printf("the current state is overflow\n");
        newNum = newNum + 1;
        Num = newNum;
        flag = false;
        if (newNum == 2) {
            printf("the number of available is a2\n");
        }
        else
            printf("the number of available memory cells is %d\n", newNum);
    }
    else
        printf("the current state is OK\n");*/
    
}

void F()
{
    //if (flag==true) {
        Num = Num + 1;
    //}
    nf++;
    
}

void Rec1()
{
l1:
    M();
    counter();
    goto l1;
}

void Rec2()
{
l1:
    M();
    F();
    counter();
    goto l1;
}

void Rec3()
{
l1:
    M();
    counter();
    goto l1;
    F();
}

int main()
{
     Rec2();
    /*int id ;
    scanf("%d",&id);
    switch (id) {
        case 1: Rec1();
        case 2: Rec2();
        case 3: Rec3();
    }*/
}

