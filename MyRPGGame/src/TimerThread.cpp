#include "../include/TimerThread.hpp"

 int TimerThread::keyListener(sf::Event *event,sf::RenderWindow *window){
    while(window->pollEvent(*event)){
        switch(event->type){
            case sf::Event::Closed:
                window->close();
                break;
            case sf::Event::KeyPressed:
                std::cout << event->key.code << std::endl;
                return event->key.code;
            default:
                break;
        }
    }
    return -1;
}

void *TimerThread::start(void *arg) {
    struct threadArguments *arguments = (threadArguments*) arg;
    keyListener(&(arguments->event),arguments->window);
    return nullptr;
}