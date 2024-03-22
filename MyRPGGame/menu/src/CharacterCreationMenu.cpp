#include "../include/CharacterCreationMenu.hpp"

CharacterCreationMenu::CharacterCreationMenu(std::vector<std::string> itemsStrings) : Menu(itemsStrings) {
    for (auto &item : itemsStrings) {
        mainItemsStrings.push_back(item);
    }
    // init body items strings
    bodyItemsStrings.push_back("Light");
    bodyItemsStrings.push_back("Dark");
    bodyItemsStrings.push_back("Skeleton");
    bodyItemsStrings.push_back("Tanned");
    bodyItemsStrings.push_back("Orc");
    // init torso items strings (TODO: create sub menu for all choices)
    torsoItemsStrings.push_back("Leather");
    torsoItemsStrings.push_back("Long Sleeve");
    torsoItemsStrings.push_back("Short Sleeve");
    // init pants items strings
    pantsItemsStrings.push_back("Magenta");
    pantsItemsStrings.push_back("Red");
    pantsItemsStrings.push_back("Teal");
    pantsItemsStrings.push_back("White");
}

void CharacterCreationMenu::updateBodyMenuItems() {
    menuItems.clear(); // resetting the menu items for setting new ones
    initMenuItems(bodyItemsStrings);
}

void CharacterCreationMenu::updatePantsMenuItems() {
    menuItems.clear(); // resetting the menu items for setting new ones
    initMenuItems(pantsItemsStrings);
}

void CharacterCreationMenu::updateTorsoMenuItems() {
    menuItems.clear(); // resetting the menu items for setting new ones
    initMenuItems(torsoItemsStrings);
}

MenuActions CharacterCreationMenu::execute() {
    return (MenuActions) currentMenuItemIdx;
}
