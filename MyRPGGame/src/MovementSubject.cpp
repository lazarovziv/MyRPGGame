#include "../include/MovementSubject.hpp"
#include <algorithm>

void MovementSubject::notifyAll() {
    for (auto itr = observers.begin(); itr != observers.end(); itr++) (*itr)->update();
} 

void MovementSubject::registerObserver(std::shared_ptr<MovementObserver> observer) {
    if (observer != NULL) {
        observers.push_back(observer);
    }
}

void MovementSubject::unregisterObserver(std::shared_ptr<MovementObserver> observer) {
    if (std::find(observers.begin(), observers.end(), observer) != observers.end()) {
        observers.erase(observer);
    }
}  