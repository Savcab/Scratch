#include <vector>

#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

class BloomFilter{
    public:
        BloomFilter();
        ~BloomFilter();

    private:
        vector<int> primes_sizes = {};

}






#endif