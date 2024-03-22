#include "../include/Subject.hpp"

void Subject::notifyAll() {
    for (auto &observer : observers) observer->notify();
}

void Subject::registerObserver(Observer *observer) {
    observers.push_back(observer);
}

void Subject::unregisterObserver(Observer *observer) {
    // searching for the observer
    auto iterator = std::find(observers.begin(), observers.end(), observer);
    // if not found, do nothing
    if (iterator == observers.end()) return;
    // remove it
    observers.erase(iterator);
}  