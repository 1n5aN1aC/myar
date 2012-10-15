/*
  All process have 3 file descriptors.
  0- stdin
  1- stdout
  2- sterr
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char **argv)
{
  int fd = open("test.txt", O_WRONLY | O_CREAT, S_IRWXU);
  if (fd == -1)
  {
    perror("derp herp went wrong");
	exit(-1);
  }
  
  int array[] = {1,2,3,4,5};  //can use with structs as well as shortcut
  
  int num_written = write(fd, array, 5 * sizeof(int));   //sizeof(array[0])        //count is in bytes
  
  if (close(fd) == -1) {
  
  
  }
  
  return 0;
}
