
# include "mem.h"

bool bInit = false;
int freeSize = TOTAL_SIZE;
char memory[TOTAL_SIZE];
Bloc * firstBloc = NULL;

void init()
{
    for (int i = 0; i < TOTAL_SIZE; i = i + 1)
    {
        memory[i] = '-';
    }
    
    firstBloc = (Bloc *) &memory[0];
    firstBloc->alloc = false;
    firstBloc->size = TOTAL_SIZE - sizeof(Bloc);
    firstBloc->prev = NULL;
    firstBloc->next = NULL;
    bInit = true;
}

void myMemCpy(void * src, void * dest, int len)
{
    for (int i = 0; i < len; i = i + 1)
    {
        *((char *)(dest + i)) = *((char *)(src + i));
    }
}

/*
void defrag()
{
    Bloc * prevFreeBloc = firstBloc;
    Bloc * allocBloc;
    
    while (prevFreeBloc != NULL && prevFreeBloc->next != NULL &&
        !(prevFreeBloc->alloc == false && prevFreeBloc->next->alloc == true))
    {
        prevFreeBloc = prevFreeBloc->next;
    }
    allocBloc = prevFreeBloc->next;

    // Copy Data to prev Free Block
    myMemCpy(allocBloc->address, prevFreeBloc->address, allocBloc->size);

    Bloc * newFreeBloc = (Bloc*) malloc(sizeof(Bloc));
    newFreeBloc->alloc = false;
    newFreeBloc->address = prevFreeBloc->address + allocBloc->size;
    newFreeBloc->size = prevFreeBloc->size;
    newFreeBloc->prev = allocBloc;
    newFreeBloc->next = allocBloc->next;
    
    allocBloc->address = prevFreeBloc->address;
    allocBloc->prev = prevFreeBloc->prev;
    prevFreeBloc->prev->next = allocBloc;
    allocBloc->next->prev = newFreeBloc;
    allocBloc->next = newFreeBloc;
}
//*/

void * myMalloc(int size)
{
    if (!bInit) init();

    // Not enough free Size
    // if (freeSize < size) return NULL;

    // Get first suited free Block
    Bloc * bloc = firstBloc;
    while (bloc != NULL && !(bloc->alloc == false && bloc->size > size))
    {
        bloc = bloc->next;
    }
    // No free Block found
    if (bloc == NULL) return NULL;
    /*
    {
        // Enough free Size
        if (freeSize >= size)
        {
            // Defragment Memory
            defrag();

            // 2nd Try
            freeBloc = firstBloc;
            while (freeBloc != NULL && !(freeBloc->alloc == false && freeBloc->size > size))
            {
                freeBloc = freeBloc->next;
            }
            // No free Block found
            if (freeBloc == NULL) return -1;
        }
        // Not enough Size
        else return -1;
    }
    //*/

    // Still some place in Free Block
    if (bloc->size > size)
    {
        // Create new free Block
        Bloc * newFreeBloc = bloc + sizeof(Bloc) + size;
        newFreeBloc->alloc = false;
        newFreeBloc->size = bloc->size - (sizeof(Bloc) + size);
        newFreeBloc->prev = bloc;
        newFreeBloc->next = bloc->next;

        bloc->next = newFreeBloc;
        bloc->size = size;
    }

    // Alloc Block
    bloc->alloc = true;
    freeSize = freeSize - size;

    return bloc + sizeof(Bloc);
}

void myFree(void * ptr)
{
    // Find alloc Block
    Bloc * bloc = firstBloc;
    while (bloc != NULL && bloc + sizeof(Bloc) != ptr)
    {
        bloc = bloc->next;
    }
    // Alloc Block not found
    if (bloc == NULL) return;
    // Requested Pointer is not allocated
    if (bloc->alloc == false) return;

    // Free the Block
    bloc->alloc = false;
    // freeSize = freeSize + allocBloc->size;
    
    /*
    Bloc * freeBloc;
    // If next Bloc is free
    if (allocBloc->next != NULL && allocBloc->next->alloc == false)
    {
        freeBloc = allocBloc->next;
        allocBloc->size = allocBloc->size + freeBloc->size;
        allocBloc->next = freeBloc->next;
        freeBloc->next->prev = allocBloc;
    }
    // If prev Bloc is free
    if (allocBloc->prev != NULL && allocBloc->prev->alloc == false)
    {
        freeBloc = allocBloc->prev;
        allocBloc = freeBloc;
        allocBloc->size = allocBloc->size + freeBloc->size;
        allocBloc->prev = freeBloc->prev;
        freeBloc->prev->next = allocBloc;
    }
    //*/
}

void write(void * ptr, char * str, int len)
{
    // Find alloc Block
    Bloc * bloc = firstBloc;
    while (bloc != NULL && bloc + sizeof(Bloc) != ptr)
    {
        bloc = bloc->next;
    }
    // Alloc Block not found
    if (bloc == NULL) return;
    // Requested Pointer is not allocated
    if (bloc->alloc == false) return;
    // To long Message
    if (len > bloc->size) return;

    for (int i = 0; i < len; i = i + 1)
    {
        *((char *)(ptr + i * sizeof(char))) = str[i];
    }
}

void disp()
{
    int iBloc = 0;
    void * ptr;
    Bloc * bloc = firstBloc;
    while (bloc != NULL)
    {
        printf("Bloc : %d\r\n", iBloc);
        printf(" - address : %p\r\n", bloc);
        printf(" - alloc : %d\r\n", bloc->alloc);
        printf(" - size : %d\r\n", bloc->size);
        printf(" - prev : %p\r\n", bloc->prev);
        printf(" - next : %p\r\n", bloc->next);
        ptr = bloc + sizeof(Bloc);
        if (bloc->alloc == true)
        {
            printf(" - data : [");
            for (int i = 0; i < bloc->size; i = i + 1)
            {
                printf("%c", *((char *)(ptr + i * sizeof(char))));
            }
            printf("]\r\n");
        }
        bloc = bloc->next;
        iBloc = iBloc + 1;
    }
}
