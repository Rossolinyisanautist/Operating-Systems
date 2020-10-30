
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#define HANDLE int

typedef long ssize_t;
typedef unsigned long size_t;

ssize_t write(HANDLE fd, const void* buff, size_t len) {
	ssize_t written;
	
	__asm__ __volatile__ (
		"mov $1, %%rax\n\t"
		"syscall\n\t"
		: "=a"(written)
	);
	
	return written;
}

size_t strlen(const char* str) {
	size_t res = 0;
	while(*str++) {
		res++;
	}
	return res;
}

int main(int argc, char** argv) {
	
	for(int i = 1; i < argc; i++) {
		write(STDOUT_FILENO, argv[i], strlen(argv[i]));	
		if(i < argc - 1) {
			write(STDOUT_FILENO, " ", 1);
		}
	
	}
	write(STDOUT_FILENO, "\n", sizeof("\n") - 1);	
	
	return 0;
}
