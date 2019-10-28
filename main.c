
# include <stdio.h>

# include "mem.h"

int main()
{
    char * p[90];
    
    init();

    p[0] = myMalloc(10);
    write(p[0], "<===pi===>", 10);
    p[1] = myMalloc(10);
    write(p[1], "<===pi===>", 10);
    disp();

    /*
    for (int i = 0; i < 30; i = i + 1)
    {
        p[i] = myMalloc(10);
        write(p[i], "<===pi===>", 10);
    }
    disp();
    //*/

    /*
    for (int i = 0; i < 30; i = i + 2)
    {
        myFree(p[i]);
    }
    //*/

    /*
    disp();
    p[89] = myMalloc(40);
    disp();
    //*/

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