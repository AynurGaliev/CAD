//
//  ChannelData.cpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#include "ChannelData.hpp"

List<List<int>> ChannelData::setupEVCG(List<Net*> nets) {
    if (nets.count == 0) {
        return List<List<int>>();
    }
    int netsCount = this->nets.count;
    this->EVCG = alloc(netsCount);
    
    for(int i=0; i<netsCount; i++) {
        for(int j=0; j<netsCount; j++) {
            if (i!=j) {
                if (checkWay(j, i, VCG, netsCount) == true) {
                    EVCG[j][i] = 1;
                } else {
                    EVCG[j][i] = -1;
                }
            }
        }
    }
    return EVCG;
}

ChannelData::ChannelData(List<Net*> nets, List<Pin*> topPins, List<Pin*> bottomPins) {
    this->nets = nets;
    this->topPins = topPins;
    this->bottomPins = bottomPins;
    this->EVCG = setupEVCG(this->nets);
}

ChannelData::ChannelData(int top[], int bottom[], int count, int maxNumber) {
    
    List<Net*>* netsPointer = prepareNets(top, bottom, count, maxNumber);
    memcpy(&this->nets, netsPointer, sizeof(*netsPointer));
    
    this->topPins = List<Pin*>();
    this->bottomPins = List<Pin*>();
    
    for(int i=0; i<maxNumber; i++) {
        Net* net = this->nets[i];
        for(int j=0; j<net->pins.count; j++) {
            if (net->pins[j]->top) {
                this->topPins.append(net->pins[j]);
            } else {
                this->bottomPins.append(net->pins[j]);
            }
        }
    }
}

List<List<int>> ChannelData::setupVCG(int count) {
    this->VCG = alloc(count);
    for (int i=0; i<count; i++) {
        for (int j=0; j<count; j++) {
            VCG[i][j] = 0;
        }
    }
    for(int i=0; i<pinCount; i++) {
        if (this->topPins[i]->number != 0 && bottomPins[i]->number != 0) {
            this->VCG[topPins[i]->number - 1][bottomPins[i]->number - 1] = 1;
        }
    }
    return this->VCG;
}


bool ChannelData::checkWay(int start, int end, List<List<int>> graph, int count) {
    
    bool* flags = new bool[count];
    
    int* queue = new int[count];
    int pointer = -1;
    queue[++pointer] = start;
    flags[start] = true;
    
    while(pointer !=-1) {
        int node = queue[pointer--];
        for(int i=0; i<count; i++) {
            if (graph[node][i] == 1 && !flags[i]) {
                queue[pointer++] = i;
                flags[i] = true;
            }
        }
    }
    return flags[end];
}