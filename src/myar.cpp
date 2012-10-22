// myar.cpp : main project file.

#include "stdinc.h"
#include "/usr/include/ar.h"
#include "file_stat.c"

using namespace std;

#define HELP "Correct Program Usage:\n-q quickly append files\n-x extract files\n-t print short File list\n-v print a verbose File list\n-d delete files\n-A quickly append all files in current directory\n"
#define BLOCKSIZE 4096

string archivePath;  //Path to the archive file we are opening / reading.
int archiveFdRd = -2; //FileDescriptor for Reading.
int archiveFdWr = -2; //FileDescriptor for writing.
ar_hdr* fileList = new ar_hdr[100];
string* argList = new string[8];

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
void fixList();
void returnShortList();
void returnFullList();

bool writeFromArchive();
bool writeToFileFromArcLoc(string file, int loc, int length);

bool addToArchiveEnd(string file);
bool addDirToArchiveEnd(string file);

int main(int argc, char** argv)
{
	int  i;
	int qflag, dflag, Aflag = 0;	//initialize junk
	char *flags = NULL;
	char *afile = NULL;
 
    for (i = 0; i < argc; i++) {
		if (i == 1 && argc > 1)
			flags = argv[i];
		else if (i == 2 && argc > 2)
			afile = argv[i];
		else if (i >= 3 && argc > 3)
			argList[i - 3] = argv[i];
		else if (i >= 10 && argc > 10)
			return kill("too many arguments / files!  I can't handle all this awesomness!\n Please use less than 10 arguments!");
	}

	if (argc < 3) {
		cout << HELP << endl;
		return kill("Improper use of Myar");
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
		writeFromArchive();
	else if (strstr(flags, "d") != NULL)			//if -d  delete files
		dflag = 1;
	else if (strstr(flags, "A") != NULL)			//if -A  append all in folder
		Aflag = 1;
	else {
		cout << endl << HELP << endl;
		cout << "Unknown option character: " << flags << endl;
		return kill("Unknown option character");
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
	fixList();
	return true;
}
void fixList() {
	int i = 0;
	while (i < sizeof(fileList)) {
		int j = 0;
		char let = ' ';
		while (j < 15) {
			let = fileList[i].ar_name[j];
			if (let == '/')
				fileList[i].ar_name[j] = '\0';		//null (end of string)
			j++;
		}
		i++;										//TODO- check for valid ARFMAG
	}
}
void returnShortList() {
	getList();							//Parse File
	int i = 0;
	while (i < sizeof(fileList)) {
		if (!strncmp(fileList[i].ar_name, "", 1) == 0) {
			cout << charToString(fileList[i].ar_name, 16) << endl;		//output filenames.
		}																//NOT sanatised!
		fileList[i].ar_name[strlen(fileList[i].ar_name)-1] = '\0';		//TODO- check this
		i ++;
	}
}
void returnFullList() {
	getList();							//Parse File
	int i = 0;
	int j = 0;
	time_t t;
	struct tm *gmp, *locp;
	struct tm gm, loc;
	while (i < sizeof(fileList)) {
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
			printf("%d %d %02d:%02d %d",									//mostly working date
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

bool writeFromArchive() {
	getList();
	int fileSizeSoFar = 0;

	int i;
	for (i = 0; i < 6; i++) {							//iterate through FileList
		int j;
		for (j = 0; j < sizeof(argList); j++) {			//iterate through argList
			if (strcmp(argList[j].c_str(), fileList[i].ar_name) == 0) {
				cout << "Now writing file " << fileList[i].ar_name << " ...." << endl;
				writeToFileFromArcLoc(charToString(fileList[i].ar_name, 16), 8 + (i+1)*60 + fileSizeSoFar , atoi(fileList[i].ar_size));
			}
		}
		fileSizeSoFar += atoi(fileList[i].ar_size);
	}
	return true;
}
bool writeToFileFromArcLoc(string path, int loc, int length) {
	char buf[BLOCKSIZE];
	int num_read = 0;
	int tot_read = 0;
	int num_written = 0;

	int fd_write = open(path.c_str(), O_WRONLY | O_CREAT, S_IRWXU);
	if(fd_write == -1)
		return kill("Can't open output file");
	
	lseek(fd_write, 0, SEEK_SET);
	lseek(archiveFdRd, loc, SEEK_SET);
	while(tot_read < length) {
		if (length - tot_read < BLOCKSIZE) {
			num_read = read(archiveFdRd, buf, length - num_read);
			tot_read += num_read;
		}
		else
			num_read += read(archiveFdRd, buf, BLOCKSIZE);
		num_written += write(fd_write, buf, num_read);				//write
		if (num_read != num_written)
			return kill("Error Occured during writing");
		num_read = 0;
	}
	close(fd_write);
	return true;
}