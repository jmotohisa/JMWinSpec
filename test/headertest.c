#include <stdio.h>
#include "../WinSpecHeader25.h"

int main()
{
  WINXHEADER_STRUCT test;
  printf("Align of %lu (should be 1)\n",__alignof__(test));
  printf("Header Size (should be 4100):%ld\n",sizeof(WINXHEADER_STRUCT));
  printf(" short:%ld\n",sizeof(short));
  printf(" int :%ld\n",sizeof(int));
  printf(" long (should be 4): %ld\n",sizeof(long));
  printf(" float:%ld\n",sizeof(float));
  printf(" double:%ld\n",sizeof(double));
  printf(" BYTE:%ld\n",sizeof(BYTE));
  printf(" WORD:%ld\n",sizeof(WORD));
  printf(" DWORD (should be 4):%ld\n",sizeof(DWORD));
}
