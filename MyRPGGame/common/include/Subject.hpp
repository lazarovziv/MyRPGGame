#include <vector>
#include <algorithm>
#include "Observer.hpp"

class Subject {
protected:
    std::vector<Observer *> observers;

public:
    Subject() = default;
    void registerObserver(Observer *observer);
    void unregisterObserver(Observer *observer);
    void notifyAll();
}; 