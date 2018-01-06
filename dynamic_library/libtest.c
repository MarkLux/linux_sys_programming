# include "libtest.h"

/**
 * compile command: gcc libtest.c -fPIC -shared -o libtest.so
 */

int add(int a,int b)
{
    return a+b;
}