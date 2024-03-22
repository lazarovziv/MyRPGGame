//#pragma once

#ifndef GameMap_hpp
#define GameMap_hpp

#include <iostream>
#include "SFML/Graphics.hpp"
#include "../../entities/include/NPCEnemy.hpp"
#include "../../entities/include/Player.hpp"
#include <vector>
#include "../../common/include/Constants.h"
#include "../../entities/include/LandscapeEntity.hpp"
#include "../../common/include/Graph.hpp"
#include "../../physics/include/RigidBodyForceRegistry.hpp"
#include "../../physics/include/RigidBodyGravity.hpp"
#include "../../physics/include/Collision.hpp"

class NPCEnemy;

class GameMap {
private:
    // in relation to the 2d array of world map
    int worldMapRow;
    int worldMapCol;
    bool initializedMapGraph = false; // used also for indicator for player's first arrival

    bool exitableFromLeft, exitableFromRight, exitableFromTop, exitableFromBottom;

    sf::Texture texture;
//    sf::Sprite *backgroundSprite;
    std::unique_ptr<sf::Sprite> backgroundSprite;

    const int NUM_OF_MAX_ENEMIES = 3;
    // areas where entities cannot move or reach by walking
    std::vector<LandscapeEntity *> landscapes;
    // enemies in current map
    std::vector<NPCEnemy *> enemiesVector;
    std::vector<GameEntity *> entities; // contains NPCs and player
    std::vector<physics::RigidBody *> bodies; // contains all game entities
    std::shared_ptr<Player> player;
    // for handling all collisions and forces in the map
    std::unique_ptr<physics::RigidBodyGravity> gravityForceGenerator;
    std::unique_ptr<physics::RigidBodyGravity> groundForceGenerator;
    std::unique_ptr<physics::RigidBodyForceRegistry> forceRegistry;
    
public:
    GameMap(int row, int col);
    GameMap(int row, int col, bool up, bool down, bool right, bool left);
    ~GameMap();

    int getWorldMapRow() const;
    int getWorldMapCol() const;
    
    void init();

    sf::Sprite* getBackgroundSprite();

    std::vector<LandscapeEntity *> getLandscapes();
    void addLandscape(LandscapeEntity* entity);
    bool isExitableFromLeft() const;
    bool isExitableFromRight() const;
    bool isExitableFromTop() const;
    bool isExitableFromBottom() const;
    // for changing maps during gameplay
    void setIsExitableFromLeft(bool flag);
    void setIsExitableFromRight(bool flag);
    void setIsExitableFromTop(bool flag);
    void setIsExitableFromBottom(bool flag);

    Player *getPlayer();
    physics::RigidBodyForceRegistry *getForceRegistry() const;
    void setPlayer(std::shared_ptr<Player> newPlayer);
    void removePlayer();
    std::vector<NPCEnemy *> getEnemies();
    void addEnemy(NPCEnemy *enemy);
    void removeEnemyAtIndex(int i);
    void removeEnemy(NPCEnemy *enemy);
    void removeAllEnemies();

    static real generateRandom(int min, int max);

    void resolveCollisions(real dt);
    void update(real dt);
    
    bool operator ==(const GameMap& map) const;
};

#endif /* GameMap_hpp */
