#include "../include/MainMenu.hpp"

MainMenu::MainMenu(std::vector<std::string> itemsStrings) : Menu(itemsStrings) {}

MenuActions MainMenu::execute() {
    std::cout << (MenuActions) currentMenuItemIdx << std::endl;
    return (MenuActions) currentMenuItemIdx;
}
