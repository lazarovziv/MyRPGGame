#include "../include/MenuRepository.hpp"

void MenuRepository::setMenu(Menu *menu) {
    menu->setIsActive(false);
    this->menu = menu;
}

int MenuRepository::execute(Constants::GameState *state) {
    return menu->execute(state);
}

void MenuRepository::moveUp() {
    menu->moveUp();
}

void MenuRepository::moveDown() {
    menu->moveDown();
}

void MenuRepository::resetMenuItemIdx() {
    menu->resetMenuItemIdx();
}

bool MenuRepository::menuHasSubMenus() {
    return menu->hasSubMenus();
}

void MenuRepository::updateSubMenu(Menu *currentMenu, Constants::GameState *state) {
    setMenu(menu->getSubMenus()->at(menu->execute(state)));
    currentMenu = this->menu;
}

bool MenuRepository::choseSubMenuItem() {
    return menu->isSubMenuItem();
}

bool MenuRepository::isGameMenu() {
    return menu->isGameMenu();
}
