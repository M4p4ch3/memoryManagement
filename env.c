
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
    int size = 0;
    int id = 0;
    int reqID = 0;
    char str[10];
    void * ptr[100];

    id = 0;
    while (strcmp(str, "-1") != 0)
    {
        scanf("%s", str);

        if (strcmp(str, "malloc") == 0)
        {
            scanf("%d", &size);
            ptr[id] = myMalloc(size);
            id = id + 1;
        }
        else if (strcmp(str, "free") == 0)
        {
            scanf("%d", &reqID);
            myFree(getPtr(reqID));
        }
        else if (strcmp(str, "disp") == 0)
        {
            disp();
        }
        else if (strcmp(str, "prettyDisp") == 0)
        {
            prettyDisp();
        }
    }

    return 0;
}