#include "Player.hpp"
#include "Game.hpp"
//#include "TextureLoader.hpp"

Player::Player() : GameEntity() {
    expPoints = 0;
    
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
    Texture playerTexture;
    playerTexture.loadFromFile("/Users/zivlazarov/Projects/C++/MyRPGGame/graphics/player.png");
    sprite.setTexture(playerTexture);
    sprite.setOrigin(Game::TILE_SIZE/2, Game::TILE_SIZE/2);
    sprite.setPosition(position);
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
}
