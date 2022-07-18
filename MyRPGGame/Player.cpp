#include "Player.hpp"
//#include "Game.hpp"
//#include "TextureLoader.hpp"

Player::Player() : GameEntity() {
    expPoints = 0;
    
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
    Texture playerTexture;
    playerTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/player.png");
    sprite.setTexture(playerTexture);
    sprite.setOrigin(32, 32);
    sprite.setPosition(position);
}

Player::Player(PlayerType type) : GameEntity() {
    expPoints = 0;
    
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
    Texture playerTexture;
    playerTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/player.png");
    sprite.setTexture(playerTexture);
    sprite.setOrigin(32, 32);
    sprite.setPosition(position);
    setPlayerType(type);
    entityCircle = new Circle(position.x, position.y, 32);
    attackRangeCircle = new Circle(position.x, position.y, (entityCircle->getRadius() * 5/3) + weapon->getHitRadius());
}

int Player::getStrengthPoints() {
    return strengthPoints;
}

int Player::getIntelligencePoints() {
    return intelligencePoints;
}

int Player::getCriticalHitPoints() {
    return criticalHitsPoints;
}

int Player::getExpPoints() {
    return expPoints;
}

void Player::incrementExpPoints(int amount) {
    // TODO: add logic for handling leveling up
    expPoints += amount;
}

void Player::levelUpStrengthPoints() {
    strengthPoints++;
}

void Player::levelUpIntelligencePoints() {
    intelligencePoints++;
}

void Player::levelUpCriticalHitsPoints() {
    criticalHitsPoints++;
}

void Player::setPlayerType(PlayerType type) {
    this->type = type;
    switch (this->type) {
        case PlayerType::KNIGHT:
            setWeapon(WeaponType::SWORD);
            break;
        case PlayerType::DUAL_WIELDER:
            setWeapon(WeaponType::DAGGER);
            break;
        case PlayerType::WIZARD:
            setWeapon(WeaponType::STAFF);
            break;
    }
    // increase player stats with it's weapon's stats (maybe defence points for some?)
    increaseAttackPoints(weapon->getAttackPoints());
}
