#define asmv __asm__ __volatile__

// tested
long ish_read(int fd, void *buffer, unsigned long buffer_size)
{
	register long res asm("a0");
	asmv(
		"li a7, 63\n\t"
		"move a0, %[fd]\n\t"
		"move a1, %[bf]\n\t"
		"move a2, %[sz]\n\t"
		"ecall"
		: "=r"(res)
		: [fd]"r"(fd), [bf]"r"(buffer), [sz]"r"(buffer_size)
		: "a7"
	);
	
    	return res;
}

//tested
int ish_chdir(const char *path)
{
	register int res asm("a0");
	//register int syscallnum asm("a7") = 49;
	asmv (
		"li a7, 49\n\t"
		"ecall"
		:"=r"(res)
		: //"r"(syscallnum)
		: "a7"
	);
    	return res;
}

//tested
void ish_exit(int status)
{
    	register int res asm("a0");
	//register int scn asm("a7") = 93;
	asmv (
		"li a7, 93\n\t"
		"ecall"
		:"=r"(res)
		://"r"(scn)
		: "a7"
	);
}

// tested
int ish_stat(const char *path, void *stat_result)
{
    	register int res asm("a0");
	asmv (
		"li a7, 79\n\t"
		"xor a3, a3, a3\n\t"
		"move a2, %[buff]\n\t"
		"move a1, %[path]\n\t"
		"li a0, -100\n\t"
		"ecall"
		: "=r"(res)
		: [path]"r"(path), [buff]"r"(stat_result)
		: "a1", "a2", "a3", "a7"
	);

    return res;
}

// tested
int ish_open(const char *path, int flags)
{
    	register int res asm("a0");
	asmv (
		"li a7, 56\n\t"
		"xor a3, a3, a3\n\t"
		"move a2, %[flags]\n\t"
		"move a1, %[path]\n\t"
		"li a0, -100\n\t"
		"ecall"
		: "=r"(res)
		: [path]"r"(path), [flags]"r"(flags)
		: "a1", "a2", "a3", "a7"
	);

    return res;
}

// tested
int ish_creat(const char *path, unsigned int mode)
{
    	register int res asm("a0");
	asmv (
		"li a7, 56\n\t"
		"xor a3, a3, a3\n\t"
		"move a3, %[mode]\n\t"
		"li a2, 577\n\t"
		"move a1, %[path]\n\t"
		"li a0, -100\n\t"
		"ecall"
		: "=r"(res)
		: [path]"r"(path), [mode]"r"(mode)
		: "a1", "a2", "a3", "a7"
	);

    return res;
}

// tested
int ish_dup2(int old_file_descriptor, int new_file_descriptor)
{
	register int res asm("a0");
	asmv (
		"li a7, 24\n\t"
		"li a2, 0\n\t"
		"ecall"
		: "=r"(res)
		: 
		: "a2", "a7"
	);
   	 return res;
}

// tested
int ish_close(int file_descriptor)
{
	register int res asm("a0");
	asmv(
		"li a7, 57\n\t"
		"ecall"
		: "=r"(res)
		:
		: "a7"
	);
    	return res;
}

// tested
int ish_fork()
{
	register int res asm("a0");
	asmv (
		"li a7, 220 \n\t"
		"li a0, 0\n\t"
		"lui a0, 0x1200\n\t"
		"addi a0, a0, 17\n\t"
		"li a1, 0\n\t"
		"li a2, 0\n\t"
		"li a3, 0\n\t"
		"addi a4, tp, -1600\n\t"
		"ecall"
		: "=r"(res)
		: 
		: "a1", "a2", "a3", "a4", "a7"
	);
	return res;
}
// tested
int ish_execve(const char *path, char *const arguments[], char *const environment[])
{
	register int res asm("a0");

	asmv(
		"li a7, 221\n\t"
		"ecall\n\t"
		: "=r"(res)
		:
		: "a7"
	);

    	return res;
}
// tested
int ish_waitpid(int pid, int *status, int options)
{
    	register int res asm("a0");
	asmv (
		"li a7, 260\n\t"
		"xor a3, a3, a3\n\t"
		"ecall"
		: "=r"(res)
		:
		: "a3", "a7"
	);

	return res;
}

// tested
long ish_write(int file_descriptor, const void *buffer, unsigned long buffer_size)
{
    	register int res asm("a0");
	
	asmv(
		"li a7, 64\n\t"
		"ecall"
		: "=r"(res)
		:
		: "a7"
	);
    	return res;
}

#ifdef asmv
#undef asmv
#endif
