#ifndef MYRPGGAME_MAINMENU_HPP
#define MYRPGGAME_MAINMENU_HPP

#include "Menu.hpp"

class MainMenu : public Menu {
public:
    MainMenu(std::vector<std::string> itemsStrings);

    MenuActions execute() override;
};

#endif //MYRPGGAME_MAINMENU_HPP
