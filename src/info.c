#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#define BLOCKSIZE 4096

bool isArchiveFile(string file) {
	fd = open(file, O_RDONLY);
	if(fd == -1){
		perror("Can't open input file");
		exit(-1);
		return false;
	}
	string head = "";
	if(fd == 0) {
		lseek(fd, SEEK_SET, 0);
		read(fd, head, 7);
		if (head == "!<arch>") {
			return true;
		}
		else {
			return false;
			perror("Not an archive file.");
		}
	}
	return false;
}

int main(int argc, char **argv)
{
	char *input = argv[1];
	char *output = argv[2];
	int in_fd;
	int out_fd;
	char buf[BLOCKSIZE];

	int num_read;
	int num_written = 0;

	int location;

	in_fd = open(input, O_RDONLY);
	if(in_fd == -1){
		perror("Can't open input file");
		exit(-1);
	}

	out_fd = open(output, O_WRONLY, S_IRWXU);
	if(out_fd == -1){
		perror("Can't open input file");
		exit(-1);
	}
	
	location = 0;

	//lseek(in_fd, -1, SEEK_END);
	while((num_read = read(in_fd, buf, BLOCKSIZE)) > 0){
		num_written = write(out_fd, buf, num_read);
		
		while(num_read != num_written){
			num_read -= num_written;
			location += num_written;
			num_written = write(out_fd, buf + location, num_read);
		}

		if (num_written == -1){
			perror("error writing - deleting output file");
			unlink(output);
			exit(-1);
		}
		
		//lseek(in_fd, -2, SEEK_CUR);

	}

	close(in_fd);
	close(out_fd);


	return 0;
}
