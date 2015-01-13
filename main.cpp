/**
 * Author: Yang Liu, Ruimin Sun, Yuemin Li
 * Virtual Computer Project - Performance Isolation bench
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>  // getpid use this header
#include <string.h>
#include <thread>
#include <chrono>

#include "cpu_test.h"
#include "simple_controller.h"
#include "KFilter.h"

using namespace std;

int main(int argc, const char * argv[]) {
	int pid = getpid();
	printf("Start running at pid %d\n", pid);
	sleep(1);

	Simple_controller *ctrl = new Simple_controller();
	Snake snake(ctrl);

	// KFilter *filter = new KFilter();
	// Snake snake(filter);
	int usage = -1;
	bool monitor = false;

	for(int i=0; i<argc; i++) {
		if(!strcmp(argv[i], "-c")) {
			if(i+1 == argc) {
				printf("Attemp to set CPU usage to 80%c\n", '%');
				usage = 80;
			} else {
				try{
					usage = atoi(argv[i+1]);
					printf("Setting CPU usage to %d%c\n", usage, '%');
				} catch(int e) { printf("Invalid CPU usage value.\n"); }
			}
		} else if (!strcmp(argv[i], "-m")) {
			monitor = true;
		}
	}

	if(monitor) { snake.monitor(); }
	else if (usage == -1) {
		printf("Attemp to set CPU usage to default: 80%c\n", '%');
		usage = 80;
		snake.start(80);
	} else {
		snake.start(usage);
	}
	
	return 0;
}
