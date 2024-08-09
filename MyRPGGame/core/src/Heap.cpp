#include "Heap.hpp"

Heap::Heap(int size) {
    length = size;
}

int Heap::parent(int i) {
    return (int) i/2;
}

int Heap::left(int i) {
    return 2*i;
}

int Heap::right(int i) {
    return 2*i + 1;
}

void Heap::build() {
    heapSize = length;
    for (int i = length/2; i >= 0; i--) {
        heapify(i);
    }
}

// min heapify
void Heap::heapify(int i) {
    int left = this->left(i);
    int right = this->right(i);
    int largest = left;
    if (left <= heapSize && array[left].left < array[i].left) largest = left;
    if (right <= heapSize && array[right].left < array[largest].left) largest = right;
    if (largest != i) {
        // swap
        std::swap(array[largest], array[i]);
        heapify(largest);
    }
}

void Heap::heapSort() {
    this->build();
    for (int i = length - 1; i >= 1; i--) {
        heapSize--;
        heapify(0);
    }
}

//FloatRect Heap::insert(FloatRect* data) {
//    
//}
//
//FloatRect Heap::remove(int i) {
//    
//}
