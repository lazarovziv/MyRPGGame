#include <memory>
#include <vector>
#include "MovementObserver.hpp"


class MovementSubject {
private:
    std::vector<shared_ptr<MovementObserver>> observers;
    
    void notifyAll();

public:
    void register(std::shared_ptr<MovementObserver> observer);
     void unregister(std::shared_ptr<MovementObserver> observer);
     
}; 