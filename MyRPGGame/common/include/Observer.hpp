#ifndef MovementObserver_hpp
#define MovementObserver_hpp

class Observer {
public:
    Observer() = default;
    ~Observer() = default;
    virtual void notify() = 0;
};

#endif /* MovementObserver_hpp */