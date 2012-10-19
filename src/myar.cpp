// myar.cpp : main project file.

#include "stdinc.h"
#include "ar.h"

using namespace std;

#define HELP "-q quickly append files\n-x extract files\n-t print short File list\n-v print a verbose File list\n-d delete files\n-A quickly append all files in current directory\n"

string archivePath;  //Path to the archive file we are opening / reading.
int archiveFdRd = -2; //FileDescriptor for Reading.
int archiveFdWr = -2; //FileDescriptor for writing.
ar_hdr* fileList = new ar_hdr[5];

//Method Declarations
bool kill(string message);
bool error(string message);
bool checkValidAr(int fd);
bool openArchiveRead(string path, bool check);
bool openArchiveWrite(string path);
bool closeArchive();
void readHeaderAt(int start, int arrayLoc);
ar_hdr* bruteForceHeaderAt(int start);
bool getList();
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
	openArchiveWrite("./ex.a");

	bruteForceHeaderAt(9);

	//getList();

	//cout << fileList[0].ar_name;

    cout << "Program Finished Main :)" << endl;
	

	closeArchive();
	return 0;
}

bool kill(string message) { //Ends program with error.
	perror(message.c_str());
	exit(-1);
	return false;
}
bool error(string message) {
	perror(message.c_str());
	return false;
}

bool checkValidAr(int fd) {
	char head[SARMAG];
	lseek(fd, 0, SEEK_SET);
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
bool openArchiveWrite(string path) {
	if (archivePath != path)
		return kill("Please Open For Reading Before Writing.");
	archiveFdWr = open(path.c_str(), O_WRONLY);
	if(archiveFdWr == -1)
		return kill("Can't open output file.");
	else if(archiveFdWr == -2)
		return kill("Something Really Bad Happened.");
	else {
		return true;
	}
}
bool closeArchive() {
	if (close(archiveFdRd) == -1)
		return kill("could not close Read fd");
	if (close(archiveFdWr) == -1)
		return kill("could not close Write fd");
	cout << "DERP CLOSED FILE" << endl;
	return true;
}

void readHeaderAt(int start, int arrayLoc) {
	lseek(archiveFdRd, start, SEEK_SET);								//seek to where the header starts
	struct ar_hdr* h = (struct ar_hdr*)malloc(sizeof(struct ar_hdr));	//Create a temporary struct.
	read(archiveFdRd, h, sizeof(struct ar_hdr));						//Read into temporary struct

	cout << fileList[0].ar_name << endl;

	//memcpy ( &fileList[arrayLoc], &h, sizeof(h) );						//Save read struct into struct array
}
ar_hdr* bruteForceHeaderAt(int start) {
	struct ar_hdr* h = (struct ar_hdr*)malloc(sizeof(struct ar_hdr));

	lseek(archiveFdRd, start, SEEK_SET);
	read(archiveFdRd, h.ar_name, 16);
	cout << h.ar_name;

	return h;
}
bool getList() {
	readHeaderAt(8, 0);
	//cout << fileList[0].ar_name << endl;
	return true;
}