#include "../include/Weapon.hpp"

Weapon::Weapon(physics::Vector initialPosition, WeaponType type) {
    // TODO: handle shield differently with the directions map
    transitionDirectionsSpritesMap[MoveDirection::UP] = 1;
    transitionDirectionsSpritesMap[MoveDirection::LEFT] = 2;
    transitionDirectionsSpritesMap[MoveDirection::DOWN] = 3;
    transitionDirectionsSpritesMap[MoveDirection::RIGHT] = 4;

    rigidBody = std::make_unique<physics::Circle>(initialPosition.x, initialPosition.y, initialPosition.z, (real) Constants::TILE_SIZE/4);
//    weaponCircle = new Circle(center, (float) Constants::TILE_SIZE/4);

    switch (type) {
        case WeaponType::SWORD:
            name = "Sword";
            attackPoints = 10;
            hitRadius = 5 * 6.5f;
            numHitsPerSecond = 3;
            // init graphics
            sprite = new sf::Sprite();
            if (!texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "images/weapons/oversize/right_hand/male/longsword_male.png")) {
                std::cout << "Texture NOT loaded properly!" << std::endl;
                texture.setSmooth(true);
            } else std::cout << "Weapon loaded properly." << std::endl;
            sprite->setTexture(texture);
            sprite->setTextureRect(sf::IntRect(transitionDirectionsSpritesMap[transitionDirection]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
            sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
            sprite->setPosition(position.x, position.y);

            break;
        case WeaponType::DAGGER:
            name = "Dagger";
            attackPoints = 5;
            hitRadius = 2 * 4.5f;
            numHitsPerSecond = 7;
            break;
        case WeaponType::AXE:
            name = "Axe";
            attackPoints = 12;
            hitRadius = 6.5f * 6.5f;
            numHitsPerSecond = 2;
            break;
        case WeaponType::MACE:
            name = "Mace";
            attackPoints = 12;
            hitRadius = 6.5f * 6.5f;
            numHitsPerSecond = 2;
            break;
        case WeaponType::BARE_HANDED:
            name = "Bare Handed";
            attackPoints = 1;
            hitRadius = 1 * 3.5f;
            numHitsPerSecond = 12;
            break;
        case WeaponType::STAFF:
            name = "Staff";
            attackPoints = 2;
            hitRadius = 3 * 4.5f;
            numHitsPerSecond = 3;
            break;
    }

    position.x = rigidBody->getPosition().x;
    position.y = rigidBody->getPosition().y;
}

void Weapon::increaseAttackPoints(int amount) {
    attackPoints += amount;
}

void Weapon::increaseHitRadius(real amount) {
    hitRadius += amount;
}

void Weapon::increaseNumHitsPerSecond(int amount) {
    numHitsPerSecond += amount;
}

void Weapon::decreaseAttackPoints(int amount) {
    // don't let any attack points lower than 1
    if (attackPoints - amount <= 1) return;
    attackPoints -= amount;
}

void Weapon::decreaseHitRadius(real amount) {
    // don't let any hit radius lower than 1
    if (hitRadius - amount <= 1) return;
    hitRadius -= amount;
}

void Weapon::decreaseNumHitsPerSecond(int amount) {
    // don't let any hit radius lower than 1
    if (numHitsPerSecond - amount <= 1) return;
    numHitsPerSecond -= amount;
}

int Weapon::getAttackPoints() {
    return attackPoints;
}

real Weapon::getHitRadius() {
    return hitRadius;
}

int Weapon::getNumHitsPerSecond() {
    return numHitsPerSecond;
}

void Weapon::setIntRectPosition(int left, int top, int width, int height) {
    spriteRect.left = left;
    spriteRect.top = top;
    spriteRect.width = width;
    spriteRect.height = height;
    sprite->setTextureRect(spriteRect);
}

MoveDirection Weapon::getTransitionDirection() {
    return transitionDirection;
}

std::map<MoveDirection, int> Weapon::getTransitionDirectionsSpritesMap() {
    return transitionDirectionsSpritesMap;
}

sf::Vector2f Weapon::getPosition() {
    return position;
}

sf::Sprite* Weapon::getSprite() {
    return sprite;
}

int Weapon::getTransition() {
    return transition;
}

void Weapon::incrementTransition() {
    // defined by the pngs order
    // TODO: add an extra field for identifying another weapons which uses different logic
    if (transition < Constants::WEAPON_TRANSITION_MAX) transition++;
    else transition = Constants::WEAPON_TRANSITION_MIN;
}

void Weapon::setTransitionDirection(MoveDirection direction) {
    transitionDirection = direction;
}

void Weapon::update(real dt) {
    position.x = rigidBody->getPosition().x;
    position.y = rigidBody->getPosition().y;
    sprite->setPosition(position.x, position.y);
//    weaponCircle->setCenter(points[(int)position.y][(int)position.x]);
}
