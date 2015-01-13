//
//  cpu_test.h
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#ifndef VM_cpu_test_h
#define VM_cpu_test_h

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <chrono>
#include <mutex>

#include "controller_base.h"

using namespace std;

struct c_status{
	/* data */
	char name[20];
	unsigned int user;
	unsigned int nice;
	unsigned int system;
	unsigned int idle;
    
	c_status() {
		user = nice = system = idle = 0;
	}
    
	bool operator ==(const c_status& op) {
        return (user == op.user &&
                nice == op.nice &&
                system == op.system &&
                idle == op.idle);
    }
    
    bool operator !=(const c_status &op) {
    	return (this->user != op.user ||
                this->nice != op.nice ||
                this->system != op.system ||
                this->idle != op.idle);
    }
    
    c_status operator =(const c_status& s) {
    	strcpy(this->name, s.name);
    	this->user = s.user;
    	this->nice = s.nice;
    	this->system = s.system;
    	this->idle = s.idle;
    	return *this;
    }
};

class Snake {
public:
	Snake(ControllerBase *ctrl_object);
	bool start(int);
	void set_target(int);
	bool monitor();

	void test();
	
private:
	volatile int rest_time, gross;
	volatile float current;

	FILE *fwp;
	int numCPU;
	float c_usage[4];
	c_status prev, curr;
	c_status p_cpu[4], c_cpu[4];  // Only support 4 CPUs, should use pointer for more support
	mutex t_lock, c_lock;

	ControllerBase *ctrl;  // In order to make polymophism works, use a pointer
	
	//pthread_t cpu_consumer, cpu_guard;
	bool get_cpu_info(c_status &occupation);
	void controller();
	void *brute_multi(void) __attribute__((deprecated)); // This is from IBS
	void *brute_prime(void);
	void *single_cpu_monitor(void);
	void *multi_cpu_monitor(void);
    
	float update_single_cpu_usage();
	void update_multi_cpu_usage();
	//int dummy_controller() __attribute__((deprecated));
	// int get_cpu_usage() __attribute__((deprecated));
    // c_status get_process_cpu_usage();
};

typedef  void* (Snake::*SnakePtr)(void);
typedef  void* (*PthreadPtr)(void*);

#endif
