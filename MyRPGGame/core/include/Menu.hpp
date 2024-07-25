//#pragma once

#ifndef MYRPGGAME_MENU_HPP
#define MYRPGGAME_MENU_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include <map>
#include <vector>

//using namespace std;

enum MenuActions {
    ACTION_RESUME, ACTIONS_INVENTORY, ACTION_SETTINGS, ACTION_EXIT, ACTION_SUBMENU
};

class Menu {
protected:
    std::vector<sf::Text> *menu_items;
    sf::Font font;
    int current_menu_items_idx;
    int num_menu_items;
    bool active; // indicator whether the menu is currently shown on screen (if switched off we'll go to parent menu)
    bool game_menu = false;
    Menu *parent_menu; // for navigating backwards from the submenus
    std::vector<Menu*> *sub_menus = nullptr; // setting an option for every menu to have a sub menu
    std::map<int, Menu*> *sub_menus_indices_map = nullptr;

    void set_parent_menu(Menu *menu);

public:
    explicit Menu(std::vector<std::string> items_strings, bool game_menu);
    ~Menu() = default;

    void init_menu_items(std::vector<std::string> items_strings);
    std::vector<sf::Text> *get_menu_items();
    std::vector<Menu*> *get_sub_menus();
    void render(real current_window_x, real current_window_y, sf::RenderWindow *window);
    void move_up();
    void move_down();
    bool has_sub_menus();
    void add_sub_menu(Menu *menu, int menu_index);
    Menu *get_parent_menu();
    void set_is_active(bool flag);
    bool is_active();
    bool is_game_menu();
    virtual bool is_sub_menu_item();
    virtual int execute(Constants::GameState *state);
    void reset_menu_item_idx();
};

#endif //MYRPGGAME_MENU_HPP
