#include "../include/MainMenu.hpp"

MainMenu::MainMenu(std::vector<std::string> itemsStrings) : Menu(itemsStrings) {}

MenuActions MainMenu::execute() {
    return (MenuActions) currentMenuItemIdx;
}
