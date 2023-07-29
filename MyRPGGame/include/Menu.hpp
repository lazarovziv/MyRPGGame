//#pragma once

#ifndef MYRPGGAME_MENU_HPP
#define MYRPGGAME_MENU_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <vector>

using namespace sf;

enum MenuActions {
    ACTION_RESUME, ACTION_SETTINGS, ACTION_EXIT
};

class Menu {
protected:
    std::vector<sf::Text> menuItems;
    sf::Font font;
    int currentMenuItemIdx;
    int numOfMenuItems;

public:
    explicit Menu(std::vector<std::string> itemsStrings);
    ~Menu() = default;

    void render(int currentWindowX, int currentWindowY, RenderWindow *window);
    void moveUp();
    void moveDown();
    virtual MenuActions execute();
    void resetMenuItemIdx();
};

#endif //MYRPGGAME_MENU_HPP
