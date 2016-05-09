//
//  Helpers.cpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#include "Helpers.hpp"
#include "Net.hpp"

//int** alloc(int size) {
//    int** array = new int*[size];
//    for(int i=0; i<size; i++) {
//        array[i] = new int[size];
//    }
//    return array;
//}

List<List<int>> alloc(int size) {
    List<List<int>> list = List<List<int>>();
    for (int i=0; i<size; i++) {
        List<int> tempList = List<int>();
        list.append(tempList);
    }
    return list;
}

Net* netForPin(int pin, List<Net*> nets) {
    for (int i = 0; i < nets.count; i++) {
        if(nets[i]->pin() == pin) {
            return nets[i];
        }
    }
    return NULL;
}

bool isCovered(Net* net1, Net* net2) {
    bool fullCover = ((net1->left() < net2->left()  && net1->right() > net2->right()) ||
                      (net1->left() > net1->right() && net1->right() < net2->right()));
    bool left = (net1->left() >= net2->left() && net1->left() <= net2->right());
    bool right = (net1->right() >= net2->left() && net1->right() <= net2->right());
    return left || right || fullCover;
    
}

List<Net*>* prepareNets(int top[], int bottom[], int count, int maxNumber) {
    
    List<Net*>* nets = new List<Net*>();
    
    for(int i=1; i<=maxNumber; i++) {
        List<Pin*> pins = List<Pin*>();
        for(int j=0; j<count; j++) {
            if (top[j] == i) {
                Pin* pin = new Pin(true, i, j);
                pins.append(pin);
            }
            if (bottom[j] == i) {
                Pin* pin = new Pin(false, i, j);
                pins.append(pin);
            }
        }
        Net* net = new Net(pins);
        nets->append(net);
    }
    return nets;
}