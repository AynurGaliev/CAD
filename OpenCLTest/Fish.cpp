//
//  Fish.cpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#include "Fish.hpp"
#include <cstring>
#include <cstdlib>
#include <array>
#include <exception>
#include "Helpers.hpp"

using namespace std;

bool haveValue(int* array, int value) {
    int size = sizeof(array)/sizeof(int);
    for(int i=0; i<size; i++) {
        if(array[i] == value) return true;
    }
    return false;
}

int arraySize(int *array) {
    return sizeof(array)/sizeof(int);
}

Fish::Fish(ChannelData* data,
     int iterationCount,
     double weight,
     double individualStep,
     double collStep){
    this->data = data;
    this->weight = weight;
    this->collStep = collStep;
    this->individualStep = individualStep;
    this->formDecisionMatrix();
}

double Fish::calculateFitnessValue(List<List<int>> decision) {
    
    int netsCount = this->data->nets.count;
    int magistralNumber = 0;
    for (int i=0; i<netsCount; i++) {
        this->data->nets[i]->magistralNumber = 0;
    }
    Net* netsOnCurrentMagistral = new Net[netsCount];
    int netsOnMagistralPointer = -1;
    
    List<List<int>> tempEVCG = alloc(netsCount);
    memcpy(&tempEVCG, &this->data->EVCG, sizeof(this->data->EVCG));
    
    int* placedPins = new int[this->data->pinCount];
    int placedPinsPointer = -1;
    
    while(placedPinsPointer != netsCount) {
        magistralNumber++;
        netsOnMagistralPointer = -1;
        int count = sizeof(decision[0])/sizeof(int);
        int decisionSize = sizeof(decision)/sizeof(int);
        for(int i=0; i<count; i++) {
            for(int j=0; j<decisionSize; j++) {
                Net* net = netForPin(decision[j][i], this->data->nets);
                if (net != NULL && !haveValue(placedPins, decision[j][i]) && tempEVCG[decision[j][i]-1].count == 0) {
                    bool canPlace = true;
                    for(int k=0; k<netsOnMagistralPointer; k++) {
                        canPlace &= !isCovered(net, &netsOnCurrentMagistral[k]);
                    }
                    if (canPlace) {
                        placedPins[++placedPinsPointer] = decision[j][i];
                        net->magistralNumber = magistralNumber;
                        netsOnCurrentMagistral[++netsOnMagistralPointer] = *net;
                    }
                }
            }
        }
        for(int k=0; k<placedPinsPointer; k++) {
            for(int l=0; l<tempEVCG.count; l++) {
                for(int m=0; m<tempEVCG.count; m++) {
                    tempEVCG[l][m] = -1;
                }
            }
        }
    }
    
    int maxMagistralNumber = -1;
    for(int i=0; i<netsCount; i++) {
        if (this->data->nets[i]->magistralNumber > maxMagistralNumber) {
            maxMagistralNumber = this->data->nets[i]->magistralNumber;
        }
    }
    double fitnessValue = 0.0;
    for(int i=0; i<netsCount; i++) {
        fitnessValue += this->data->nets[i]->fitnessValueForMagistral(maxMagistralNumber);
    }
    return fitnessValue;
}

