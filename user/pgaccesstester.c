#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/riscv.h" // defines PGSIZE
#include "user/user.h"
#include "kernel/param.h"
#include "kernel/fcntl.h"


int main(int argc, char **argv)
{
    printf("RUNNING PGACCESS() TEST");
    char *pageTester;
    int accessBitmap = 0;
    pageTester = malloc(sizeof(int)*8*PGSIZE);
    for (int i = 0; i < sizeof(int)*8; ++i)
    {
        pageTester[i*PGSIZE]=1; // accessed
    }
    if (pgaccess(pageTester,sizeof(int)*8,&accessBitmap) < 0)
    {
        printf("error in pgaccess\n");
    }
    else
    {
        for (int i =0; i < sizeof(int)*8; ++i)
        {
            printf("Page %d, accessed: %d\n",
                i, (accessBitmap & (1<<i))!=0);
        }
    }
    exit(0);
}