#include <stdio.h>
#include <assert.h>

int Num = 1 ;

void M()
{
  Num = Num - 1;
  assert(Num >= 0);
}

void F()
{
  Num = Num + 1 ;
}

void database()
{
  M();
  M();
  F();
  F();
}

void person()
{
  M();
  M();
  F();
  F();
}
int main()
{
  person();
  // database();
  printf("End of program\n");
}