void Fish::formDecisionMatrix() {
    
    int netsCount = this->data->nets.count;
    List<List<int>> tempEVCG = alloc(netsCount);
    memcpy(&tempEVCG, &this->data->EVCG, sizeof(this->data->EVCG));
    
    List<int> pinNumbers = List<int>(netsCount);
    for(int i=0; i<netsCount; i++) {
        pinNumbers[i] = i+1;
    }
    int pinNumbersPointer = netsCount + 1;
    while(pinNumbersPointer != - 1) {
        
        for(int i=0; i<netsCount; i++) {
            for(int j=0; j<i+1; j++) {
                Net* net1 = netForPin(pinNumbers[i], this->data->nets);
                Net* net2 = netForPin(pinNumbers[j], this->data->nets);
                if (net1->left() > net2->left()) {
                    int tempValue = pinNumbers[i];
                    pinNumbers[i] = pinNumbers[j];
                    pinNumbers[j] = tempValue;
                }
            }
        }

        List<int> column = List<int>();
        
        for(int i=0; i<netsCount; i++) {
            Net* net = netForPin(pinNumbers[i], this->data->nets);
            bool isCoveredValue = true;
            for(int j=0; j<column.count; j++) {
                if(!isCovered(net, netForPin(column[j], this->data->nets))) {
                    isCoveredValue = false;
                }
            }
            if (isCoveredValue) {
                column.append(pinNumbers[i]);
            }
        }
        
        for(int k=0; k<column.count; k++) {
            if(haveValue(pinNumbers, column[k])) {
                
                int index = -1;
                for(int l=0; l<netsCount; l++) {
                    if(pinNumbers[l] == column[k]) {
                        index = l;
                        break;
                    }
                }
                if (index != -1) {
                    for(int m = index +1; m<netsCount; m++) {
                        pinNumbers[m-1] = pinNumbers[m];
                    }
                    int pinNumbersCount = sizeof(pinNumbers)/sizeof(int);
                    pinNumbers = (int*)realloc(pinNumbers, (pinNumbersCount-1)*sizeof(int));
                }
            }
        }
        this->decision.append(column);
    }
    
    int maxCount = -1;
    for(int i=0; i<decision.count; i++) {
        int size = sizeof(this->decision[i])/sizeof(int);
        if(size > maxCount) {
            maxCount = size;
        }
    }
    maxCount +=0; // extra zeros
    
    for(int i=0; i<decision.count; i++) {
        int length = maxCount - decision.count;
        for(int j=0; j<length; j++) {
            int index = rand()%decision.count;
            for(int k = 0; k<length; k++) {
                this->decision[i].insert(index, 0);
            }
        }
    }
}


void Fish::updateIndividualStep(double progress, double individualStepStart, double individualStepFinal) throw(ArgumentException) {
    if (progress >=0 && progress <=1) {
        this->individualStep = individualStepStart + (individualStepFinal - individualStepStart)*progress;
    } else {
        char error[] = "Progress can't be less 0 or more 1";
        throw new ArgumentException(error);
    }
}

void Fish::individualSwim() {
    
    int netsCount = this->data->nets.count;
    List<List<int>> tempDecision= alloc(netsCount);
    memcpy(&tempDecision, &this->decision, sizeof(this->data->EVCG));
    
    for(int i=0; i<tempDecision.count; i++) {
        for(int j=0; j<tempDecision[i].count; j+=2) {
            double randValue = rand()%100/100.0;
            if (randValue < this->individualStep) {
                if (j<tempDecision[i].count && j+1 < tempDecision[i].count) {
                    int temp = tempDecision[i][j];
                    tempDecision[i][j] = tempDecision[i][j+1];
                    tempDecision[i][j+1] = temp;
                }
            }
        }
    }
    
    for(int i=0; i<decision.count; i++) {
        for(int j=1; j<decision[i].count; j+=2) {
            double randValue = rand()%100/100.0;
            if (randValue < this->individualStep) {
                if (j<tempDecision[i].count && j+1 < tempDecision[i].count) {
                    int temp = tempDecision[i][j];
                    tempDecision[i][j] = tempDecision[i][j+1];
                    tempDecision[i][j+1] = temp;
                }
            }
        }
    }
}

void Fish::feed(double bestFitnessValue) {
    weight +=(prevFitnessValue - fitnessValue)/bestFitnessValue;
}

