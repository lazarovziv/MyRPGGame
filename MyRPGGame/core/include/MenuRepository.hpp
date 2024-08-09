#ifndef MYRPGGAME_MENUREPOSITORY_HPP
#define MYRPGGAME_MENUREPOSITORY_HPP

#include "Menu.hpp"
#include <SFML/Graphics.hpp>

class MenuRepository {
private:
    Menu *menu;

public:
    MenuRepository() = default;
    ~MenuRepository() = default;

    void move_up();
    void move_down();
    void set_menu(Menu *menu);
    int execute(Constants::GameState *state);
    void reset_menu_item_idx();
    bool menu_has_sub_menus();
    void update_sub_menu(Menu *current_menu, Constants::GameState *state);
    bool chose_sub_menu_item();
    bool is_game_menu();

};

#endif //MYRPGGAME_MENUREPOSITORY_HPP