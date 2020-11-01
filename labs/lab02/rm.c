#include <unistd.h>
#include <fcntl.h>

int main(int argc, char** argv)
{
	for(int i = 1; i < argc; ++i) {
		unlinkat(AT_FDCWD, argv[i], 0);
	}
	return 0;
}