void Fish::searchBarycenter(List<Fish*> fishes) {
    
    List<Fish*> sortedArray = fishes.sorted([](Fish* value1, Fish* value2) {
        return value1->fitnessValue > value2->fitnessValue;
    });
    Fish* fish = sortedArray[0];
    for(int i = fishes.count - 1; i>=0; i--) {
        fish->move(fishes[i], true, 0.5);
    }
    fish->fitnessValue = calculateFitnessValue(fish->decision);
}

void Fish::move(Fish* etalonFish, bool isIntensification, double probability) {
    
    for(int i=0; i<etalonFish->decision.count; i++) {
        
        for(int j=0; j<etalonFish->decision[i].count-1; j+=2) {
            
            List<int> indexes = List<int>();
            for(int k=0; k<etalonFish->decision[i].count; k++) {
                if(etalonFish->decision[i][k] == this->decision[i][j]) {
                    indexes.append(k);
                }
            }
            int indexOfFirstElement = indexes[rand()%indexes.count];
            indexes.clear();
            for(int k=0; k<etalonFish->decision[i].count; k++) {
                if(etalonFish->decision[i][k] == this->decision[i][j+1]) {
                    indexes.append(k);
                }
            }
            int indexOfLastElement = indexes[rand()%indexes.count];
            double randValue = rand()%100/100.0;
            
            if(isIntensification) {
                if(randValue <= probability && indexOfFirstElement > indexOfLastElement) {
                    this->decision.swap(i, j);
                }
            } else {
                if(randValue <= probability && indexOfFirstElement < indexOfLastElement) {
                    this->decision.swap(i, j);
                }
            }
         }
        
        for(int j=1; j<etalonFish->decision[i].count-1; j+=2) {
            
            List<int> indexes = List<int>();
            for(int k=0; k<etalonFish->decision[i].count; k++) {
                if(etalonFish->decision[i][k] == this->decision[i][j]) {
                    indexes.append(k);
                }
            }
            int indexOfFirstElement = indexes[rand()%indexes.count];
            indexes.clear();
            for(int k=0; k<etalonFish->decision[i].count; k++) {
                if(etalonFish->decision[i][k] == this->decision[i][j+1]) {
                    indexes.append(k);
                }
            }
            int indexOfLastElement = indexes[rand()%indexes.count];
            double randValue = rand()%100/100.0;
            
            if(isIntensification) {
                if(randValue <= probability && indexOfFirstElement > indexOfLastElement) {
                    this->decision.swap(i, j);
                }
            } else {
                if(randValue <= probability && indexOfFirstElement < indexOfLastElement) {
                    this->decision.swap(i, j);
                }
            }
        }
    }
}

int Fish::distance(Fish* etalonFish) {
    int distance = 0.0;
    for(int i=0; i<this->decision.count; i++) {
        List<int> indexes = this->decision[i];
        List<int> positiveIndexes = indexes.findAll([](int index) {
            return index > 0;
        });
        for(int j=0; j<positiveIndexes.count; j++) {
            for(int k=j+1; k<positiveIndexes.count; k++) {
                int firstIndexInThisColumn = this->decision[i].findIndex([&positiveIndexes, j](int index) {
                    return index == positiveIndexes[j];
                });
                int secondIndexInThisColumn = this->decision[i].findIndex([&positiveIndexes, k](int index) {
                    return index == positiveIndexes[k];
                });
                int firstIndexInEtalonColumn = etalonFish->decision[i].findIndex([&positiveIndexes, j](int index) {
                    return index == positiveIndexes[j];
                });
                int secondIndexInEtalonColumn = etalonFish->decision[i].findIndex([&positiveIndexes, k](int index) {
                    return index == positiveIndexes[k];
                });
                if (!(((firstIndexInThisColumn <= secondIndexInThisColumn && firstIndexInEtalonColumn <= secondIndexInEtalonColumn)
                       || (firstIndexInThisColumn >= secondIndexInThisColumn && firstIndexInEtalonColumn >= secondIndexInEtalonColumn))))
                {
                    distance++;
                }
            }
        }
    }
    return distance;
}




































