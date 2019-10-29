
# include <stdio.h>

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
    char str[10] = "<===pi===>";
    char * p[30];

    p[0] = myMalloc(10);
    str[5] = '0';
    write(p[0], str, 10);
    p[1] = myMalloc(10);
    str[5] = '1';
    write(p[1], str, 10);
    p[2] = myMalloc(10);
    str[5] = '2';
    write(p[2], str, 10);
    p[3] = myMalloc(10);
    str[5] = '3';
    write(p[3], str, 10);
    p[4] = myMalloc(10);
    str[5] = '4';
    write(p[4], str, 10);
    myFree(p[1]);
    myFree(p[3]);
    disp();

    defrag();
    disp();


    
    /*
    for (int i = 0; i < 30; i = i + 1)
    {
        p[i] = myMalloc(10);
        printf("p[%d] : %p\r\n", i, p[i]);
        str[5] = (char) (i + 65);
        write(p[i], str, 10);
    }
    //*/

    /*
    for (int i = 0; i < 30; i = i + 2)
    {
        myFree(p[i]);
    }
    disp();
    //*/

    //*/
    return 0;
}