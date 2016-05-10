//
//  ChannelData.hpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#ifndef ChannelData_hpp
#define ChannelData_hpp

#include <stdio.h>
#include "Net.hpp"
#include "Pin.hpp"

struct ChannelData {
    List<Net*> nets;
    List<Pin*> topPins;
    List<Pin*> bottomPins;
    int pinCount;
    List<List<int>> VCG;
    List<List<int>> EVCG;
    
    void setupEVCG(List<Net*> nets);
    void setupVCG(int count);
    bool checkWay(int start, int end, List<List<int>> &graph, int count);
    
    ChannelData(List<Net*> nets, List<Pin*> topPins, List<Pin*> bottomPins);
    ChannelData(int top[], int bottom[], int count, int maxNumber);
    
    void description() {
        printf("----CHANNEL DATA-----/n");
        printf("----NETS----\n");
        for(int i=0; i<nets.count; i++) {
            this->nets[i]->description();
        }
        printf("----TOP PINS-----");
        for(int i=0; i<topPins.count; i++) {
            topPins[i]->description();
        }
        printf("----BOTTOM PINS-----");
        for(int i=0; i<bottomPins.count; i++) {
            bottomPins[i]->description();
        }
        printf("Pin count - %d\n", pinCount);
        printf("------VCG------");
        for(int i=0; i<VCG.count; i++) {
            for(int j=0; j<VCG[i].count; j++) {
                printf("%d", VCG[i][j]);
            }
            printf("\n");
        }
        printf("------EVCG------");
        for(int i=0; i<EVCG.count; i++) {
            for(int j=0; j<EVCG[i].count; j++) {
                printf("%d", EVCG[i][j]);
            }
            printf("\n");
        }
    }
};

#endif /* ChannelData_hpp */