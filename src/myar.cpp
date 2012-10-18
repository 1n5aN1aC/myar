// myar.cpp : main project file.

#include "stdinc.h"
#include "ar.h"

using namespace std;

#define HELP "-q quickly append files\n-x extract files\n-t print short File list\n-v print a verbose File list\n-d delete files\n-A quickly append all files in current directory\n"

string archivePath;  //Path to the archive file we are opening / reading.
int archiveFdRd = -2; //FileDescriptor for Reading.
int archiveFdWr = -2; //FileDescriptor for writing.
//ar_hdr* fileList = new ar_hdr[1];;
ar_hdr* fileList[3];

bool kill(string message) { //Ends program with error.
	perror(message.c_str());
	exit(-1);
	return false;
}
bool error(string message) {
	perror(message.c_str());
	return false;
}

int curLoc(int fd) {
	return lseek(fd, SEEK_CUR, 0);
}

bool checkValidAr(int fd) {
	char head[SARMAG];
	lseek(fd, SEEK_SET, 0);
	read(fd, head, SARMAG);
	if (strncmp(head, ARMAG, 8) == 0)
		return true;
	else
		return kill("Archive file header is Corrupt.");
}
bool openArchiveRead(string path, bool check) {
	archivePath = path;
	archiveFdRd = open(path.c_str(), O_RDONLY);
	if (archiveFdRd == -1)
		return kill("Can't open input file.");
	else if (archiveFdRd == -2)
		return kill("Something Really Bad Happened.");
	else if (check == true)
		return checkValidAr(archiveFdRd);
	else
		return true;
}
bool openArchiveWrite(string path, bool check) {
	if (archivePath != path)
		return kill("Please Open For Reading Before Writing.");
	archiveFdWr = open(path.c_str(), O_WRONLY);
	if(archiveFdWr == -1)
		return kill("Can't open output file.");
	else if(archiveFdWr == -2)
		return kill("Something Really Bad Happened.");
	else if (check == true)
		return checkValidAr(archiveFdWr);
	else {
		return true;
	}
}
bool closeArchive() {
	if (close(archiveFdRd) == -1)
		return error("could not close Read fd");
	if (close(archiveFdWr) == -1)
		return error("could not close Write fd");
	return true;
}

ar_hdr* readHeaderAt(int start) {
	lseek(archiveFdRd, SEEK_SET, start);
	struct ar_hdr* h = (struct ar_hdr*)malloc(sizeof(struct ar_hdr));
	read(archiveFdRd, h, sizeof(struct ar_hdr));
	if (archiveFdRd == -1) {
		perror("Failed to read archive!");
		exit(-1); 
	}
	return h;
}
bool getList() {
	lseek(archiveFdRd, SEEK_SET, SARMAG);
	struct ar_hdr* h = readHeaderAt(curLoc(archiveFdRd));

	int i = sizeof(fileList); //find size of array.  since arrays start at 0, size will be end + 1
	memcpy ( &fileList[i], &h, sizeof(h) );  //Save read struct into struct array
	
	return true;
}
void returnShortList();
void returnFullList();
bool fileIsInArchive(string file);

bool writeFromArchive(string file);
bool writeFromArchiveLoc(int start, int length);

bool addToArchiveEnd(string file);
bool addDirToArchiveEnd(string file);

int main(int argc, char** argv)
{
	openArchiveRead("./ex.a", true);
	openArchiveWrite("./ex.a", true);
	closeArchive();
	getList();
	cout << fileList[0]->ar_name;

    cout << "Program Finished Main :)" << "\n";
	
	return 0;
}