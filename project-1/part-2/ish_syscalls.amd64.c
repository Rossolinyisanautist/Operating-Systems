#define asm __asm__
#define vltl __volatile__
#define naked __attribute__((naked))


// volatile is implicitly defined for basic asm
asm (
    ".global ish_read\n\t"
        "ish_read:\n\t"
        "push %rbp\n\t"
        "mov %rsp, %rbp\n\t"
        "xor %rax, %rax\n\t"
        "syscall\n\t"
        "mov %rbp, %rsp\n\t"
        "pop %rbp\n\t"
        "ret"
);
extern long ish_read(int, void*, unsigned long);

/*
long ish_read(int file_descriptor, void* buffer, unsigned long buffer_size)
{
    long res = -1;
    __asm__ __volatile__ (
        "xor %%rax, %%rax\n\t"
        "syscall\n\t"
        "mov %%rax, %0\n\t"
        : "=r"(res)
        :
        : "%rax"
    );
    return res;
}
*/


naked int ish_chdir(const char* path)
{
    asm vltl (
        "push %rbp\n\t"
        "mov %rsp, %rbp\n\t"
        "mov $80, %rax\n\t"
        "syscall\n\t"
        "mov %rbp, %rsp\n\t"
        "pop %rbp\n\t"
        "ret\n\t"
    );
    /*
    int res = 0;
    __asm__ __volatile__ (
    "mov $80, %%rax\n\t"
    "syscall\n\t"
    : "=r" (res)
    :
    : "%rax"
    );
    return res;
    */
}

void ish_exit(int status)
{
    asm vltl (
        "mov $60, %%rax\n\t"
        "syscall\n\t"
        :
        :
        : "%rax"
    );
}


int ish_stat(const char* path, void* stat_result)
{   
    int res = -1;
    asm vltl (
        "mov $4, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=rm" (res)
        : 
        : "%rax"
    );    
    return res;
}

int ish_open(const char *path, int flags)
{
    int res = -1;
    asm vltl (
        "mov $2, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r"(res)
        :
        : "%rax"
    );
    return res;
}

int ish_creat(const char *path, unsigned int mode)
{
    int res = -1;
    asm vltl (
        "mov $85, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=rm"(res)
        :
        : "%rax"
    );
    return res;
}

int ish_dup2(int old_file_descriptor, int new_file_descriptor)
{
    int res = -1;
    asm vltl (
        "mov $33, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r"(res)        
        :
        : "%rax"
    );

    return res;
}

int ish_close(int file_descriptor)
{
    int res = -1;
    asm vltl (
        "mov $3, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r"(res)
        :
        : "%rax"
    );

    return res;
}

int ish_fork()
{
    int res = -1;
    asm vltl(
        "mov $57, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r"(res)
        :
        : "%rax"
    );

    return res;
}

int ish_execve(const char *path, char *const arguments[], char *const environment[])
{
    int res = -1;
    asm vltl (
        "mov $59, %%rax\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r"(res)
        :
        : "%rax"
    );

    return res;
}

int ish_waitpid(int pid, int *status, int options)
{
    int res = -1;
    asm vltl(
        "mov $61, %%rax\n\t"
        "xor %%r10, %%r10\n\t"
        "syscall\n\t"
        "mov %%eax, %0\n\t"
        : "=r"(res)
        :
        : "%rax", "%r10"
    );

    return res;
}

long ish_write(int file_descriptor, const void *buffer, unsigned long buffer_size)
{
    long res = -1;
    asm vltl (
        "mov $1, %%rax\n\t"
        "syscall\n\t"
        "mov %%rax, %0\n\t"
        : "=r"(res)
        :
        : "%rax"        
    );

    return res;
}

#ifdef naked
#undef naked
#endif

#ifdef asm
#undef asm
#endif

#ifdef vltl
#undef vltl
#endif
