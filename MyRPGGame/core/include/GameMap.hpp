//#pragma once

#ifndef GameMap_hpp
#define GameMap_hpp

#include <iostream>
#include <SFML/Graphics.hpp>
#include "NPCEnemy.hpp"
#include "Player.hpp"
#include <vector>
#include "Circle.hpp"
#include "Constants.h"
#include "LandscapeEntity.hpp"
#include "Graph.hpp"
#include "RigidBodyForceRegistry.hpp"
#include "RigidBodyGravity.hpp"
#include "Collision.hpp"
class NPCEnemy;

class GameMap {
private:
    // in relation to the 2d array of world map
    int world_map_row;
    int world_map_col;
    Graph<Point *> *map_graph;
    bool initialized_map_graph = false; // used also for indicator for player's first arrival

    bool exitableFromLeft, exitableFromRight, exitableFromTop, exitableFromBottom;

    std::unique_ptr<Circle> leftExitCircle;
    std::unique_ptr<Circle> rightExitCircle;
    std::unique_ptr<Circle> topExitCircle;
    std::unique_ptr<Circle> bottomExitCircle;

    sf::Texture texture;
//    sf::Sprite *backgroundSprite;
    std::unique_ptr<sf::Sprite> background_sprite;

    const int NUM_OF_MAX_ENEMIES = 3;
    // areas where entities cannot move or reach by walking
    std::vector<LandscapeEntity *> landscapes;
    // enemies in current map
    std::vector<NPCEnemy *> enemies_vector;
    std::vector<GameEntity *> entities; // contains NPCs and player
    std::vector<physics::RigidBody *> bodies; // contains all game entities
    std::shared_ptr<Player> player;
    // for handling all collisions and forces in the map
    std::unique_ptr<physics::RigidBodyGravity> gravity_force_generator;
    std::unique_ptr<physics::RigidBodyGravity> ground_force_generator;
    std::unique_ptr<physics::RigidBodyForceRegistry> force_registry;
    
public:
    GameMap(int row, int col);
    GameMap(int row, int col, bool up, bool down, bool right, bool left);
    GameMap(int row, int col, Circle *up, Circle *down, Circle *right, Circle *left);
    ~GameMap();

    int get_world_map_row() const;
    int get_world_map_col() const;
    
    void init();

    sf::Sprite* get_background_sprite();

    std::vector<LandscapeEntity *> get_landscapes();
    void add_landscape(LandscapeEntity* entity);
    bool isExitableFromLeft() const;
    bool isExitableFromRight() const;
    bool isExitableFromTop() const;
    bool isExitableFromBottom() const;
    // for changing maps during gameplay
    void setIsExitableFromLeft(bool flag);
    void setIsExitableFromRight(bool flag);
    void setIsExitableFromTop(bool flag);
    void setIsExitableFromBottom(bool flag);

    Player *get_player();
    physics::RigidBodyForceRegistry *get_force_registry() const;
    void set_player(std::shared_ptr<Player> new_player);
    void remove_player();
    std::vector<NPCEnemy *> get_enemies();
    void add_enemy(NPCEnemy *enemy);
    void remove_enemy_at_index(int i);
    void remove_enemy(NPCEnemy *enemy);
    void remove_all_enemies();

    void set_top_exit_circle(Circle *circle);
    void set_bottom_exit_circle(Circle *circle);
    void set_right_exit_circle(Circle *circle);
    void set_left_exit_circle(Circle *circle);

    Circle *getTopExitCircle();
    Circle *getBottomExitCircle();
    Circle *getRightExitCircle();
    Circle *getLeftExitCircle();

    static real generate_random(int min, int max);

    void resolve_collisions(real dt);
    void update(real dt);
    
    bool operator ==(const GameMap& map) const;
};

#endif /* GameMap_hpp */
