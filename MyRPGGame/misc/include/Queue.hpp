#ifndef MYRPGGAME_QUEUE_HPP
#define MYRPGGAME_QUEUE_HPP

#include <vector>
#include <queue>
//#include "Node.hpp"

class Node;

//using namespace std;

class Queue {
private:
    bool isMin = true;
    int heapSize;
    int length;

    std::vector<Node *> items;

public:
    Queue(bool isMin, int size);

    Node* min();
    Node* extractMin();
    void buildHeap(vector<Node *> nodes);
    void insert(Node *node);
    void changeValue(Node *node, int value);
    void heapify(int idx);
    int parentIdx(int idx);
    int leftChildIdx(int idx);
    int rightChildIdx(int idx);
    void swap(int firstIdx, int secondIdx);

    bool isEmpty();
    int size();
};

#endif //MYRPGGAME_QUEUE_HPP
