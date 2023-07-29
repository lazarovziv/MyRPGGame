#include "../include/Menu.hpp"

Menu::Menu(std::vector<std::string> itemsStrings) {
    if (!font.loadFromFile("../graphics/fonts/arial.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        // TODO: throw error
    }

    numOfMenuItems = (int) itemsStrings.size();
    currentMenuItemIdx = 0;

    for (int i = 0; i < numOfMenuItems; i++) {
        sf::Text text;
        text.setString(itemsStrings.at(i));
        text.setFont(font);
        if (i == currentMenuItemIdx) text.setFillColor(sf::Color::Blue);
        else text.setFillColor(sf::Color::White);
        menuItems.push_back(text);
    }
}

void Menu::render(int currentWindowX, int currentWindowY, sf::RenderWindow *window) {
    for (int i = 0; i < numOfMenuItems; i++) {
        // center all items
        menuItems[i].setPosition(currentWindowX, currentWindowY + (i-1) * Constants::TILE_SIZE);
        menuItems[i].setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
        menuItems[i].setStyle(sf::Text::Bold | sf::Text::Italic);
        window->draw(menuItems[i]);
    }
}

void Menu::moveUp() {
    menuItems[currentMenuItemIdx].setFillColor(sf::Color::White);
    // if at the top, go to last
    if (currentMenuItemIdx == 0) currentMenuItemIdx = numOfMenuItems - 1;
    else currentMenuItemIdx--;
    menuItems[currentMenuItemIdx].setFillColor(sf::Color::Blue);
}

void Menu::moveDown() {
    menuItems[currentMenuItemIdx].setFillColor(sf::Color::White);
    // if at the bottom, go to first
    if (currentMenuItemIdx == numOfMenuItems - 1) currentMenuItemIdx = 0;
    else currentMenuItemIdx++;
    menuItems[currentMenuItemIdx].setFillColor(sf::Color::Blue);
}

void Menu::resetMenuItemIdx() {
    menuItems[currentMenuItemIdx].setFillColor(sf::Color::White);
    currentMenuItemIdx = 0;
    menuItems[currentMenuItemIdx].setFillColor(sf::Color::Blue);
}

MenuActions Menu::execute() {
    return (MenuActions) currentMenuItemIdx;
}