#include "CharacterCreationMenu.hpp"

CharacterCreationMenu::CharacterCreationMenu(std::vector<std::string> itemsStrings) : Menu(itemsStrings) {
    for (auto &item : itemsStrings) {
        main_items_strings.push_back(item);
    }
    // init body items strings
    body_items_strings.push_back("Light");
    body_items_strings.push_back("Dark");
    body_items_strings.push_back("Skeleton");
    body_items_strings.push_back("Tanned");
    body_items_strings.push_back("Orc");
    // init torso items strings (TODO: create sub menu for all choices)
    torso_items_strings.push_back("Leather");
    torso_items_strings.push_back("Long Sleeve");
    torso_items_strings.push_back("Short Sleeve");
    // init pants items strings
    pants_items_strings.push_back("Magenta");
    pants_items_strings.push_back("Red");
    pants_items_strings.push_back("Teal");
    pants_items_strings.push_back("White");
}

void CharacterCreationMenu::update_body_menu_items() {
    menu_items.clear(); // resetting the menu items for setting new ones
    init_menu_items(body_items_strings);
}

void CharacterCreationMenu::update_pants_menu_items() {
    menu_items.clear(); // resetting the menu items for setting new ones
    init_menu_items(pants_items_strings);
}

void CharacterCreationMenu::update_torso_menu_items() {
    menu_items.clear(); // resetting the menu items for setting new ones
    init_menu_items(torso_items_strings);
}

MenuActions CharacterCreationMenu::execute() {
    return (MenuActions) current_menu_items_idx;
}
