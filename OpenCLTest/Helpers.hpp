//
//  Helpers.hpp
//  OpenCLTest
//
//  Created by Galiev Aynur on 08.05.16.
//  Copyright © 2016 FlatStack. All rights reserved.
//

#ifndef Helpers_hpp
#define Helpers_hpp

#include <cstdlib>
#include <cstring>
#include <functional>

using namespace std;
class Net;

template <class T>
struct List {
    T* array;
    int count;
    
    void resize(int newSize) {
        count = newSize;
        if (array != NULL) {
            array = (T*)realloc(array, sizeof(T)*newSize);
        } else {
            array = new T[newSize];
        }
    }
    
    void insert(int index, T value) {
        resize(count+1);
        for(int i=count-1; i>index; i--) {
            array[i] = array[i-1];
        }
        array[index] = value;
    }
    
    void removeAt(int index) {
        for(int i=index; i<count-1; i++) {
            array[index] = array[index+1];
        }
        resize(count-1);
    }
    
    void remove(T value) {
        for(int i=0; i<count; i++) {
            if (array[i] == value) {
                removeAt(i);
            }
        }
    }
    
    void append(T value) {
        resize(count+1);
        array[count-1] = value;
    }
    
    T& operator[] (const int index) {
        return array[index];
    }
    
    List<T>& operator=(const List<T>& rhs) {
        count = rhs.count;
        array = new T[count];
        memcpy(array, rhs.array, count*sizeof(T));
        return *this;
    }

    List<T> sorted(function<bool (T, T)>  func) {
        T* tempArray = new T[count];
        memcpy(tempArray, array, sizeof(T)*count);
        for(int i=0; i<count; i++) {
            for(int j=i+1; j<count; j++) {
                if(func(tempArray[i], tempArray[j])) {
                    swap(i, j);
                }
            }
        }
        return List<T>(tempArray, count);
    }
    
    void forEach(function<void (T)> func) {
        for(int i=0; i<count; i++) {
            func(array[i]);
        }
    }
    
    int findIndex(function<bool (T)> func) {
        for(int i=0; i<count; i++) {
            if (func(array[i])) {
                return i;
            }
        }
        return -1;
    }
    
    T* min(function<bool (T, T)> func) {
        if(count == 0) {
            return NULL;
        }
        T* best = &array[0];
        for(int i=1; i<count; i++) {
            if(func(*best, array[i])) {
                best = &array[i];
            }
        }
        return best;
    }
    
    T* max(function<bool (T, T)> func) {
        if(count == 0) {
            return NULL;
        }
        T best = array[0];
        for(int i=1; i<count; i++) {
            if(func(best, array[i])) {
                best = array[i];
            }
        }
        return best;
    }
    
    List<T> findAll(function<bool (T)> func) {
        List<T> list = List<T>();
        for(int i=0; i<count; i++) {
            if(func(array[i])) {
                list.append(array[i]);
            }
        }
        return list;
    }
    
    void clear() {
        delete [] array;
        array = NULL;
        count = 0;
    }
    
    void swap(int index1, int index2) {
        T tempValue = array[index1];
        array[index1] = array[index2];
        array[index2] = tempValue;
    }
    
    List(T* copiedArray, int size) {
        array = new T[size];
        memcpy(array, copiedArray, size*sizeof(T));
    }
    
    List(List<T> const &copiedList) {
        count = copiedList.count;
        array = new T[count];
    }
    
    List() {
        count = 0;
        array = NULL;
    }
};

List<Net*>* prepareNets(int* top, int* bottom, int count, int maxNumber);
List<List<int>> alloc(int size);
Net* netForPin(int pin, List<Net*> nets);
bool isCovered(Net* net1, Net* net2);

#endif /* Helpers_hpp */