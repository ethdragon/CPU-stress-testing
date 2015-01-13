//
//  simple_controller.cpp
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#include <stdio.h>
#include "simple_controller.h"

Simple_controller::Simple_controller() {
	//last_usage = 0;
	rest_time = 80;
}

int Simple_controller::update(float usage) {
	//printf("usage - last_usage = %f - %d\n", usage, target);
	int gap = (int)(usage) - target;
	if(gap > 5) { rest_time += 2; }
	else if(gap < -5) { rest_time -= 2; }
	return rest_time>0? rest_time: 1;
}

void Simple_controller::set_target(int t) {
	target = t;
}