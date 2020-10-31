#define asm __asm__
#define vltl __volatile__
#define naked __attribute__((naked))

/*
 * disable this code as it may produce errors
 * if compiler decides to discard ish_read
// read works
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
*/

// works too
long ish_read(int file_descriptor, void* buffer, unsigned long buffer_size)
{
    register long res asm("%rax");
    __asm__ __volatile__ (
        "xor %%rax, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        : "D"(file_descriptor), "S"(buffer), "d"(buffer_size)
        :
    );
    return res;
}


// experiment with naked func
// chdir works
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
    // works too
    /*
    register int res asm("%eax");
    __asm__ __volatile__ (
    "mov $80, %%rax\n\t"
    "syscall\n\t"
    : "=r" (res)
    :
    :
    );
    return res;
    */
}

// tested. works
void ish_exit(int status)
{
    asm vltl (
        "mov $60, %%rax\n\t"
        "mov %[status], %%edi\n\t"
        "syscall\n\t"
        :
        : [status]"r"(status)
        : "%rax", "%rdi"
    );
}

// tested. works
int ish_stat(const char* path, void* stat_result)
{   
    register int res asm("%eax");
    asm vltl (
        "mov $4, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        : "D"(path),"S"(stat_result)
        : 
    );    
    return res;
}

// tested. works 
int ish_open(const char *path, int flags)
{
    register int res asm("%eax");
    asm vltl (
        "mov $2, %%rax\n\t"
        "xor %%rdx, %%rdx\n\t"
        "syscall\n\t"
        : "=r"(res)
        : "D"(path), "S"(flags)
        : "%rdx"
    );
    return res;
}

// works
int ish_creat(const char *path, unsigned int mode)
{
    register int res asm("%eax");
    asm vltl (
        "mov $85, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        : "D"(path), "S"(mode)
        :
    );
    return res;
}

// works
int ish_dup2(int old_fd, int new_fd)
{
    register int res asm("%eax");
    asm vltl (
        "mov $33, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)        
        : "D"(old_fd), "S"(new_fd)
        : 
    );

    return res;
}

// works
int ish_close(int file_descriptor)
{
    register int res asm("%eax");
    asm vltl (
        "mov $3, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        : "D"(file_descriptor)
        : 
    );

    return res;
}

// works
int ish_fork()
{
    register int res asm("%eax");
    asm vltl(
        "mov $57, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        :
        :
    );

    return res;
}

// works
int ish_execve(const char *path, char *const arguments[], char *const environment[])
{
    register int res asm("%eax");
    asm vltl (
        "mov $59, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        : "D"(path), "S"(arguments), "d"(environment)
        : 
    );

    return res;
}

// works
int ish_waitpid(int pid, int *status, int options)
{
    register int res asm("%eax");
    asm vltl(
        "mov $61, %%rax\n\t"
        "xor %%r10, %%r10\n\t" // no rusage
        "syscall\n\t"
        : "=r"(res)
        : "D"(pid), "S"(status), "d"(options)
        : "%r10"
    );

    return res;
}

// works
long ish_write(int file_descriptor, const void *buffer, unsigned long buffer_size)
{
    register long res asm("%eax");
    asm vltl (
        "mov $1, %%rax\n\t"
        "syscall\n\t"
        : "=r"(res)
        :"D"(file_descriptor), "S"(buffer), "d"(buffer_size)
        : 
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
