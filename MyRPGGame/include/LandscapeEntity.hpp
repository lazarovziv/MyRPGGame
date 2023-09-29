#pragma once

#ifndef LandscapeEntity_hpp
#define LandscapeEntity_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Constants.h"
#include "../include/GameEntity.hpp"

enum class LandscapeType { TREE, GRASS, FLOWER, HOUSE }; // add more

class LandscapeEntity : public GameEntity {
private:
    LandscapeType type;
    
public:
    LandscapeEntity() = default;
    LandscapeEntity(LandscapeType type, physics::Vector initialPosition);
    LandscapeType getType();
    
};

#endif /* LandscapeEntity_hpp */
