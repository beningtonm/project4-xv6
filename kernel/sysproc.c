#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

uint64
sys_vmprint(void)
{
  pagetable_t pagetable = 0;

  argaddr(0, pagetable);
  vmprint(pagetable); // Call the vmprint function from proc.c.

  return 0; // Return success.
}


// // Previous Version
// uint64
// sys_pgaccess(void)
// {
//   uint64 start_virt_addr; // Starting virtual address of the first user page to check.
//   int num_pages;          // Number of pages to check
//   uint64 result_addr;     // User address to a buffer to store the results into a bitmask
//   //int result = 0;

//   // Read arguments passed from user space
//   argaddr(0, &start_virt_addr);
//   argint(1, &num_pages);
//   argaddr(2, &result_addr);

//   pagetable_t pt = myproc()->pagetable;
//   vmprint(pt);
//   // Call pgaccess() to perform the actual work
//   if (pgaccess(&pt, start_virt_addr, num_pages, result_addr) < 0)
//   {
//     return -1; // Error in pgaccess function
//   }

//   // Copy the result back to user space
//   if (copyout(pt, result_addr, (char *) &result_addr, sizeof(result_addr)) < 0)
//   {
//     return -1; // Error in copying result to user space
//   }

//   return 0; // Success
// }

// Gary Fix
uint64
sys_pgaccess(void)
{
  uint64 start_virt_addr; // Starting virtual address of the first user page to check.
  int num_pages;          // Number of pages to check
  uint64 result_addr;     // User address to a buffer to store the results into a bitmask
  int bitmap = 0; 

  // Read arguments passed from user space
  argaddr(0, &start_virt_addr);
  argint(1, &num_pages);
  argaddr(2, &result_addr);

  pagetable_t pt = myproc()->pagetable;
  vmprint(pt);
  // Call pgaccess() to perform the actual work
  if (pgaccess(&pt, start_virt_addr, num_pages, &bitmap) < 0)
  {
    return -1; // Error in pgaccess function
  }

  // Copy the result back to user space
  if (copyout(pt, result_addr, (char *) &bitmap, sizeof(bitmap)) < 0)
  {
    return -1; // Error in copying result to user space
  }

  return 0; // Success
}





// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
