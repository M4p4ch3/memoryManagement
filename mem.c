
# include "mem.h"

bool bInit = false;
int freeSize = TOTAL_SIZE;
char data[TOTAL_SIZE];
Bloc * firstBloc = NULL;

void init()
{
    firstBloc = (Bloc *) malloc(sizeof(Bloc));
    firstBloc->alloc = false;
    firstBloc->size = TOTAL_SIZE;
    firstBloc->address = 0;
    firstBloc->prev = NULL;
    firstBloc->next = NULL;
    bInit = true;

    for (int i = 0; i < TOTAL_SIZE; i = i + 1)
    {
        data[i] = '-';
    }
}

void myMemCpy(int srcAdd, int destAdd, int size)
{
    for (int i = 0; i < size; i = i + 1)
    {
        data[destAdd + i] = data[srcAdd + i];
    }
}

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

int myMalloc(int size)
{
    if (!bInit) init();

    // Not enough free Size
    if (freeSize < size) return -1;

    // Get first suited free Block
    Bloc * freeBloc = firstBloc;
    while (freeBloc != NULL && !(freeBloc->alloc == false && freeBloc->size > size))
    {
        freeBloc = freeBloc->next;
    }
    // No free Block found
    if (freeBloc == NULL)
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

    // Create new alloc Block
    Bloc * newAllocBloc = (Bloc *) malloc(sizeof(Bloc));
    newAllocBloc->alloc = true;
    newAllocBloc->address = freeBloc->address;
    newAllocBloc->size = size;
    newAllocBloc->prev = freeBloc->prev;
    freeSize = freeSize - size;

    if (freeBloc->prev != NULL)
    {
        freeBloc->prev->next = newAllocBloc;
    }
    else // (freeBloc->prev == NULL)
    {
        firstBloc = newAllocBloc;
    }

    // Still some place in Free Block
    if (freeBloc->size > size)
    {
        newAllocBloc->next = freeBloc;
        freeBloc->size = freeBloc->size - size;
        freeBloc->address = freeBloc->address + size;
        freeBloc->prev = newAllocBloc;
    }
    // Free Block totally used
    else // (freeBloc->size == size)
    {
        newAllocBloc->next = freeBloc->next;
        if (freeBloc->next != NULL)
        {
            freeBloc->next->prev = newAllocBloc;
        }
        free(freeBloc);
    }

    return newAllocBloc->address;
}

void myFree(int address)
{
    // Find alloc Block
    Bloc * allocBloc = firstBloc;
    while (allocBloc != NULL && allocBloc->address != address)
    {
        allocBloc = allocBloc->next;
    }
    // Alloc Block not found
    if (allocBloc == NULL) return;
    // Requested Address is not allocated
    if (allocBloc->alloc == false) return;

    // Free the Block
    allocBloc->alloc = false;
    freeSize = freeSize + allocBloc->size;
    
    Bloc * freeBloc;
    // If next Bloc is free
    if (allocBloc->next != NULL && allocBloc->next->alloc == false)
    {
        freeBloc = allocBloc->next;
        allocBloc->size = allocBloc->size + freeBloc->size;
        allocBloc->next = freeBloc->next;
        freeBloc->next->prev = allocBloc;
        free(freeBloc);
    }
    // If prev Bloc is free
    if (allocBloc->prev != NULL && allocBloc->prev->alloc == false)
    {
        freeBloc = allocBloc->prev;
        allocBloc->address = freeBloc->address;
        allocBloc->size = allocBloc->size + freeBloc->size;
        allocBloc->prev = freeBloc->prev;
        freeBloc->prev->next = allocBloc;
        free(freeBloc);
    }
}

void write(int address, int len, char * str)
{
    // Find alloc Block
    Bloc * allocBloc = firstBloc;
    while (allocBloc != NULL && allocBloc->address != address)
    {
        allocBloc = allocBloc->next;
    }
    // Alloc Block not found
    if (allocBloc == NULL) return;
    // Requested Address is not allocated
    if (allocBloc->alloc == false) return;
    // To long Message
    if (len > allocBloc->size) return;

    for (int i = 0; i < len; i = i + 1)
    {
        data[address + i] = str[i];
    }
}

int read(int address, int offset)
{
    return data[address + offset];
}

void dispData()
{
    Bloc * bloc = firstBloc;
    printf(" ");
    while (bloc != NULL)
    {
        if (bloc->address < 10)
        {
            printf("0");
        }
        printf("%d", bloc->address);
        for (int i = 0; i < bloc->size - 2; i = i + 1)
        {
            printf(" ");
        }
        bloc = bloc->next;
    }
    printf("\r\n");
    printf("[");
    bloc = firstBloc;
    while (bloc != NULL)
    {
        for (int i = 0; i < bloc->size; i = i + 1)
        {
            printf("%c", data[bloc->address + i]);
        }
        bloc = bloc->next;
    }
    printf("]\r\n");
    printf(" ");
    bloc = firstBloc;
    while (bloc != NULL)
    {
        for (int i = 0; i < bloc->size; i = i + 1)
        {
            if (bloc->alloc == false) printf("F");
            else printf(" ");
        }
        bloc = bloc->next;
    }
    printf("\r\n");
}

void dispBlocs()
{
    Bloc * bloc = firstBloc;
    while (bloc != NULL)
    {
        if (bloc->alloc == false)
        {
            printf("Free Block :\r\n");
        }
        else // (bloc->alloc == 1)
        {
            printf("Alloc Block :\r\n");
        }
        printf(" - Address : %d\r\n", bloc->address);
        printf(" - Size : %d\r\n", bloc->size);
        bloc = bloc->next;
    }
}
