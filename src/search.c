#include "search.h"

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "globals.h"

int containsKeyword(const char *filename, const char *keyword) { return strstr(filename, keyword) != NULL; }

void searchFiles(const char *directory, const char *keyword)
{
	DIR *dir;
	struct dirent *entry;

	if ((dir = opendir(directory)) == NULL) {
		perror("Error opening directory");
		exit(EXIT_FAILURE);
	}

	while ((entry = readdir(dir)) != NULL) {
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
			continue;
		}

		struct stat statbuf;
		char path[PATH_MAX];
		snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

		if (stat(path, &statbuf) == -1) {
			perror("Error getting file status");
			continue;
		}

		if (containsKeyword(entry->d_name, keyword) || strchr(keyword, 'k') != NULL) {
			printf("%s/%s\n", directory, entry->d_name);
		}

		if (S_ISDIR(statbuf.st_mode)) {
			searchFiles(path, keyword);
		}
	}

	closedir(dir);
}

void runFileSearch(const char *directory, const char *keyword)
{
	printf("Searching for files with keyword '%s' in directory '%s':\n", keyword, directory);
	searchFiles(directory, keyword);
	printf("Search complete.\n");
}
