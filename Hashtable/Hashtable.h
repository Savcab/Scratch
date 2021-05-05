#include <string>
#include <iostream>
#include "../AVLTree/avlbst.h"

using namespace std;
class Hashtable{
    public: 
        Hashtable(bool debug = false, unsigned int probing = 0);
        ~Hashtable();
        void add(string k);
        int count(string k);
        //prints out all key value pairs to the ostream
        void reportAll(ostream& stream) const;
    private:
        //load factor
        double load_factor;
        //5 integers as key that will be used for hashing
        int r [5];
        //will store all the possible prime sizes (up to 28 of them)
        const int PRIME_SIZES[28] = {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489, 6584983, 13169977, 26339969, 52679969, 105359969, 210719881, 421439783, 842879579, 1685759167};
        //will store all the complementary prime numbers for the corresponding prime size
        const int PRIME_DOUBLE_HASH[28] = {7, 19, 43, 89, 193, 389, 787, 1583, 3191, 6397, 12841, 25703, 51431, 102871, 205721, 411503, 823051, 1646221, 3292463, 6584957, 13169963, 26339921, 52679927, 105359939, 210719881, 421439749, 842879563, 1685759113};
        int size_index;
        //stores all the data
        pair<string, int>* data;
        //which mode the hashtable is in
        //0: linear probling. 1: quadratic probing. 2: double-hashing. 3: use AVL tree
        unsigned int mode;
        AVLTree<string, int>* avl;

        void resize();
        int hash(string k) const;
        int doubleHash(string k) const;
        //helper function for hash and doublehash
        int* hashHelper(string k) const;
        //helper function for add
        int findAddIndex(string k) const;


};