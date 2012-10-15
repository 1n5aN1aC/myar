Myar - Lame re-write of ar in C
====================
This program will allow the user to archive and unarchive files, compatable with UNIX's ar command.

The goals of this plugin:

 * Its homework :)
 * Learn more about system calls.
 * Learn more about C, as opposed to C++

Usage
---------

myar {arguments} archivefile ...

arguments include:

 * -q quickly append named files to archive
 * -x extract named files
 * -t print a concise table of contents of the archive
 * -v print a verbose table of contents of the archive
 * -d delete named files from archive
 * -A quickly append all “regular” files in the current directory. (except the archive itself)
 * -w EC: for a given timeout, add all modified files to the archive. (except the archive itself)

License
----------
This project has a LGPL license. Do whatever you want with it.<br>