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
    long id;
    LandscapeType type;
    
public:
    LandscapeEntity();
    LandscapeEntity(LandscapeType type, int x, int y);
    LandscapeEntity(LandscapeType type, Point *center);
    ~LandscapeEntity();
    long getID();
    LandscapeType getType();
    
};

#endif /* LandscapeEntity_hpp */
