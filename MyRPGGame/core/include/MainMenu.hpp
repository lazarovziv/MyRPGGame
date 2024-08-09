//#pragma once

#ifndef MYRPGGAME_MAINMENU_HPP
#define MYRPGGAME_MAINMENU_HPP

#include "Menu.hpp"

class MainMenu : public Menu {
public:
    explicit MainMenu(std::vector<std::string> items_strings);

    MenuActions execute() override;
};

#endif //MYRPGGAME_MAINMENU_HPP
