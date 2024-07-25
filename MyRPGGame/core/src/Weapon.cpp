#include "../include/Weapon.hpp"

Weapon::Weapon(physics::Vector initialPosition, WeaponType type) {
    // TODO: handle shield differently with the directions map
    transition_directions_sprites_map[MoveDirection::UP] = 1;
    transition_directions_sprites_map[MoveDirection::LEFT] = 2;
    transition_directions_sprites_map[MoveDirection::DOWN] = 3;
    transition_directions_sprites_map[MoveDirection::RIGHT] = 4;

    rigid_body = std::make_unique<physics::Circle>(initialPosition.x, initialPosition.y, initialPosition.z, (real) Constants::TILE_SIZE/4);
//    weaponCircle = new Circle(center, (float) Constants::TILE_SIZE/4);

    switch (type) {
        case WeaponType::SWORD:
            name = "Sword";
            attack_points = 10;
            hit_radius = 5 * 6.5f;
            num_hits_per_second = 3;
            // init graphics
            sprite = new sf::Sprite();
            if (!texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "images/weapons/oversize/right_hand/male/longsword_male.png")) {
                std::cout << "Texture NOT loaded properly!" << std::endl;
                texture.setSmooth(true);
            } else std::cout << "Weapon loaded properly." << std::endl;
            sprite->setTexture(texture);
            sprite->setTextureRect(sf::IntRect(transition_directions_sprites_map[transition_direction]*Constants::TILE_SIZE, 0, Constants::TILE_SIZE, Constants::TILE_SIZE));
            sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
            sprite->setPosition(position.x, position.y);

            break;
        case WeaponType::DAGGER:
            name = "Dagger";
            attack_points = 5;
            hit_radius = 2 * 4.5f;
            num_hits_per_second = 7;
            break;
        case WeaponType::AXE:
            name = "Axe";
            attack_points = 12;
            hit_radius = 6.5f * 6.5f;
            num_hits_per_second = 2;
            break;
        case WeaponType::MACE:
            name = "Mace";
            attack_points = 12;
            hit_radius = 6.5f * 6.5f;
            num_hits_per_second = 2;
            break;
        case WeaponType::BARE_HANDED:
            name = "Bare Handed";
            attack_points = 1;
            hit_radius = 1 * 3.5f;
            num_hits_per_second = 12;
            break;
        case WeaponType::STAFF:
            name = "Staff";
            attack_points = 2;
            hit_radius = 3 * 4.5f;
            num_hits_per_second = 3;
            break;
    }

    position.x = rigid_body->get_position().x;
    position.y = rigid_body->get_position().y;
}

void Weapon::increase_attack_points(int amount) {
    attack_points += amount;
}

void Weapon::increase_hit_radius(real amount) {
    hit_radius += amount;
}

void Weapon::increase_num_hits_per_second(int amount) {
    num_hits_per_second += amount;
}

void Weapon::decrease_attack_points(int amount) {
    // don't let any attack points lower than 1
    if (attack_points - amount <= 1) return;
    attack_points -= amount;
}

void Weapon::decrease_hit_radius(real amount) {
    // don't let any hit radius lower than 1
    if (hit_radius - amount <= 1) return;
    hit_radius -= amount;
}

void Weapon::decrease_num_hits_per_second(int amount) {
    // don't let any hit radius lower than 1
    if (num_hits_per_second - amount <= 1) return;
    num_hits_per_second -= amount;
}

int Weapon::get_attack_points() {
    return attack_points;
}

real Weapon::get_hit_radius() {
    return hit_radius;
}

int Weapon::get_num_hits_per_second() {
    return num_hits_per_second;
}

void Weapon::set_int_rect_position(int left, int top, int width, int height) {
    sprite_rect.left = left;
    sprite_rect.top = top;
    sprite_rect.width = width;
    sprite_rect.height = height;
    sprite->setTextureRect(sprite_rect);
}

MoveDirection Weapon::get_transition_direction() {
    return transition_direction;
}

std::map<MoveDirection, int> Weapon::get_transition_directions_sprites_map() {
    return transition_directions_sprites_map;
}

sf::Vector2f Weapon::get_position() {
    return position;
}

sf::Sprite* Weapon::get_sprite() {
    return sprite;
}

Circle* Weapon::get_weapon_circle() {
    return weapon_circle;
}

int Weapon::get_transition() {
    return transition;
}

void Weapon::increment_transition() {
    // defined by the pngs order
    // TODO: add an extra field for identifying another weapons which uses different logic
    if (transition < Constants::WEAPON_TRANSITION_MAX) transition++;
    else transition = Constants::WEAPON_TRANSITION_MIN;
}

void Weapon::set_transition_direction(MoveDirection direction) {
    transition_direction = direction;
}

void Weapon::update(real dt) {
    position.x = rigid_body->get_position().x;
    position.y = rigid_body->get_position().y;
    sprite->setPosition(position.x, position.y);
//    weaponCircle->setCenter(points[(int)position.y][(int)position.x]);
}
