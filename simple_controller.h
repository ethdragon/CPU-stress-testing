//
//  dummy_controller.h
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#ifndef VM_dummy_controller_h
#define VM_dummy_controller_h

#include "controller_base.h"

class Simple_controller: public ControllerBase {
public:
	Simple_controller();
	virtual int update(float usage);
	virtual void set_target(int);

private:
	//float last_usage;
	int rest_time;
};

#endif