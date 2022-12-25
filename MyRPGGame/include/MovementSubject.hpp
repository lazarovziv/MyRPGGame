#include <memory>
#include <vector>
#include "MovementObserver.hpp"


class MovementSubject {
private:
    std::vector<std::shared_ptr<MovementObserver>> observers;

public:
    void registerObserver(std::shared_ptr<MovementObserver> observer);
    void unregisterObserver(std::shared_ptr<MovementObserver> observer);
    void notifyAll();
}; 