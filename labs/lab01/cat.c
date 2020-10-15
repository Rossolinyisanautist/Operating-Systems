#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFF_SIZE 131072
static char buff[BUFF_SIZE];

int main(int argc, char** argv) {
	static void* buffer = buff;
	if(argc > 1 ) {
		for(int i = 1; i < argc; i++) {
			// same as open() if AT_FDCWD provided
			int fd = openat(AT_FDCWD, argv[i], O_RDONLY);
			
			ssize_t read_res = 0;
			while((read_res = read(fd, buffer, BUFF_SIZE)) > 0) {
				if(read_res > BUFF_SIZE) {
					// something wrong happened on read
				}

				ssize_t written = 0, towrite = read_res;
				while(towrite > 0) {
					do {
						written = write(STDOUT_FILENO, buffer, towrite);
					} while((written < 0) && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK));
					if(written < 0) {
						// TODO: error handling
					}
					towrite = towrite - written;
					buffer = buffer + written;
				}
			}
			// close handle
			close(fd);
		}
	}
	else {
		ssize_t read_res = 0;
		while((read_res = read(STDIN_FILENO, buffer, BUFF_SIZE)) > 0) {
			if(read_res > BUFF_SIZE) {
				// something wrong happened on read
			}
			ssize_t written = 0, towrite = read_res;
			while(towrite > 0) {
				do {
					written = write(STDOUT_FILENO, buffer, towrite);
				} while((written < 0) && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK));
				if(written < 0) {
					// TODO: error handling
				}
				towrite = towrite - written;
				buffer = buffer + written;
			}
		}
	}
}
