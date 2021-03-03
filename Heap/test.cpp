#include "heap.h"
#include "heap.cpp"
#include <vector>

using namespace std;
int main(){
    Heap<int> h = Heap<int>();
    vector<int> ex = {9, 8, -1, -500, 90, 0, -59, 0, 0, -1, -1, 500, -59, 8};
    for(vector<int>::iterator it = ex.begin() ; it != ex.end(); it++){
        h.push(*it);
        h.printHeap();
    }

    cout << "Finished building the heap" << endl << endl;
    try{
        while(1){
            //h.printHeap();
            int temp = h.top();
            cout << temp << endl;
            h.pop();
        }
    } catch(exception &e){
        cout << "end of heap reached" << endl;
    }
    





    return 0;
}