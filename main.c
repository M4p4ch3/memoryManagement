
# include <stdio.h>

# include "mem.h"

int main()
{
    init();
    dispData();
    
    int p1 = myMalloc(10);
    dispData();

    int p2 = myMalloc(10);
    int p3 = myMalloc(10);
    dispData();

    write(p1, 10, "<---p1--->");
    write(p2, 10, "<---p2--->");
    write(p3, 10, "<---p3--->");
    dispData();

    myFree(p2);
    dispData();

    int p4 = myMalloc(5);
    dispData();

    write(p4, 5, "<p4->");
    dispData();

    defrag();
    dispData();
    /*
    
    int p5 = myMalloc(2);
    dispData();

    myFree(p4);
    dispData();
    
    myFree(p5);
    dispData();

    int p6 = myMalloc(5);
    dispData();

    myFree(p6);
    dispData();

    defrag();
    dispData();

    //*/
    return 0;
}