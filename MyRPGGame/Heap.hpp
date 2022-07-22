#ifndef Heap_hpp
#define Heap_hpp

#include <stdio.h>
//#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class Heap {
private:
    vector<FloatRect> array;
    int length;
    int heapSize;
    
public:
    Heap(int length);
    ~Heap() = default;
    int parent(int i);
    int left(int i);
    int right(int i);
    void build();
    void heapify(int i);
    void heapSort();
//    void insert(FloatRect* data);
//    FloatRect remove(int i);
};

#endif /* Heap_hpp */
