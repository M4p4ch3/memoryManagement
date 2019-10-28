
# include <stdio.h>

# include "mem.h"

int main()
{
    char * p1;
    char * p2;
    char * p3;
    
    init();
    p1 = myMalloc(10);
    p2 = myMalloc(10);
    p3 = myMalloc(10);
    write(p1, "<===p1===>", 10);
    write(p2, "<===p2===>", 10);
    write(p3, "<===p3===>", 10);
    myFree(p2);
    myFree(p3);
    disp();

    myFree(p1);
    p1 = myMalloc(250);
    p2 = myMalloc(250);
    p3 = myMalloc(250);
    write(p1, "<===p1======", 20);
    write(p2, "<===p2======", 20);
    write(p3, "<===p3======", 20);
    myFree(p2);
    disp();

    p2 = myMalloc(300);
    if (p2 != NULL)
    {
        write(p2, "<===p2======", 20);
    }
    disp();

    /*
    char * p4 = myMalloc(5);
    disp();

    write(p4, 5, "<p4->");
    disp();

    // defrag();
    disp();
    /*
    
    int p5 = myMalloc(2);
    disp();

    myFree(p4);
    disp();
    
    myFree(p5);
    disp();

    int p6 = myMalloc(5);
    disp();

    myFree(p6);
    disp();

    defrag();
    disp();

    //*/
    return 0;
}