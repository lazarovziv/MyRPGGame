#include "../include/Player.hpp"
//#include "Game.hpp"

Player::Player() : GameEntity() {
    expPoints = 0;
    speed = 2;
//    texture = new Texture();
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
    sprite = new Sprite();
    texture.loadFromFile("../graphics/player_down_1_64.png");
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
}

Player::Player(PlayerType type) : GameEntity() {
    expPoints = 0;
    strengthPoints = 0;
    intelligencePoints = 0;
    criticalHitsPoints = 0;
    this->type = type;
//    speed = 6.f;
    setPlayerType(type);
    entityCircle = new Circle(position.x, position.y, Constants::TILE_SIZE/4);
    attackRangeCircle = new Circle(position.x, position.y, (entityCircle->getRadius() * (float) 10/3) + weapon->getHitRadius());

    for (int state = 0; state < 10; state++) {
        auto movementState = static_cast<EntityMovementState>(state);
        createMovementStateSprite(movementState);
    }
    sprite = movementStateSpritesMap[EntityMovementState::WALK];
}

Player::Player(PlayerType type, Point *center) : GameEntity(center) {
    expPoints = 0;
    strengthPoints = 0;
    intelligencePoints = 0;
    criticalHitsPoints = 0;
    this->type = type;
    if (!texture.loadFromFile("../graphics/player/gray_t_shirt_leather_pants_spritesheet.png")) {
        std::cout << "Texture NOT loaded properly!" << endl;
        texture.setSmooth(true);
    } else std::cout << "Texture loaded properly." << endl;
    // TextureLoader.getInstance()->loadFromFile("player.png");
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(moveDirectionsSpritesMap[moveDirection] * Constants::TILE_SIZE,
                                       Constants::WALK_ROW * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE));
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    setPlayerType(type);
    attackRangeCircle->setRadius(attackRangeCircle->getRadius() + weapon->getHitRadius());
    battleTimeout = 0.15;
}

Player::~Player() {
    delete weapon;
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

void Player::setPlayerType(PlayerType t) {
    type = t;
    switch (type) {
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
    // increase player stats with its' weapon's stats (maybe defence points for some?)
    increaseAttackPoints(weapon->getAttackPoints());
}

// TODO: add more functionality or else use GameEntity's update method
void Player::update(Point ***points, float dt) {
    if (!dead) {
        sprite->setPosition(position.x, position.y);
        // updating entity circle and attack circle
        entityCircle->setCenter(points[(int)position.y][(int)position.x]);
        attackRangeCircle->setCenter(points[(int)position.y][(int)position.x]);

        weapon->update(points);
        // notifying all observers (enemies) in current map
        notifyAll();
    }
}
