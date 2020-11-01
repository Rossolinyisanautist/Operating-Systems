#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char** argv) 
{
	for(int i = 1; i < argc; ++i) {
		mkdir(argv[i], 0777);
	}
}
