#include "../include/Menu.hpp"

Menu::Menu(std::vector<std::string> itemsStrings, bool gameMenu) {
    if (!font.loadFromFile(Constants::GRAPHICS_BASE_PATH + "fonts/arial.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        // TODO: throw error
    }
    menuItems = new std::vector<sf::Text>();
    subMenusIndexesMap = new std::map<int, Menu*>();
    initMenuItems(itemsStrings);
    this->gameMenu = gameMenu;
}

void Menu::initMenuItems(std::vector<std::string> itemsStrings) {
    numOfMenuItems = (int) itemsStrings.size();
    currentMenuItemIdx = 0;

    for (int i = 0; i < numOfMenuItems; i++) {
        sf::Text text;
        text.setString(itemsStrings.at(i));
        text.setFont(font);
        if (i == currentMenuItemIdx) text.setFillColor(sf::Color::Blue);
        else text.setFillColor(sf::Color::White);
        menuItems->push_back(text);
    }
}

std::vector<sf::Text>* Menu::getMenuItems() {
    return menuItems;
}

std::vector<Menu*>* Menu::getSubMenus() {
    return subMenus;
}

void Menu::render(real currentWindowX, real currentWindowY, sf::RenderWindow *window) {
    for (int i = 0; i < numOfMenuItems; i++) {
        // center all items
        menuItems->at(i).setPosition(currentWindowX, currentWindowY + (i-1) * Constants::TILE_SIZE);
        menuItems->at(i).setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
        menuItems->at(i).setStyle(sf::Text::Bold | sf::Text::Italic);
        window->draw(menuItems->at(i));
    }
}

void Menu::moveUp() {
    menuItems->at(currentMenuItemIdx).setFillColor(sf::Color::White);
    // if at the top, go to last
    if (currentMenuItemIdx == 0) currentMenuItemIdx = numOfMenuItems - 1;
    else currentMenuItemIdx--;
    menuItems->at(currentMenuItemIdx).setFillColor(sf::Color::Blue);
}

void Menu::moveDown() {
    menuItems->at(currentMenuItemIdx).setFillColor(sf::Color::White);
    // if at the bottom, go to first
    if (currentMenuItemIdx == numOfMenuItems - 1) currentMenuItemIdx = 0;
    else currentMenuItemIdx++;
    menuItems->at(currentMenuItemIdx).setFillColor(sf::Color::Blue);
}

void Menu::resetMenuItemIdx() {
    menuItems->at(currentMenuItemIdx).setFillColor(sf::Color::White);
    currentMenuItemIdx = 0;
    menuItems->at(currentMenuItemIdx).setFillColor(sf::Color::Blue);
}

bool Menu::hasSubMenus() {
    return subMenus != nullptr;
}

void Menu::addSubMenu(Menu *menu, int menuIndex) {
    if (subMenus == nullptr) {
        subMenus = new std::vector<Menu*>();
    }
    subMenus->push_back(menu);
    (*subMenusIndexesMap)[menuIndex] = menu;
    menu->setParentMenu(this);
}

Menu* Menu::getParentMenu() {
    return parentMenu;
}

void Menu::setIsActive(bool flag) {
    active = flag;
}

bool Menu::isActive() {
    return active;
}

bool Menu::isGameMenu() {
    return gameMenu;
}

void Menu::setParentMenu(Menu *menu) {
    parentMenu = menu;
}

int Menu::execute(Constants::GameState *state) {
    return currentMenuItemIdx;
}

bool Menu::isSubMenuItem() {
    return (*subMenusIndexesMap)[currentMenuItemIdx] != nullptr;
}
