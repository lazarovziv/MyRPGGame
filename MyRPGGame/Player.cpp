#include "Player.hpp"
//#include "Game.hpp"
//#include "TextureLoader.hpp"

Player::Player() : GameEntity() {
    expPoints = 0;
    speed = 2.f;
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
    texture.loadFromFile("../graphics/player_down_1_64.png");
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position);
}

Player::Player(PlayerType type) : GameEntity() {
    expPoints = 0;
//    speed = 6.f;
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
    if (!texture.loadFromFile("../graphics/johnny_64.png")) {
        std::cout << "Texture NOT loaded properly!" << endl;
        texture.setSmooth(true);
    } else std::cout << "Texture loaded properly." << endl;
    // playerTexture.loadFromFile("/home/ziv/projects/cpp/MyRPGGame/graphics/player.png");
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
    // sprite->scale(2.0, 2.0);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position);
    setPlayerType(type);
    entityCircle = new Circle(position.x, position.y, Constants::TILE_SIZE/2);
    attackRangeCircle = new Circle(position.x, position.y, (entityCircle->getRadius() * (float)5/3) + weapon->getHitRadius());
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
