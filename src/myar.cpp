// myar.cpp : main project file.

#include "stdinc.h"
#include "ar.h"
#include "file_stat.c"

using namespace std;

#define HELP "Correct Program Usage:\n-q quickly append files\n-x extract files\n-t print short File list\n-v print a verbose File list\n-d delete files\n-A quickly append all files in current directory\n"

string archivePath;  //Path to the archive file we are opening / reading.
int archiveFdRd = -2; //FileDescriptor for Reading.
int archiveFdWr = -2; //FileDescriptor for writing.
ar_hdr* fileList = new ar_hdr[100];

//Method Declarations
bool kill(string message);
bool error(string message);
bool checkValidAr(int fd);
bool openArchiveRead(string path, bool check);
bool openArchiveWrite(string path);
bool closeArchive();
string charToString(char chAr[], int num);
ar_hdr getHeaderAt(int start);
bool getList();
void returnShortList();
void returnFullList();

bool writeFromArchive(string file);
bool addToArchiveEnd(string file);
bool addDirToArchiveEnd(string file);

int main(int argc, char** argv)
{
	int  i;
	int qflag, dflag, xflag, Aflag = 0;	//initialize junk
	char *flags = NULL;
	char *afile = NULL;
	string* argList = new string[8];
 
    for (i = 0; i < argc; i++) {
		if (i == 1)
			flags = argv[i];
		if (i == 2)
			afile = argv[i];
		if (i >= 3) {
			argList[i - 3] = argv[i];
		}
		if (i >= 10)
			kill("too many arguments / files!  I can't handle all this awesomness!\n Please use less than 10 arguments!");
	}

	openArchiveRead(afile, true);	//open files
	openArchiveWrite(afile);

	if (strstr(flags, "t") != NULL)					//if -t  quick file list
		returnShortList();
	else if (strstr(flags, "v") != NULL)			//if -v  verbose file list
		returnFullList();
	else if (strstr(flags, "q") != NULL)			//if -q  append file to archive
		qflag = 1;
	else if (strstr(flags, "x") != NULL)			//if -x  extract file(s)
		xflag = 1;
	else if (strstr(flags, "d") != NULL)			//if -d  delete files
		dflag = 1;
	else if (strstr(flags, "A") != NULL)			//if -A  append all in folder
		Aflag = 1;
	else {
		cout << endl << HELP << endl;
		cout << "Unknown option character: " << flags << endl;
		kill("Unknown option character");
	}

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
	return true;
}

string charToString(char inArray[], int num) {
	string final = "";
	for(int i = 0; i < num; i++) {
		final += inArray[i];
	}
	return final;
}
ar_hdr getHeaderAt(int start) {
	struct ar_hdr h = {};

	lseek(archiveFdRd, start, SEEK_SET);
	read(archiveFdRd, h.ar_name, 16);
	read(archiveFdRd, h.ar_date, 12);
	read(archiveFdRd, h.ar_uid, 6);
	read(archiveFdRd, h.ar_gid, 6);
	read(archiveFdRd, h.ar_mode, 8);
	read(archiveFdRd, h.ar_size, 10);
	read(archiveFdRd, h.ar_fmag, 2);

	return h;
}
bool getList() {
	int end = lseek(archiveFdRd, 0, SEEK_END);
	lseek(archiveFdRd, 0, SEEK_SET);
	int i = 0;
	int loc = 8;
	while (lseek(archiveFdRd, 0, SEEK_CUR) < end) {
		fileList[i] = getHeaderAt(loc);
		loc = lseek(archiveFdRd, 0, SEEK_CUR) + atoi(fileList[i].ar_size);
		i++;
	}
	return true;
}
void returnShortList() {
	getList();							//Parse File
	bool quit = false;
	int i = 0;
	while (!quit && i < sizeof(fileList)) {
		if (!strncmp(fileList[i].ar_name, "", 1) == 0) {
			cout << charToString(fileList[i].ar_name, 16) << endl;		//output filenames.
		}																//NOT sanatised!
		i ++;
	}
}
void returnFullList() {
	getList();							//Parse File
	bool quit = false;
	int i = 0;
	int j = 0;
	time_t t;
	struct tm *gmp, *locp;
	struct tm gm, loc;
	while (!quit && i < sizeof(fileList)) {
		if (!strncmp(fileList[i].ar_name, "", 1) == 0) {
			cout << file_perm_string(atoi(fileList[i].ar_mode), 0);		//ouput file attribute marks
			if (atoi(fileList[i].ar_size) <= 99999);
				cout << " ";											//spacer
			for (j = 0; j < 5; j++)
				cout << fileList[i].ar_uid[j];							//output file UID
			cout << "/";												//spacer
			for (j = 0; j < 5; j++)
				cout << fileList[i].ar_gid[j];							//output FileGID
			cout << setw(7) << atoi(fileList[i].ar_size);				//output filesize
			cout << " ";												//spacer
			t = atol(fileList[i].ar_date);
			locp = localtime(&t);										//get local time
			loc = *locp;
			if (loc.tm_min == 0)										//hack for 0 spacing.
				loc.tm_min = 00;
			printf("%d %d %d:%d0 %d",									//mostly working date
				loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_year);
			cout << " ";												//spacer
			int j = 0;
			while (j<15 && true) {
				cout << fileList[i].ar_name[j];							//working file name
				j++;
				if (fileList[i].ar_name[j] == '/')						//cut off annoying wrong part of filename.
					j = 20;
			}
		}
		i ++;
		cout << endl;
	}
}

bool writeFromArchive(string file) {

}