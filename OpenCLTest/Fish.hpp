//
//  Fish.hpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#ifndef Fish_hpp
#define Fish_hpp

#include <stdio.h>
#include "ChannelData.hpp"

class ArgumentException: public exception {
    
public:
    
    ArgumentException(char* message) {
        this->message = message;
    }
    
    char* description() {
        return this->message;
    }
    
private:
    char* message;
    
};

struct Fish {
    ChannelData* data;
    double individualStep;
    double collStep;
    double currentSumWeight;
    double prevSumWeight;
    List<List<int>> decision;
    double weight;
    double prevFitnessValue;
    double fitnessValue;
    
    Fish(ChannelData* data,
         int iterationCount,
         double weight,
         double individualStep,
         double collStep){
        this->data = data;
        this->weight = weight;
        this->collStep = collStep;
        this->individualStep = individualStep;
    }
    
    bool operator>(const Fish& rhs) {
        return this->fitnessValue > rhs.fitnessValue;
    }
    
    bool operator<(const Fish& rhs) {
        return this->fitnessValue < rhs.fitnessValue;
    }
    
    bool operator>=(const Fish& rhs) {
        return this->fitnessValue >= rhs.fitnessValue;
    }
    
    bool operator<=(const Fish& rhs) {
        return this->fitnessValue <= rhs.fitnessValue;
    }
    
    void individualSwim();
    double calculateFitnessValue(List<List<int>>);
    void formDecisionMatrix();
    void updateIndividualStep(double progress, double individualStepStart, double individualStepFinal) throw(ArgumentException);
    void feed(double);
    void searchBarycenter(List<Fish*> fishes);
    void move(Fish* etalonFish, bool isIntensification, double probability = 1.0);
    int distance(Fish*);
};

#endif /* Fish_hpp */