#include "../include/MovementSubject.hpp"
#include <algorithm>

void MovementSubject::notifyAll() {
    for (auto itr = observers.begin(); itr != observers.end(); itr++) (*itr)->update();
} 

void MovementSubject::register(std::shared_ptr<MovementObserver> observer) {
    if (*observer != NULL) {
        observers.push_back(observer);
    }
}

void MovementSubject::unregister(std::shared_ptr<MovementSubject> observer) {
    if (std::find(observers.begin(), observers.end(), observer) != observers.end()) {
        observers.erase(observer);
    }
}  