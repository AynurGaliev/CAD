//
//  FSSAlgorithm.hpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 09.05.16.
//  Copyright © 2016 F&latStack. All rights reserved.
//

#ifndef FSSAlgorithm_hpp
#define FSSAlgorithm_hpp

#include <stdio.h>
#include "ChannelData.hpp"
#include "Fish.hpp"

struct FSSAlgorithm {
    
    ChannelData* data;
    double individualStepStart;
    double individualStepFinal;
    double collStep;
    double maxWeight;
    int populationSize;
    int iterationsCount;
    List<Fish*> fishes;
    double prevSumWeight;
    double currentSumWeight;
    int syncPeriod;
    Fish* barycenter;
    
    FSSAlgorithm(int top[],
                 int bottom[],
                 int count,
                 int populationSize,
                 int iterationsCount,
                 int syncPeriod,
                 double maxWeight,
                 double collStep,
                 double individualStepStart,
                 double individualStepFinal) {
        
        this->individualStepFinal = individualStepFinal;
        this->individualStepStart = individualStepStart;
        this->collStep = collStep;
        this->populationSize = populationSize;
        this->iterationsCount = iterationsCount;
        this->syncPeriod = syncPeriod;
        int maxNumber = -1;
        for(int i=0; i<count; i++) {
            if(top[i] > maxNumber) {
                maxNumber = top[i];
            }
            if(bottom[i] > maxNumber) {
                maxNumber = bottom[i];
            }
        }
        ChannelData data = ChannelData(top, bottom, count, maxNumber);
        for(int i=0; i<populationSize; i++) {
            Fish fish = Fish(&data, iterationsCount, maxWeight/2.0, individualStepStart, collStep);
            fishes.append(&fish);
        }
    }
    
    void start() {
        
        this->fishes.forEach([this](Fish* fish) {
            this->prevSumWeight += fish->fitnessValue;
            return;
        });
        this->currentSumWeight = prevSumWeight;
        
        for(int i=1; i<=this->iterationsCount; i++) {
            
            if(i%this->syncPeriod == 0) {
                
            } else {
                
                //1. Individual swim
                this->fishes.forEach([](Fish* fish) {
                    fish->individualSwim();
                });
                
                //2. Search for best fish
                Fish** bestFish = this->fishes.min([](Fish* fish1, Fish* fish2) {
                    return (*fish1) > (*fish2);
                });
                double bestFF = (*bestFish)->fitnessValue;
                
                //3. Feed operator
                fishes.forEach([bestFF](Fish* fish) {
                    fish->feed(bestFF);
                });
                
                //4. Instinctive swim
                List<Fish*> fishesWithPositiveDelta = this->fishes.findAll([](Fish* fish) {
                    return fish->prevFitnessValue - fish->fitnessValue > 0;
                });
                List<Fish*> sortedFishes = fishesWithPositiveDelta.sorted([](Fish* fish1, Fish* fish2) {
                    return fish1 > fish2;
                });
                double sumFitnessValue = 0.0;
                this->fishes.forEach([&sumFitnessValue](Fish* fish) {
                    sumFitnessValue += fish->fitnessValue;
                });
                for(int j=0; j<sortedFishes.count; j++) {
                    double probability = this->fishes[j]->fitnessValue/sumFitnessValue;
                    for(int k=0; k<this->fishes.count; k++) {
                        this->fishes[k]->move(sortedFishes[j], true, probability);
                    }
                }
                
                //5. Search for barycenter
                List<Fish*> sortedFishesInFFOrder = this->fishes.sorted([](Fish* fish1, Fish* fish2) {
                    return fish1 > fish2;
                });
                this->barycenter = sortedFishesInFFOrder[this->fishes.count - 1];
                this->fishes.forEach([this](Fish* fish) {
                    fish->move(this->barycenter, true, 0.5);
                });
                
                //6. Sum all fish weights
                this->prevSumWeight = this->currentSumWeight;
                
                sumFitnessValue = 0.0;
                this->fishes.forEach([&sumFitnessValue](Fish* fish) {
                    sumFitnessValue += fish->fitnessValue;
                });
                this->currentSumWeight = sumFitnessValue;
                
                //7. Collective swim
                int maxDistance = 0;
                this->fishes.forEach([this, &maxDistance](Fish* fish) {
                    int distance = fish->distance(this->barycenter);
                    if(maxDistance < distance) {
                        maxDistance = distance;
                    }
                });
                this->fishes.forEach([this, maxDistance](Fish* fish) {
                    if(maxDistance != 0) {
                        bool value = this->currentSumWeight >= this->prevSumWeight;
                        fish->move(this->barycenter, value, collStep*(fish->distance(this->barycenter)/maxDistance));
                    }
                });
                
                //8. Update individual params
                this->fishes.forEach([i, this](Fish* fish) {
                    fish->updateIndividualStep(i/this->iterationsCount, this->individualStepStart, this->individualStepFinal);
                });
            }
            
        }
        
    }
};

#endif /* FSSAlgorithm_hpp */
