#include <time.h>
#include <math.h>
#include "Hashtable.h"


Hashtable::Hashtable(bool debug, unsigned int probing){
    mode = probing;
    if(mode == 3){
        avl = new AVLTree<string, int>();
    } else {
        load_factor = 0;
        avl = nullptr;
        size_index = 0;
        if(debug){
            r[0] = 983132572;
            r[1] = 62337998;
            r[2] = 552714139;
            r[3] = 984953261;
            r[4] = 261934300;
        } else {
            srand(time(NULL));
            for(int i = 0 ; i < 5 ; i++){
                r[i] = rand()%(PRIME_SIZES[size_index]);
            }
        }
        data = new pair<string, int>[PRIME_SIZES[size_index]];
        for(int i = 0 ; i < PRIME_SIZES[size_index] ; i++){
            data[i] = make_pair("", 0);
        }
    }
}

Hashtable::~Hashtable(){
    //AVLTree
    if(mode == 3){
        delete avl;
    } else {
        delete [] data;
    }
}

/**
 * if k is already in the Hashtable, then increment its value. 
 * If it is new, add it to the Hashtable with a value of 1
 * */
void Hashtable::add(string k){
    //edge case
    if(k == ""){ return;}
    //AVLTree mode
    if(mode == 3){
        avl->insert(make_pair(k, count(k)+1));
        return;
    }

    bool newItem = true;
    int pos = findAddIndex(k);
    if(data[pos].first == k){
        newItem = false;
    }
    data[pos] = make_pair(k, data[pos].second+1);

    //adjust the load factor
    if(newItem){
        load_factor += (1.0/PRIME_SIZES[size_index]);
    }
    //resize if needed
    if(load_factor >= 0.5){
        resize();
    }
}

/**
 * For resizing the hashtable after the load factor exceeds 0.5
 * It just increases the table size to a prime that is roughly 2x its previous size and rehashes all the elements
 * */
void Hashtable::resize(){
    //save a copy of loading factor first so it doesn't affect add() when this function calls add()
    double temp = load_factor;
    load_factor = 0;
    size_index++;
    pair<string, int>* old = data;
    data = new pair<string, int>[PRIME_SIZES[size_index]];
    for(int i = 0 ; i < PRIME_SIZES[size_index] ; i++){
        data[i] = make_pair("", 0);
    }
    //rehash all the values
    for(int i = 0 ; i < PRIME_SIZES[size_index-1] ; i++){
        if(old[i].first != ""){
            for(int j = 0 ; j < old[i].second ; j++){
                add(old[i].first);
            }
        }
    }
    delete [] old;

    //adjust the load factor
    load_factor = temp;
    load_factor *= PRIME_SIZES[size_index-1];
    load_factor /= PRIME_SIZES[size_index];
}

/**
 * Returns the int associated with k. Returns 0 is k is not in the table
 * */
int Hashtable::count(string k){
    //AVLTree mode
    if(mode == 3){
        if(avl->find(k) == avl->end()){
            return 0;
        }
        return (*(avl->find(k))).second;
    }

    int pos = findAddIndex(k);
    //either an empty index (which has 0 as second) or the actual count of
    return data[pos].second;
}

/**
 * Private helper function for add and count
 * Finds the index that an item should be added/modified to (either the index of k or the next empty index to add k to)
 * Basically "follows the blob" for probing
 * returns -1 if it is AVLTree
 * */
int Hashtable::findAddIndex(string k) const{
    //AVLTree
    if(mode == 3){
        return -1;
    }
    int curr = hash(k);
    int count = 0;
    int pos  = curr;
    int dh;
    //if double hashing
    if(mode == 2){
        dh = doubleHash(k);
    }
    while(data[pos].first != k && data[pos].first != ""){
        //linear probing
        if(mode == 0){
            pos++;
        //quadratic probing
        } else if(mode == 1){
            count++;
            pos = curr+(count*count);
        //double hashing
        } else if(mode == 2){
            pos += dh;
        }
        pos %= PRIME_SIZES[size_index];
    }
    return pos;
}

/**
 * Prints out all of the elements of the hashtable to the ostream
 * */
void Hashtable::reportAll(ostream& stream) const{
    //is AVLTree
    if(mode == 3){
        for(AVLTree<string, int>::iterator it = avl->begin() ; it!= avl->end() ; ++it){
            stream << it->first << " " << it->second << endl;
        }
    } else {
        for(int i = 0 ; i < PRIME_SIZES[size_index] ; i++){
            if(data[i].first != ""){
                //DEBUGING USE vv
                //stream << i << " ";
                //DEBUGGING USE ^^
                stream << data[i].first << " " << data[i].second << endl;
            }
        }
    }
}

/**
 * Hash helper: takes in a string and breaks it down into an int array of size 5
 * This is used by both the hash functions
 * NOTE: returns dynamically casted memory, MUST delete in the function that uses it
 * */
int* Hashtable::hashHelper(string k) const{
    int* w = new int[5];
    unsigned int index = k.size()-1;
    for(int i = 4 ; i >= 0 ; i--){
        w[i] = 0;
        unsigned int newSize = 6;
        if(k.size() < newSize){
            newSize = k.size();
        }
        for(int j = newSize ; j > 0 ; j--){
            w[i] += (pow(26, newSize - j) * (int)(k[index] - 'a'));
            index--;
        }
        //delete the last few chars from k
        k = k.substr(0, k.size()-newSize);
    }
    return w;
}


/**
 * Hashes a string into an integer that can be used for indexing for the hashtable
 * Longest string that it accepts is 28 letters
 * */
int Hashtable::hash(string k) const{
    //size of the hashtable
    int m  = PRIME_SIZES[size_index];
    //int array of size 5
    int* w = hashHelper(k);
    long long result = 0;
    for(int i = 0 ; i < 5 ; i++){
        //did %m here just so that result wouldn't be too huge
        result += (((long long)w[i]*(long long)r[i]));
    }
    delete [] w;

    result %= m;
    return result;
}

/**
 * The second hash function
 * Uses hashHelper as well, used for double hashing in the Hashtable
 **/
int Hashtable::doubleHash(string k) const{
    //int array of size 5
    int* w = hashHelper(k);
    long long result = 0;
    for(int i = 0 ; i < 5 ; i++){
        result += w[i];
    }
    //prime smaller than size
    int p = PRIME_DOUBLE_HASH[size_index];
    result %= p;
    result = (p - result);
    delete [] w;
    return result;
}