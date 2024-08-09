#include "MainMenu.hpp"

MainMenu::MainMenu(std::vector<std::string> itemsStrings) : Menu(itemsStrings) {}

MenuActions MainMenu::execute() {
    return (MenuActions) current_menu_items_idx;
}
