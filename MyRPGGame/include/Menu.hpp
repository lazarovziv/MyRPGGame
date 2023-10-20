//#pragma once

#ifndef MYRPGGAME_MENU_HPP
#define MYRPGGAME_MENU_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <map>
#include <vector>

//using namespace std;

enum MenuActions {
    ACTION_RESUME, ACTIONS_INVENTORY, ACTION_SETTINGS, ACTION_EXIT, ACTION_SUBMENU
};

class Menu {
protected:
    std::vector<sf::Text> *menuItems;
    sf::Font font;
    int currentMenuItemIdx;
    int numOfMenuItems;
    bool active; // indicator whether the menu is currently shown on screen (if switched off we'll go to parent menu)
    bool gameMenu = false;
    Menu *parentMenu; // for navigating backwards from the submenus
    std::vector<Menu*> *subMenus = nullptr; // setting an option for every menu to have a sub menu
    std::map<int, Menu*> *subMenusIndexesMap = nullptr;

    void setParentMenu(Menu *menu);

public:
    explicit Menu(std::vector<std::string> itemsStrings, bool gameMenu);
    ~Menu() = default;

    void initMenuItems(std::vector<std::string> itemsStrings);
    std::vector<sf::Text> *getMenuItems();
    std::vector<Menu*> *getSubMenus();
    void render(real currentWindowX, real currentWindowY, sf::RenderWindow *window);
    void moveUp();
    void moveDown();
    bool hasSubMenus();
    void addSubMenu(Menu *menu, int menuIndex);
    Menu *getParentMenu();
    void setIsActive(bool flag);
    bool isActive();
    bool isGameMenu();
    virtual bool isSubMenuItem();
    virtual int execute(Constants::GameState *state);
    void resetMenuItemIdx();
};

#endif //MYRPGGAME_MENU_HPP
