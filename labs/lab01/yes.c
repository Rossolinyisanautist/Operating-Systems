#include <stdio.h>
#include <stdbool.h>
int main(int argc, char** argv) {

	if(argc > 1) {
		while(true) {
			int n = 1;
			while(n < argc) {
				printf(n < argc-1 ? "%s " : "%s\n", argv[n]);
				n++;
			}
		}
	}
	else {
		while(true) {
			puts("y");
		}
	}
	return 0;
}
