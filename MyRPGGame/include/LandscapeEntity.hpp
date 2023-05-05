#pragma once

#ifndef LandscapeEntity_hpp
#define LandscapeEntity_hpp

#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../include/Constants.h"
#include "../include/GameEntity.hpp"

using namespace sf;

enum class LandscapeType { TREE, GRASS, FLOWER, HOUSE }; // add more

class LandscapeEntity : public GameEntity {
private:
    LandscapeType type;
    
public:
    LandscapeEntity() = default;
    LandscapeEntity(LandscapeType type, int x, int y);
    LandscapeEntity(LandscapeType type, Point *center);
    LandscapeType getType();
    
};

#endif /* LandscapeEntity_hpp */
