#ifndef MYRPGGAME_CHARACTERCREATIONMENU_HPP
#define MYRPGGAME_CHARACTERCREATIONMENU_HPP

#include "Menu.hpp"
#include "../../entities/include/Player.hpp"

class CharacterCreationMenu : public Menu {

private:
    std::string bodyFileName, torsoFileName, pantsFileName;
    std::vector<std::string> mainItemsStrings, bodyItemsStrings, torsoItemsStrings, pantsItemsStrings;
    Player player;

public:
    explicit CharacterCreationMenu(std::vector<std::string> itemsStrings);
    ~CharacterCreationMenu() = default;

    void setBodyFileName(std::string fileName);
    void setTorsoFileName(std::string fileName);
    void setPantsFileName(std::string fileName);
    void updateBodyMenuItems();
    void updateTorsoMenuItems();
    void updatePantsMenuItems();

    MenuActions execute() override;

};

#endif
