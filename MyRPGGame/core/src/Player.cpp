#include "../include/Player.hpp"
//#include "Game.hpp"

Player::Player() : GameEntity() {
    exp_points = 0;
//    texture = new Texture();
//    sprite.setTexture(TextureLoader::getInstance()->loadTexture("player.png"));
//    sprite = new sf::Sprite();
    texture.loadFromFile("../graphics/player_down_1_64.png");
    sprite->setTexture(texture);
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
}

Player::Player(PlayerType type, physics::Vector initialPosition) : GameEntity(initialPosition, physics::RigidBodyType::CIRCLE) {
    exp_points = 0;
    strength_points = 0;
    intelligence_points = 0;
    critical_hist_points = 0;
    is_player = true;
    this->type = type;
    // gray_t_shirt_leather_pants_spritesheet
    if (!texture.loadFromFile(Constants::GRAPHICS_BASE_PATH + "player/uriel_spritesheet.png")) {
        std::cout << "Texture NOT loaded properly!" << std::endl;
        texture.setSmooth(true);
    } else std::cout << "Texture loaded properly." << std::endl;
    // TextureLoader.getInstance()->loadFromFile("player.png");
    sprite->setTexture(texture);
    sprite->setTextureRect(sf::IntRect(move_directions_sprites_map[move_direction] * Constants::TILE_SIZE,
                                       Constants::WALK_ROW * Constants::TILE_SIZE,
                                       Constants::TILE_SIZE, Constants::TILE_SIZE));
    sprite->setOrigin(Constants::TILE_SIZE/2, Constants::TILE_SIZE/2);
    sprite->setPosition(position.x, position.y);
    set_player_type(type);
}

Player::~Player() {
//    delete weapon;
}

int Player::get_strength_points() {
    return strength_points;
}

int Player::get_intelligence_points() {
    return intelligence_points;
}

int Player::get_critical_hist_points() {
    return critical_hist_points;
}

int Player::get_exp_points() {
    return exp_points;
}

void Player::increment_exp_points(int amount) {
    // TODO: add logic for handling leveling up
    exp_points += amount;
}

void Player::level_up_strength_points() {
    strength_points++;
}

void Player::level_up_intelligence_points() {
    intelligence_points++;
}

void Player::level_up_critical_hits_points() {
    critical_hist_points++;
}

void Player::set_player_type(PlayerType t) {
    type = t;
    switch (type) {
        case PlayerType::KNIGHT:
            set_weapon(WeaponType::SWORD);
            break;
        case PlayerType::DUAL_WIELDER:
            set_weapon(WeaponType::DAGGER);
            break;
        case PlayerType::WIZARD:
            set_weapon(WeaponType::STAFF);
            break;
    }
    // increase player stats with its' weapon's stats (maybe defence points for some?)
    increase_attack_points(weapon->get_attack_points());
}
