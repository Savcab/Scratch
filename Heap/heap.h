#ifndef HEAP_H
#define HEAP_H
#include <list>
#include <vector>

//THIS IS A MIN HEAP
template<typename T>
class Heap{
    public:
        //default constructor
        Heap();
        //constructor with list
        Heap(const list<T> lst);
        //copy contructor
        Heap(const Heap& other);
        //deconstructor
        ~Heap();
        //assignment operator
        Heap& operator=(const Heap& other);
        //add item to the heap
        void push_back(T& item);
        //get the top element
        T top() const;
        //remove the topmost element
        void pop();
    private:
        vector<T> data;
        int getParent(int x) const;
        int getLChild(int x) const;
        int getRChild(int x) const;
};

#endif