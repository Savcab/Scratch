#include "heap.h"
#include <iostream>

//Default constructor
Heap<T>::Heap(){
    data = {};
}

//Copy constructor
Heap<T>::Heap(const Heap& other){
    data = other.data;
}

//constructor with a list ADT
Heap<T>::Heap(const list<T> lst){
    data = {};
    int counter = 0;
    for(list<T>::iterator it == lst.start() ; it != lst.end() ; it++){
        data.push_back(*it);
    }
}

//Assign operator
Heap<T>& Heap<T>::operator=(const Heap& other){
    //check for self assignment
    if(&other == this){ return; }
    data = other.data;
}

//Destructor
Heap<T>::~Heap(){
}

int Heap<T>::getParent(int x) const{
    int ans = (x-1)/2;
    if(ans < 0){
        return NULL;
    } else {
        return ans;
    }
}

int Heap<T>::getLChild(int x) const{
    int ans = x*2 + 1;
    if(ans >= data.size()){
        return NULL;
    } else {
        return ans;
    }
}

int Heap<T>::getRChild(int x) const{
    int ans = x*2 + 2;
    if(ans >= data.size()){
        return NULL;
    } else {
        return ans;
    }
}


/**
 * Add new item to the heap and trickle up
 * runtime = log(n)
 * */
void Heap<T>::push_back(T& item){
    data.push_back(item);
    //start the trickle up process
    int index = data.size()-1;
    int parent = getParent(index);
    while(data[parent] != NULL && data[parent] > data[index]){
        T temp = data[parent];
        data[parent] = data[index];
        data[index] = temp;
        index = parent;
        parent = getParent(index);
    }
}

/**
 * Get topmost value
 * runtime = const
 **/
T Heap<T>::top() const{
    //edge case: if heap is empty
    if(data.size() == 0){
        return NULL;
    } else {
        return data[0];
    }
}

/**
 * Pop off the top value by swapping it with the last element and trickling down
 * runtime = log(n)
 * */
void Heap<T>::pop(){
    //edge case: if heap is empty
    if(data.size() == 0){ return; }
    T temp = data[0];
    data[0] = data[data.size()-1];
    data[data.size()-1] = temp;
    
    data.pop_back();
    //start trickle down
    int index = 0;
    int min;
    do{
        int lc = getLChild(index);
        int rc = getRChild(index);
        //only 1 element
        if(lc == NULL && rc == NULL) {return;}
        //no right children
        if(rc == NULL){
            min = lc;
        //both children exist
        } else {
            if(data[lc] < data[rc]){ min = lc; }
            else { min = rc; }
        }
        //swap (there's no way that min is NULL now)
        if(data[index] < data[min]){
            T temp = data[index];
            data[index] = data[min];
            data[min] = temp;
            index = min;
        }
    } while(data[index] < data[min]);
}