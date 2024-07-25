#include "../include/Menu.hpp"

Menu::Menu(std::vector<std::string> itemsStrings, bool gameMenu) {
    if (!font.loadFromFile(Constants::GRAPHICS_BASE_PATH + "fonts/arial.ttf")) {
        std::cout << "Failed to load font!" << std::endl;
        // TODO: throw error
    }
    menu_items = new std::vector<sf::Text>();
    sub_menus_indices_map = new std::map<int, Menu*>();
    init_menu_items(itemsStrings);
    this->game_menu = gameMenu;
}

void Menu::init_menu_items(std::vector<std::string> itemsStrings) {
    num_menu_items = (int) itemsStrings.size();
    current_menu_items_idx = 0;

    for (int i = 0; i < num_menu_items; i++) {
        sf::Text text;
        text.setString(itemsStrings.at(i));
        text.setFont(font);
        if (i == current_menu_items_idx) text.setFillColor(sf::Color::Blue);
        else text.setFillColor(sf::Color::White);
        menu_items->push_back(text);
    }
}

std::vector<sf::Text>* Menu::get_menu_items() {
    return menu_items;
}

std::vector<Menu*>* Menu::get_sub_menus() {
    return sub_menus;
}

void Menu::render(real currentWindowX, real currentWindowY, sf::RenderWindow *window) {
    for (int i = 0; i < num_menu_items; i++) {
        // center all items
        menu_items->at(i).setPosition(currentWindowX, currentWindowY + (i-1) * Constants::TILE_SIZE);
        menu_items->at(i).setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
        menu_items->at(i).setStyle(sf::Text::Bold | sf::Text::Italic);
        window->draw(menu_items->at(i));
    }
}

void Menu::move_up() {
    menu_items->at(current_menu_items_idx).setFillColor(sf::Color::White);
    // if at the top, go to last
    if (current_menu_items_idx == 0) current_menu_items_idx = num_menu_items - 1;
    else current_menu_items_idx--;
    menu_items->at(current_menu_items_idx).setFillColor(sf::Color::Blue);
}

void Menu::move_down() {
    menu_items->at(current_menu_items_idx).setFillColor(sf::Color::White);
    // if at the bottom, go to first
    if (current_menu_items_idx == num_menu_items - 1) current_menu_items_idx = 0;
    else current_menu_items_idx++;
    menu_items->at(current_menu_items_idx).setFillColor(sf::Color::Blue);
}

void Menu::reset_menu_item_idx() {
    menu_items->at(current_menu_items_idx).setFillColor(sf::Color::White);
    current_menu_items_idx = 0;
    menu_items->at(current_menu_items_idx).setFillColor(sf::Color::Blue);
}

bool Menu::has_sub_menus() {
    return sub_menus != nullptr;
}

void Menu::add_sub_menu(Menu *menu, int menuIndex) {
    if (sub_menus == nullptr) {
        sub_menus = new std::vector<Menu*>();
    }
    sub_menus->push_back(menu);
    (*sub_menus_indices_map)[menuIndex] = menu;
    menu->set_parent_menu(this);
}

Menu* Menu::get_parent_menu() {
    return parent_menu;
}

void Menu::set_is_active(bool flag) {
    active = flag;
}

bool Menu::is_active() {
    return active;
}

bool Menu::is_game_menu() {
    return game_menu;
}

void Menu::set_parent_menu(Menu *menu) {
    parent_menu = menu;
}

int Menu::execute(Constants::GameState *state) {
    return current_menu_items_idx;
}

bool Menu::is_sub_menu_item() {
    return (*sub_menus_indices_map)[current_menu_items_idx] != nullptr;
}
