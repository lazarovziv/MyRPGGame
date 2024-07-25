#include "../include/Subject.hpp"

void Subject::notify_all() {
    for (auto &observer : observers) observer->notify();
}

void Subject::register_observer(Observer *observer) {
    observers.push_back(observer);
}

void Subject::unregister_observer(Observer *observer) {
    // searching for the observer
    auto iterator = std::find(observers.begin(), observers.end(), observer);
    // if not found, do nothing
    if (iterator == observers.end()) return;
    // remove it
    observers.erase(iterator);
}  