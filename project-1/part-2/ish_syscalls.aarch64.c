#define asmvol __asm__ __volatile__

long ish_read(int file_descriptor, void *buffer, unsigned long buffer_size)
{
    register long res asm("x0");
    asmvol (
        "mov x8, #63\n\t"
        "mov x0, %[fd]\n\t" 
        "mov x1, %[buf]\n\t"
        "mov x2, %[sz]\n\t"
        "svc #0\n\t"
        : "=r"(res) // constraint x0 to res
        : [fd]"r"(file_descriptor), [buf]"r"(buffer), [sz]"r"(buffer_size)  // let compiler chose optimal registers for each of these parameters
        : "x1", "x2", "x8"    // notify compiler that x1, x2 and x8 may change during exec of asm code
    );
    return res;
}

int ish_chdir(const char *path)
{
    register int res asm("w0");
    asmvol (
        "mov x8, #49\n\t"
        "mov x0, %[path]\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [path]"r"(path) // let compiler chose best location for path
        : "x8" // tell to compiler that x8 amy be modifyed in asm
    );
    return res;
}

void ish_exit(int status)
{
    asmvol (
        "mov x8, #93\n\t"
        "mov x0, %[status]\n\t"
        "svc #0\n\t"
        :
        : [status]"r"(status) // let compiler chose register for status
        : "x0", "x8" // tell compiler that asm may modify x0 and x8
    );
}

/*
 * stat is implemented using 
 *      int fstatat(int fd, int path, void* stat_res, int flags)
 *
 * to simulate simple stat we must set fd to AT_FDCWD(-100), and flags to 0.
*/
int ish_stat(const char *path, void *stat_result)
{
    register int res asm("w0");
    asmvol (
        "mov x8, #79\n\t"
        "mov x3, #0\n\t" // no flags
        "mov x2, %[stat_res]\n\t" // result
        "mov x1, %[path]\n\t" // path
        "mov x0, #-100\n\t" // cur word dir fd
        "svc #0\n\t"
        : "=r"(res)
        : [path]"r"(path), [stat_res]"r"(stat_result)
        : "x1", "x2", "x3", "x8"
    );
    return res;
}

/* 
 * open is implemented using 
 *      int openat(int fd, int path, int flags, mode_t mode)
 *
 * we simulate open in curr work dir by 
 * sending AT_FDCWD(-100) as fd param
 * and setting mode to zero
*/
int ish_open(const char *path, int flags)
{
    register int res asm("w0");
    __asm__ __volatile__ (
        "mov x8, #56\n\t"
        "mov x3, #0\n\t"
        "mov x2, %[flags]\n\t"
        "mov %[flags], %[path]\n\t"
        "mov x0, #-100\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [path]"r"(path), [flags]"r"(flags) // let compiler chose optimal register for these parameters
        : "x2", "x3", "x8" // norify compiler that these registers may be modifyed
    );
    return res;
}

/*
 * creat is implemented through 
 *      int openat(int fd, int path, int flags, mode_t mode)
 *
 * to simulate creat
 * set flags = (O_CREAT | O_WRONLY | O_TRUNC) (577)
 * set fd = AT_FDCWD (-100)
 * set mode = 0
*/
int ish_creat(const char *path, unsigned int mode)
{
    register int res asm("w0");

    asmvol(
        "mov x8, #56\n\t"
        "mov x3, %[mode]\n\t"
        "mov x2, #577\n\t"
        "mov x1, %[path]\n\t"
        "mov x0, #-100\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [path]"r"(path), [mode]"r"(mode)
        : "x1", "x2", "x3", "x8"  
    );
    return res;
}

/*
 * dup2 is implemented using 
 *      int dup3(int oldfd, int newfd, int flags)
 *
 * to simulate dup2 we need to set flags to 0
*/
int ish_dup2(int old_fd, int new_fd)
{
    register int res asm("w0");

    asmvol(
        "mov x8, #24\n\t"
        "mov x2, #0\n\t" // no flags
        "mov x1, %[newfd]\n\t"
        "mov x0, %[oldfd]\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [oldfd]"r"(old_fd), [newfd]"r"(new_fd)
        : "x1", "x2", "x8"
    );
    return res;
}

// arm provides implementation of close syscall
int ish_close(int fd)
{
    register int res asm("w0");
    asmvol (
        "mov x8, #57\n\t"
        "mov x0, %[fd]\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [fd]"r"(fd)
        : "x8"
    );
    return res;
}

int ish_fork()
{
    register int res asm("w0");
    asmvol (
        "mrs x21, tpidr_el0\n\t"
        "sub x5, x21, #0x700\n\t"
        "add x4, x5, #0xd0\n\t"
        "mov x3, #0\n\t"
        "mov x2, #0\n\t"
        "mov x1, #0\n\t"
        "mov x0, #0x11\n\t"
        "movk x0, #0x120, lsl #16\n\t"
        "mov x8, #220\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : 
        : "x1", "x2", "x3", "x4", "x5", "x8", "x21"
    );
    return res;
}

// arm provides own impl of execve syscall
int ish_execve(const char *path, char *const arguments[], char *const environment[])
{
    register int res asm("w0");
    asmvol(
        "mov x8, #221\n\t"
        "mov x0, %[path]\n\t"
        "mov x1, %[argv]\n\t"
        "mov x2, %[envp]\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [path]"r"(path), [argv]"r"(arguments), [envp]"r"(environment)
        : "x1", "x2", "x8"
    );
    return res;
}

/*
 * waitpid is implemented using wait4
 *      pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage)
 *
 * to simulate waitpid just set 4th param to zero
 * rusage = 0
*/
int ish_waitpid(int pid, int *status, int options)
{
    register int res asm("w0");
    asmvol (
        "mov x8, #260\n\t"
        "mov x0, %[pid]\n\t"
        "mov x1, %[status]\n\t"
        "mov x2, %[options]\n\t"
        "mov x3, #0\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [pid]"r"(pid), [status]"r"(status), [options]"r"(options)
        : "x1", "x2", "x3","x8"
    );
    return res;
}

// arm provides impl of write syscall
long ish_write(int fd, const void *buffer, unsigned long buffer_size)
{
    register long res asm("x0");
    asmvol (
        "mov x8, #64\n\t"
        "mov x0, %[fd]\n\t"
        "mov x1, %[buf]\n\t"
        "mov x2, %[sz]\n\t"
        "svc #0\n\t"
        : "=r"(res)
        : [fd]"r"(fd), [buf]"r"(buffer), [sz]"r"(buffer_size)
        : "x1", "x2", "x8"
    );

    return res;
}

#ifdef asmvol
#undef asmvol
#endif
