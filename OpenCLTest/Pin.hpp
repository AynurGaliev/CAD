//
//  Pin.hpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#ifndef Pin_hpp
#define Pin_hpp

#include <stdio.h>

struct Pin {
    bool top;
    int number;
    int xPosition;
    
    Pin(bool top, int number, int xPosition) {
        this->top = top;
        this->number = number;
        this->xPosition = xPosition;
    }
    
    void description() {
        printf("----PIN-----\n");
        printf("Top - %d\n", top);
        printf("Number - %d\n", number);
        printf("xPosition - %d\n", xPosition);
    }
};

#endif /* Pin_hpp */