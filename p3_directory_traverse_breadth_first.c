/*
Problem	3:			Write	a	program	directory_traverse_breadth_first that	
takes	as	argument	a	path	to	a	directory	searching	for	a	file	name.		To	convince	us	
that	your	program	is	working	as intended,	print	out	the	directory	and	file	names	it	
encounters	in	the	search.		
*/


#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>	
#include <string.h>	// for string compare

//This is struct that simulate a primitive queue
struct Queue{
	char pathNames[1000][500];	// array of mutable c strings, max path length = 500, maximum search = 1000 times
	int head;			// head tail index structure, will be initialized to 0
	int tail;	
};


int search(char* pathName, char* nameOfTarget) {
	// Counter initialization
	int count = 0;
	// directory pointer
	DIR* dir;
	// dirent struct
	struct dirent* fileInfo;
	
	// queue initialization
	struct Queue queue;
	queue.head = 0;
	queue.tail = 0;

	// char childPath[500];
	
	strcpy(queue.pathNames[queue.head], pathName);	// Basically enqueuing a pathName
							// Note: Must use strcpy instead of = (deep vs shallow) 
	queue.head++;

	while(queue.head != queue.tail) {
		// dequeue and attemp to open dir with the pathName obtained form the queue
		printf(/*"Queue Tail pointing to: %d Read the tail of the queue: */"%s\n" /*, queue.tail*/, queue.pathNames[queue.tail]);
		dir = opendir((const char*)queue.pathNames[queue.tail]);
		// printf("dir is: %llu\n\n", (unsigned long long) dir);
		while(dir) {
			fileInfo = readdir(dir);
			if (!fileInfo) { break; }
			// printf("Current File Name: %s\n.", fileInfo->d_name);
			if (0 == strcmp(nameOfTarget, fileInfo->d_name)) { count++; }
			
			// Create a string to hold child Path if name of target is not . or ..
			if ( (0 != strcmp(fileInfo->d_name, ".") ) && (0 != strcmp(fileInfo->d_name, "..")) ) {
				char childPath[500];
				// getcwd(childPath, 500);
				strcpy(childPath, queue.pathNames[queue.tail]);
				strcat(childPath, "/");
				strcat(childPath, fileInfo->d_name);
				// strcat(childPath, "\n");
				// printf("Current Directory Name: %s\n\n", childPath);

				// Put the child Path onto queue
				strcpy(queue.pathNames[queue.head], childPath);	//this is basically enqueuing a pathName
				queue.head++;
				// memset(childPath, 0, 500);
			}
		}

		// dequeue
		// queue.pathNames[queue.tail] = NULL;
		queue.tail++;
		// printf("dequeue!\n\n");	
	}

	return count;
}

int main (int argc, char* argv[]) {
	if(argc != 3) {
		perror("Input Error! Shame on you! Not a");
		return(-1);
	}

	printf("The path to start the search is: %s\n", argv[1]);
	printf("The name of the target file is: %s\n", argv[2]);
	
	DIR* firstEntry = opendir(argv[1]);
	if(!firstEntry) {
		perror("File Open Error:");
		exit(-1);
	}

	else {
		printf("===============Search Starts===============\n\n");
		int result = search(argv[1], argv[2]);
		printf("===============Results Are in===============\n\n");
		if (result > 0) { printf("Number of Match(es): %d\n", result); }
		else { printf("There is no match.\n"); }
	}
}
