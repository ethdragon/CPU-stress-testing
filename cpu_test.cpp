//
//  cpu_test.cpp
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#include "cpu_test.h"

Snake::Snake(ControllerBase *ctrl_obj) {
	current = 0;
	rest_time = 80;
	gross = 500;
	ctrl = ctrl_obj;

	// Test number of CPU cores
	numCPU = sysconf( _SC_NPROCESSORS_ONLN );
	if(numCPU < 1) {
		printf("%s\n", "Cannot get CPU number");
		exit(4);
	}
	printf("CPU(s): %d\n", numCPU);
	for(int i=0; i<numCPU; i++) { c_usage[i] = 0; }
	fwp = fopen("result.txt", "w+");
}

bool Snake::start(int target_usage) {

	ctrl->set_target(target_usage);
    SnakePtr consumer, guard;
    PthreadPtr ptr_consumer, ptr_guard;
    pthread_t t_consumer, t_guard;
    
    consumer = &Snake::brute_prime;
    if(numCPU > 1) { guard = &Snake::multi_cpu_monitor; }
    else { guard = &Snake::single_cpu_monitor; }
    
    ptr_consumer = *(PthreadPtr *) &consumer;
    ptr_guard = *(PthreadPtr *) &guard;
    
    // pthread_create return 0 on success, error value on failure
    if (pthread_create(&t_consumer, 0, ptr_consumer, this)) {
        pthread_detach(t_consumer);
        return 0;
    }
    if (pthread_create(&t_guard, 0, ptr_guard, this)) {
        pthread_detach(t_guard);
        return 0;
    }
    pthread_join(t_consumer, NULL);
    pthread_join(t_guard, NULL);
    return 1;
}

bool Snake::monitor() {
	SnakePtr monit;
	PthreadPtr ptr_monit;
	char buff[20];
	pthread_t t_monit;

	if (numCPU > 1) {
		monit = &Snake::multi_cpu_monitor;
		for(int i=0; i<numCPU; i++) {
			printf("CPU%d\t", i);
			sprintf(buff, "CPU%d\t", i);
			fwrite(buff, 1, 5, fwp);
		}
		printf("\n");
		fwrite("\n", 1, 1, fwp);
		fflush(fwp);
	}
	else {
		monit = &Snake::single_cpu_monitor;
		printf("CPU\n");
		fwrite("CPU\n", 1, 4, fwp);
		fflush(fwp);
	}

	ptr_monit = *(PthreadPtr *) &monit;
	if (pthread_create(&t_monit, 0, ptr_monit, this)) {
		pthread_detach(t_monit);
		return 0;
	}
	pthread_join(t_monit, NULL);
	return 1;
}

void Snake::update_multi_cpu_usage() {
	FILE *fp;
	char buff[4][256];
	int i;
	float max=0;
	fp = fopen("/proc/stat", "r");
	if (fp == NULL) {
		perror("Error opening /proc/stat \n");
		return;
	}
	else {
		if(fgets(buff[0], 256, fp) == NULL) {
			perror ("Error reading the /proc/stat \n");
			return;
		}
		for(i=0; i<numCPU; i++) {
			fgets(buff[i], 256, fp);
		}
		fclose(fp);
	}

	unsigned long s1, s2, t1, t2;
	int len;
	for(i=0; i<numCPU; i++) {
		p_cpu[i] = c_cpu[i];
		sscanf(buff[i], "%s %u %u %u %u", c_cpu[i].name, &c_cpu[i].user,
           &c_cpu[i].nice, &c_cpu[i].system, &c_cpu[i].idle);
		s1 = p_cpu[i].user + p_cpu[i].nice + p_cpu[i].system;
		s2 = c_cpu[i].user + c_cpu[i].nice + c_cpu[i].system;
		t1 = s1 + p_cpu[i].idle;
		t2 = s2 + c_cpu[i].idle;
		if (t2-t1 > 0) {
			c_usage[i] = ((float)(s2-s1))/((float)(t2-t1))*100.0;
		}
		len = sprintf(buff[i], "%4.2f\t", c_usage[i]);
		max = c_usage[i] > max ? c_usage[i] : max;
		printf("%s", buff[i]);
		fwrite(buff[i], 1, len, fwp);
	}
	c_lock.lock();
	current = max;
	c_lock.unlock();
	printf("\n");
	fwrite("\n",1 , 1, fwp);
	fflush(fwp);
}

