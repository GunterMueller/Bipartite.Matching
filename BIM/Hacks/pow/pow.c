#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  double a,b,c;
  a = strtod(argv[1],NULL);
  b = strtod(argv[2],NULL);
  c = pow(a,b);
  printf("%ld",(long) c);
  return 0;
}
