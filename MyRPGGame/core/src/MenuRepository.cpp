#include "MenuRepository.hpp"

void MenuRepository::set_menu(Menu *menu) {
    menu->set_is_active(false);
    this->menu = menu;
}

int MenuRepository::execute(Constants::GameState *state) {
    return menu->execute(state);
}

void MenuRepository::move_up() {
    menu->move_up();
}

void MenuRepository::move_down() {
    menu->move_down();
}

void MenuRepository::reset_menu_item_idx() {
    menu->reset_menu_item_idx();
}

bool MenuRepository::menu_has_sub_menus() {
    return menu->has_sub_menus();
}

void MenuRepository::update_sub_menu(Menu *currentMenu, Constants::GameState *state) {
    set_menu(menu->get_sub_menus()->at(menu->execute(state)));
    currentMenu = this->menu;
}

bool MenuRepository::chose_sub_menu_item() {
    return menu->is_sub_menu_item();
}

bool MenuRepository::is_game_menu() {
    return menu->is_game_menu();
}
