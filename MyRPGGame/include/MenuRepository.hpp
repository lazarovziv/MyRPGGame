#ifndef MYRPGGAME_MENUREPOSITORY_HPP
#define MYRPGGAME_MENUREPOSITORY_HPP

#include "Menu.hpp"
#include <SFML/Graphics.hpp>

class MenuRepository {
private:
    Menu *menu;

public:
    MenuRepository() = default;
    ~MenuRepository() = default;

    void moveUp();
    void moveDown();
    void setMenu(Menu *menu);
    void execute(Constants::GameState *state);
    void resetMenuItemIdx();
    bool menuHasSubMenus();
    void updateSubMenu(Menu *currentMenu, Constants::GameState *state);
    bool choseSubMenuItem();
    bool isGameMenu();

};

#endif //MYRPGGAME_MENUREPOSITORY_HPP