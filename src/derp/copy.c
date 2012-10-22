#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#define BLOCKSIZE 4096

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

	}

	close(in_fd);
	close(out_fd);

  //read
  //0 end of file
  //-1 error
  //number bytes read
	
	return 0;
}
