
# include <stdio.h>

# include "mem.h"

int main()
{
    char d1[10] = "<---p1--->";
    char d2[10] = "<---p2--->";
    char d3[10] = "<---p3--->";

    init();
    disp();
    
    char * p1 = myMalloc(10);
    char * p2 = myMalloc(10);
    char * p3 = myMalloc(10);
    disp();
    
    write(p1, "<===p1===>", 10);
    write(p2, "<===p2===>", 10);
    write(p3, "<===p3===>", 10);
    disp();

    myFree(p2);
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