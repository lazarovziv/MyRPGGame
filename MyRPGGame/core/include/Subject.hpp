#include <vector>
#include <algorithm>
#include "Observer.hpp"

class Subject {
protected:
    std::vector<Observer *> observers;

public:
    Subject() = default;
    void register_observer(Observer *observer);
    void unregister_observer(Observer *observer);
    void notify_all();
}; 