#pragma once

#ifndef LandscapeEntity_hpp
#define LandscapeEntity_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "GameEntity.hpp"

enum class LandscapeType { TREE, GRASS, FLOWER, HOUSE }; // add more

class LandscapeEntity : public GameEntity {
private:
    LandscapeType type;
    
public:
    LandscapeEntity() = default;
    LandscapeEntity(LandscapeType type, physics::Vector initial_position, std::vector<physics::Vector> &vertices, real mass = Constants::REAL_MAX);
    LandscapeType get_type();
    
};

#endif /* LandscapeEntity_hpp */
