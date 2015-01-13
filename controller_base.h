//
//  controller_bash.h
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#ifndef VM_controller_bash_h
#define VM_controller_bash_h

class ControllerBase {
public:
	virtual int update (float) = 0;
    virtual void set_target(int) = 0;
    
protected:
    int target;
};

#endif
