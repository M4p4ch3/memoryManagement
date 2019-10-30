
# include <stdio.h>
# include <string.h>

# include "mem.h"

void write(void * ptr, char * str, int len)
{
    // Null Pointer
    if (ptr == NULL) return;
    // No data
    if (str == NULL || len <= 0) return;

    for (int i = 0; i < len; i = i + 1)
    {
        *((char *)(ptr + i * sizeof(char))) = str[i];
    }
}

int main()
{
    char * p[30];

    p[0] = myMalloc(10);
    p[1] = myMalloc(10);
    p[2] = myMalloc(10);
    p[3] = myMalloc(6);
    p[4] = myMalloc(10);
    p[5] = myMalloc(2);
    p[6] = myMalloc(10);
    prettyDisp();
    myFree(p[1]);
    myFree(p[3]);
    myFree(p[5]);
    prettyDisp();

    p[10] = myMalloc(6);
    prettyDisp();

    myFree(getPtr(2));
    prettyDisp();

    return 0;
}