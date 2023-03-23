#include "../include/MovementSubject.hpp"

void MovementSubject::notifyAll() {
    for (auto itr = observers.begin(); itr != observers.end(); itr++) (*itr)->notify();
} 

void MovementSubject::registerObserver(std::shared_ptr<MovementObserver> observer) {
    if (observer != nullptr) {
        observers.push_back(observer);
    }
}

void MovementSubject::unregisterObserver(std::shared_ptr<MovementObserver> observer, int idx) {
    if (std::find(observers.begin(), observers.end(), observer) != observers.end()) {
        observers.erase(observers.begin() + idx); // OR observers.erase(observer);
    }
}  