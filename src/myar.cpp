// myar.cpp : main project file.

#include "stdinc.h"
#include "ar.h"

using namespace std;

String archivePath;
int archiveFd = 0;

bool setArchive(String path, bool check);
int openArchive();
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