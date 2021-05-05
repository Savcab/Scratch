#include "heap.h"
#include <iostream>
#include <list>
#include <exception>
#include <cstring>

/**
 * Customized exception to throw for heap underflow
 * */
class emptyHeadException : public std::exception{
    public:
        emptyHeadException(){
            msg_ = "The heap is empty";
        }
        virtual const char* what() const noexcept{
            return msg_.c_str();
        }
    protected:
        std::string msg_;
};


//Default constructor
template <class T>
Heap<T>::Heap(){
    data = {};
}

//Copy constructor
template <class T>
Heap<T>::Heap(const Heap& other){
    data = other.data;
}

// //constructor with a list ADT
// template <class T>
// Heap<T>::Heap(const list<T>& lst){
//     data = {};
//     int counter = 0;
//     for(list<T>::iterator it = lst.start() ; it != lst.end() ; it++){
//         data.push_back(*it);
//     }
// }

//Assign operator
template <class T>
Heap<T>& Heap<T>::operator=(const Heap& other){
    //check for self assignment
    if(&other == this){ return *this; }
    data = other.data;
}

//Destructor
template <class T>
Heap<T>::~Heap(){
}

template <class T>
int Heap<T>::size(){
    return data.size();
}

/**
 * Helper function: returns the index of the parent function. -1 if none.
 * */
template <class T>
int Heap<T>::getParent(int x) const{
    int ans = (x-1)/2;
    if(ans < 0){
        return -1;
    } else {
        return ans;
    }
}

/**
 * Helper function: returns the index of the left child. -1 if none
 * */
template <class T>
int Heap<T>::getLChild(int x) const{
    int ans = x*2 + 1;
    if(ans >= (int)data.size()){
        return -1;
    } else {
        return ans;
    }
}

/** 
 * Helper function: returns the index of the right child. -1 if none
 * */
template <class T>
int Heap<T>::getRChild(int x) const{
    int ans = x*2 + 2;
    if(ans >= (int)data.size()){
        return -1;
    } else {
        return ans;
    }
}

/**
 * Helper function: returns the index of the min child, -1 if leaf node
 * */
template <class T>
int Heap<T>::findMinChild(int index) const{
    int lc = getLChild(index);
    int rc = getRChild(index);
    int min;
    //only 1 element
    if(lc == -1 && rc == -1) {return -1;}
    //no right children
    if(rc == -1){
        min = lc;
    //both children exist
    } else {
        if(data[lc] < data[rc]){ min = lc; }
        else { min = rc; }
    }
    return min;
}


/**
 * Add new item to the heap and trickle up
 * runtime = log(n)
 * */
template <class T>
void Heap<T>::push(T item){
    data.push_back(item);
    //start the trickle up process
    int index = data.size()-1;
    int parent = getParent(index);
    while(parent != -1 && data[parent] > data[index]){
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
template <class T>
T Heap<T>::top() const{
    //edge case: if heap is empty
    if(data.size() == 0){
        throw emptyHeadException();
    } else {
        return data[0];
    }
}

/**
 * Pop off the top value by swapping it with the last element and trickling down
 * runtime = log(n)
 * */
template <class T>
void Heap<T>::pop(){
    //edge case: if heap is empty
    if(data.size() == 0){
        throw emptyHeadException();
    }
    T temp = data[0];
    data[0] = data[data.size()-1];
    data[data.size()-1] = temp;
    
    data.pop_back();
    //start trickle down
    int index = 0;
    int min = findMinChild(index);
    while(min != -1 && data[index] > data[min]){
        T temp = data[index];
        data[index] = data[min];
        data[min] = temp;
        index = min;
        min = findMinChild(index);
    }
}

/**
 * Prints out the entire data vector
 * */
template <class T>
void Heap<T>::printHeap(){
    for(unsigned int i = 0 ; i < data.size() ; i++){
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}