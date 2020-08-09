#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int fileCount;

int dirProvided = 0;
const char* dirString;

#define loopFlag 'l'
int looping = 0;

#define loopTimerFlag 't'
char* loopTimer;

char* getFileFromDir(const char* dir, int index);
char* generateCommand();

int main(int argc, char **argv) {
	srand(time(0));

	// loops over all args provided, sans the program call
	for(int i = 1; i < argc; i++) {

		// checks if arg is directory
		if(!dirProvided) {
			DIR* dirCheck = opendir(argv[i]);
			if(dirCheck) {
				dirProvided = 1;

				dirString = argv[i];

				struct dirent* dir;
				fileCount = 0; 

				// loops through all files in dir
				while((dir = readdir(dirCheck)) != NULL) {
					// ignores . and ..
					if(strcmp(dir -> d_name, ".") != 0 && strcmp(dir -> d_name, "..") != 0) fileCount++; 
				}
				closedir(dirCheck);

				continue;
			}
		}

		// checks for flags 
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				case loopFlag:
					looping = 1;
					break;
				case loopTimerFlag:
					for(int j = 1; i < sizeof(argv[j]) / sizeof(char*); j++) {
						loopTimer[j - 1] = argv[i][j];
					}
					break;
			}
		}
	}

	// checks for needed things before executing commands
	if(!dirProvided) {
		printf("please provide a directory. exiting...\n");
		return 0;
	}



	do {
		system(generateCommand());
		if(looping) sleep(60 * 60);
		//generateCommand();
	} while(looping);

	return 0;
}

char* getFileFromDir(const char* dir, int index) {
	int i = 1;
	DIR* dirCheck = opendir(dir);
	struct dirent* dir_;
	while((dir_ = readdir(dirCheck)) != NULL) {
		if(strcmp(dir_ -> d_name, ".") != 0 && strcmp(dir_ -> d_name, "..") != 0)
			if(index == i++) return dir_ -> d_name;
	}

	return "";
}

char* generateCommand() {
	char* initialCommand = "feh --bg-fill ";
	char* file = getFileFromDir(dirString, rand() % fileCount + 1);
	//char* command = malloc(sizeof(char) * (strlen(initialCommand) + strlen(dirString) + strlen(file) + 9)); // +3 for /''
	char command[1000];

	strcpy(command, initialCommand);
	strcat(command, dirString);
	strcat(command, "/'");
	strcat(command, file);
	strcat(command, "'");

	if(strcmp(command, "") != 0) return strdup(command);
	return "echo unable to change background (error generating command)";
} 
