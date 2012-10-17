// myar.cpp : main project file.

#include "stdinc.h"
#include "ar.h"

using namespace std;

String archivePath;
int archiveFdRd = 0;
int archiveFdWr = 0;

bool openArchiveRead(String path);
{
	archivePath = path;
	archiveFdRd = open(path, O_RDONLY);
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
bool closeArchive();

void getShortList();
void getFullList();
bool fileIsInArchive(String file);

bool writeFromArchive(String file);
bool writeFromArchiveLoc(int start, int length);

bool addToArchiveEnd(String file);
bool addDirToArchiveEnd(String file);

int main(int argc, char** argv)
{
    
	
	return 0;
}