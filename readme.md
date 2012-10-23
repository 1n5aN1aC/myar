Myar - Lame re-write of ar in C
====================
This program will allow the user to archive and unarchive files, compatable with UNIX's ar command.

The goals of this plugin:

 * Its homework :)
 * Learn more about system calls in UNIX.
 * Learn more about C, as opposed to C++

Compiling
---------

cd src
make
to compile myar

cd doc
make
to compile readme & documentation
 
Usage
---------

myar <argument> <archivefile> [<filename>]...

arguments include:

 * -q quickly append named files to archive
 * -x extract named files
 * -t print a concise table of contents of the archive
 * -v print a verbose table of contents of the archive
 * -d delete named files from archive
 * -A quickly append all “regular” files in the current directory. (except the archive itself)