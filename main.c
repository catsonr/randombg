#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

int fileCount;

int dirProvided = 0;
const char* dirString;

#define loopFlag 'l'
int looping = 0;

#define loopTimerFlag 't'
int loopLength = 60 * 60;

#define outputFlag 'o'
int outputting = 0;

void countFiles();
char* getFileFromDir(const char* dir, int index);
int loopLengthToInt(char* lengthStr);
char* generateCommand();

int main(int argc, char **argv) {
	// seeds randomness from time
	srand(time(0));

	// loops over all args provided	
	for(int i = 1; i < argc; i++) {

		// checks if arg is directory
		if(!dirProvided) {
			DIR* dirCheck = opendir(argv[i]);
			if(dirCheck) {
				dirProvided = 1;
				dirString = argv[i];

				continue;
			}
		}

		// checks for flags 
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				case loopFlag:
					looping = 1;

					break;
				case loopTimerFlag: {// brackets because compiler gets angry when defining variables at the start of a switch case
					char* loopLengthStr = malloc(strlen(argv[i]) - 1);
					strcpy(loopLengthStr, &argv[i][2]);
					loopLength = loopLengthToInt(loopLengthStr);

					break;
				case outputFlag:
					outputting = 1;
					break;
				}
			}
		}
	}

	// if user didn't provide directory
	if(!dirProvided) {
		printf("please provide a directory. exiting...\n");
		return 0;
	}

	do {
		countFiles();
		system(generateCommand());
		if(outputting) printf("background changed\n");
		if(looping) sleep(loopLength);
	} while(looping);

	return 0;
}

void countFiles() {
	struct dirent* dir;
	DIR* dirCheck = opendir(dirString);
	fileCount = 0;
	while((dir = readdir(dirCheck)) != NULL)
		if(strcpy(dir -> d_name, ".") != 0 && strcmp(dir -> d_name, "..") != 0) fileCount++;
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

int loopLengthToInt(char* lengthStr) {
	const char timeType = lengthStr[strlen(lengthStr) - 1];
	lengthStr[strlen(lengthStr) - 1] = 0;

	switch(timeType) {
		case 's':
			return atoi(lengthStr);
			break;
		case 'm':
			return atoi(lengthStr) * 60;
			break;
		case 'h':
			return atoi(lengthStr) * 60 * 60;
			break;
		case 'd':
			return atoi(lengthStr) * 60 * 60 * 24;
			break;
	}
	
	return -1;
}

char* generateCommand() {
	char* initialCommand = "feh --bg-fill ";
	char* file = getFileFromDir(dirString, rand() % fileCount + 1);
	char* command = malloc(strlen(initialCommand) + strlen(dirString) + strlen("/''") + strlen(file) + 1);

	strcpy(command, initialCommand);
	strcat(command, dirString);
	strcat(command, "/'");
	strcat(command, file);
	strcat(command, "'");

	if(strcmp(command, "") != 0) return command;
	return "echo unable to change background (error generating command)";
} 
