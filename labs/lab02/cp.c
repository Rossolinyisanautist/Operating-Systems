#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#define BUFF_SIZE 131072 //2^17

static char buffer[BUFF_SIZE];

int main(int argc, char** argv)
{
	char* buff = buffer;
	if(argc == 3) {
		int in_fd = openat(AT_FDCWD, argv[1], O_RDONLY);
		int out_fd = openat(AT_FDCWD, argv[2], O_WRONLY | O_CREAT | O_EXCL, 0664);
		
		ssize_t readed;
		while((readed = read(in_fd, buff, BUFF_SIZE)) > 0) {
			if(readed > BUFF_SIZE) {
				// wow what a great error >_<
			}

			ssize_t towrite = readed;
			ssize_t written = 0;

			while(towrite > 0) {	
				do {
					written = write(out_fd, buff, towrite);
				} while((written < 0) && (errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK));
				if(written < 0) {
					// non-temp error while writing
				}
				towrite -= written;
				buff += written;
			}

		}
		close(in_fd);
		close(out_fd);
	}
}
