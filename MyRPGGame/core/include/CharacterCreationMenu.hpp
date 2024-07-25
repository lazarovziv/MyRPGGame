#ifndef MYRPGGAME_CHARACTERCREATIONMENU_HPP
#define MYRPGGAME_CHARACTERCREATIONMENU_HPP

#include "Menu.hpp"
#include "Player.hpp"

class CharacterCreationMenu : public Menu {

private:
    std::string bodyFileName, torso_file_name, pants_file_name;
    std::vector<std::string> main_items_strings, body_items_strings, torso_items_strings, pants_items_strings;
    Player player;

public:
    explicit CharacterCreationMenu(std::vector<std::string> items_strings);
    ~CharacterCreationMenu() = default;

    void set_body_file_name(std::string file_name);
    void set_torso_file_name(std::string file_name);
    void set_pants_file_name(std::string file_name);
    void update_body_menu_items();
    void update_torso_menu_items();
    void update_pants_menu_items();

    MenuActions execute() override;

};

#endif
