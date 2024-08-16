#include "../include/Queue.hpp"

Queue::Queue(bool isMin, int size) {
    this->isMin = isMin;
    length = size * 2;
    heapSize = 0;
}


Node *Queue::min() {
    return items.at(0);
}

Node *Queue::extractMin() {
    if (items.size() == 0) return nullptr;
    Node *min = this->min();
    swap(0, items.size() - 1);
    items.pop_back();
    heapify(0);
    return min;
}

void Queue::buildHeap(vector<Node *> nodes) {
    for (auto node : nodes) {
        items.push_back(node);
    }
    for (int i = items.size()/2; i >= 0; i--) heapify(i);
}

void Queue::insert(Node *node) {
    int tempDistance = node->distance;
    items.push_back(node);
    node->distance = INT16_MAX;
    changeValue(node, tempDistance);
}

void Queue::changeValue(Node *node, int value) {
    if (items.size() == 1) {
        items.at(0)->distance = value;
        return;
    }

    int i = 1;
    node->distance = value;

    while (i > 0 && items.at(parentIdx(i)) > items.at(i)) {
        swap(i, parentIdx(i));
        i = parentIdx(i);
    }
}

void Queue::heapify(int idx) {
    int target = idx;
    int left = leftChildIdx(idx);
    int right = rightChildIdx(idx);

    if (left < items.size() && items.at(left) < items.at(target)) target = left;
    if (right < items.size() && items.at(right) < items.at(target)) target = right;

    if (target != idx) {
        swap(idx, target);
        heapify(target);
    }
}

int Queue::parentIdx(int idx) {
    if (idx % 2 == 0) return idx/2 - 1;
    return idx/2;
}

int Queue::leftChildIdx(int idx) {
    return 2 * idx + 1;
}

int Queue::rightChildIdx(int idx) {
    return 2 * idx + 2;
}

void Queue::swap(int firstIdx, int secondIdx) {
    Node *temp = items.at(firstIdx);
    items[firstIdx] = items[secondIdx];
    items[secondIdx] = temp;
}

bool Queue::isEmpty() {
    return items.empty();
}

int Queue::size() {
    return items.size();
}

