# include "libtest.h"
# include <stdio.h>
# include <dlfcn.h>
# include <stdlib.h>

int main()
{
    void * handle = dlopen("./libtest.so",RTLD_LAZY);

    if (handle == NULL) {
        printf("dlopen error\n");
        exit(0);
    }

    int (*f)(int a,int b);

    f = dlsym(handle,"add");

    int res = f(1,2);

    printf("%d\n",res);

    return 0;
}