void Snake::set_target(int target_usage) {
	ctrl->set_target(target_usage);
}

void Snake::test() {
	/*
     int per = get_cpu_usage();
     printf("%d\n", per);*/
	//brute_multi();
}

void *Snake::brute_prime(void) {
	int end, sq, cnt;
	cnt = 1; end= 100000000;
	// printf("%d ", 2);
	while(1) {
		for(int i=3; i<end; i++) {
			sq = sqrt(i);
        	int j;
			for(j=2; j<sq; j++) {
				if(!(i%j)) { break; }
			}
			if(i%j) {
				cnt++;
			}
			// dummy_controller();
			// update too frequently leeds to too much IO overhead
			// the controller is changed to the other thread and updated
			// only when the cpu usage is read and updated.
			// i % 500 is tuned by hand, the value may vary under different
			// operation systems.
			if(!(i%gross)) {
			usleep(rest_time); }
			//printf("rest time: %d\n", rest_time);
		}
	}
	return 0;
}

void *Snake::brute_multi(void) {
	int i,j,l,k,m,jj;
	i = 1118; j=89;
	k= 31455;l= 16452; m= 9823; jj=2342;
	while(1) {
		m = m^l;k = (k/m * jj) % i;l=j*m*k;

		i = (j * k)^m ;k = (k/m * jj) % i;m = m^l;
		m = m^l;i = (j * k)^m ;k = (k/m * jj) % i;
		m=i*i*i*i*i*i*i;// m=k*l*jj*l;
		m = m^l;k = (k/m * jj) % i;l=j*m*k;i = (j * k)^m ;
		l = (k/m * jj) % i;m = m^l;m = m^l;i = (j * k)^m ;
		k = (k/m * jj) % i; m=k*k*k*k*k - m/i;
		usleep(rest_time);
		//printf("rest time: %d\n", rest_time);
	}
    return 0;
}

bool Snake::get_cpu_info(c_status &occ) {
	FILE *fp;
	char buff[256];
	fp = fopen("/proc/stat", "r");
	if (fp == NULL) {
		perror("Error opening /proc/stat \n");
		return false;
	}
	else {
		if(fgets(buff, 256, fp) == NULL) {
			perror ("Error reading the /proc/stat \n");
			return false;
		}
		fclose(fp);
	}
	//printf("%s", buff);
	sscanf(buff, "%s %u %u %u %u", occ.name, &occ.user,
           &occ.nice, &occ.system, &occ.idle);
	return true;
}

float Snake::update_single_cpu_usage() {
	unsigned long t1, t2, s1, s2;
	float updates;
	char buff[20];
	int len;
	
	s1 = prev.user + prev.nice + prev.system;
	s2 = curr.user + curr.nice + curr.system;
	
	t1 = s1 + prev.idle;
	t2 = s2 + curr.idle;
	//printf("curr %d %d %d %d\n", curr.user, curr.nice, curr.system, curr.idle);
	//printf("prev %d %d %d %d\n", prev.user, prev.nice, prev.system, prev.idle);
	if ((t2 - t1) < 1) { return current; }
	else {
		updates = ((float)(s2-s1))/((float)(t2-t1))*100.0;
		c_lock.lock();
		current = updates;
		c_lock.unlock();
		printf("%4.2f\n", updates);
		len = sprintf(buff, "%4.2f\n", updates);
		fwrite(buff, 1, len, fwp);
		fflush(fwp);
		controller();
		return updates;
	}
}

void *Snake::multi_cpu_monitor() {
	while(1) {
		update_multi_cpu_usage();
		usleep(500000);
	}
	return 0;
}

void Snake::controller() {
	int updates = ctrl->update(current);
	//printf("rest for: %d\n\n", updates);
	t_lock.lock();
	rest_time = updates;
	t_lock.unlock();
}

void *Snake::single_cpu_monitor (void) {
	c_status updates;
	while(1) {
		//printf("%f\n", current);
		get_cpu_info(updates);
		if(updates != curr) {
			prev = curr;
			curr = updates;
			update_single_cpu_usage();
		}
		// update every second
		// the /proc/stat doen't update at a very fast speed
		// so updating too frequenting only introding much I/O
		// and leading to incorrect results
		usleep(500000); // update every half second
		
	}
	return 0;
}

