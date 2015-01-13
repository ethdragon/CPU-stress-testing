//
//  KFilter.cpp
//  VM
//
//  Created by Yang LIU, Ruimin Sun, Yuemin Li on 4/16/14.
//  Copyright (c) 2014 Yang LIU, Ruimin Sun, Yuemin Li. All rights reserved.
//

#include "KFilter.h"

KFilter::KFilter() {
    X = 0.5;   //state of t-1   example: 0;
    P = 0.1;   //error of t-1   example: 1;
    Q = 1;     //system noise
    R = 0.1;     //measurement noise
    C = 3;   //output parameter, should be negative
    target = 80;
}

void KFilter::set_target(int t){
    target = t;
    if (t==60){
        Q = 2;
	R = 0.5;
	C =5;
    }
}

int KFilter::update(float usage) {
    float K, e;
	// K for kalman gain, e for feedback error, sleep for output sleep;
	P = P + Q;
    K = P * C /(C*P*C + R);
	e = usage - C *X;
    P = P - K*C*P;
	X = X + K*e;
	return (int) (C*X>0? C*X: 1);
}
