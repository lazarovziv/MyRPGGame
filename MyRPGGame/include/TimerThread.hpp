#ifndef MYRPGGAME_TIMERTHREAD_HPP
#define MYRPGGAME_TIMERTHREAD_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
struct threadArguments{
    sf::Event event;
    sf::RenderWindow *window;
};
class TimerThread{
private:

  static int keyListener(sf::Event *event,sf::RenderWindow *window);


public:
    static void *start(void *arg);
};

#endif //MYRPGGAME_TIMERTHREAD_HPP
