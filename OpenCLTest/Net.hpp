//
//  N*e*t.hpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#ifndef Net_hpp
#define Net_hpp

#include <stdio.h>
#include "Helpers.hpp"
#include "Pin.hpp"

struct Net {
    
    List<Pin*> pins;
    int magistralNumber;
    
    int left() {
        int leftPosition = -1;
        int pinsCount = sizeof(this->pins)/sizeof(Pin);
        for(int i=0; i<pinsCount; i++) {
            if (leftPosition == -1) {
                leftPosition = pins[i]->xPosition;
            } else {
                if (pins[i]->xPosition < leftPosition) {
                    leftPosition = pins[i]->xPosition;
                }
            }
        }
        return leftPosition;
    }
    
    int right() {
        int rightPosition = -1;
        int pinsCount = sizeof(this->pins)/sizeof(Pin);
        for(int i=0; i<pinsCount; i++) {
            if (rightPosition == -1) {
                rightPosition = pins[i]->xPosition;
            } else {
                if (pins[i]->xPosition > rightPosition) {
                    rightPosition = pins[i]->xPosition;
                }
            }
        }
        return rightPosition;
    }
    
    int pin() {
        return pins[0]->number;
    }
    
    double fitnessValueForMagistral(int magistralsCount) {
        double value = 0.0;
        int pinsCount = sizeof(this->pins)/sizeof(int);
        for(int i=0; i<pinsCount; i++) {
            if(pins[i]->top) {
                value += this->magistralNumber;
            } else {
                value +=magistralsCount - this->magistralNumber + 1;
            }
        }
        return value;
    }
    
    void description() {
        printf("----Net-----/n");
        for(int i=0; i<pins.count; i++) {
            this->pins[i]->description();
        }
        printf("Magistral number - %d\n", magistralNumber);
    }
    
    Net() {
        magistralNumber = -1;
        pins = List<Pin*>();
    }
    
    Net(List<Pin*> &pins) {
        memcpy(&this->pins, &pins, sizeof(pins));
        magistralNumber = -1;
    }
};


#endif /* Net_hpp */