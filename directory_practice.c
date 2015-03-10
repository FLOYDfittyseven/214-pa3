#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "directory_practice.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>

int printDir (char *dirName)
{
	DIR *directory;
	struct dirent *entry;
	char path[PATH_MAX];
	int pathLen;
	struct stat statbuf;
	int x;
	
	directory = opendir(dirName);
	
	if(!directory)
	{
		fprintf(stderr, "Cannot open directory '%s'\n", dirName);
		return 0;
	}
	
	while(entry = readdir(directory))
	{
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
		{
			pathLen = snprintf(path, PATH_MAX, "%s/%s", dirName, entry->d_name);
			
			printf("%s\n", path);
			
			if(pathLen >= PATH_MAX)
			{
				fprintf(stderr, "Path is too long\n");
				return 0;
			}
			
			stat(path, &statbuf);
			
			/* Check if directory
			 */
			if(S_ISDIR(statbuf.st_mode))
			{
				x = printDir(path);
				
				if(!x)
				{
					fprintf(stderr, "Cannot open directory '%s'\n", path);
					return 0;
				}
			}
		}
	}
	
	closedir(directory);
	
	return 1;
}

int main ( int argc, char *argv[] )
{
	int y;
	
	if(argc < 2)
	{
		fprintf(stderr, "Invalid number of arguments\n");
		exit(EXIT_FAILURE);
	}
	
	y = printDir(argv[1]);
	
	if(!y)
	{
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}
