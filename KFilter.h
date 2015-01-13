//
//  KFilter.h
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#ifndef VM_KFilter_h
#define VM_KFilter_h

#include "controller_base.h"

class KFilter: public ControllerBase {
private:
    float X;   //state of t-1   example: 0;
    float P;   //error of t-1   example: 1;
    float Q;     //system noise
    float R;     //measurement noise
    float C;   //output parameter, should be negative
    
public:
    KFilter();
    virtual void set_target(int t);
    virtual int update(float usage);
};

#endif
