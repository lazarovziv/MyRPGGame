#ifndef LandscapeEntity_hpp
#define LandscapeEntity_hpp

#include <stdio.h>
#include <SFML/Graphics.hpp>

using namespace sf;

enum class LandscapeType { TREE, GRASS, FLOWER, HOUSE }; // add more

class LandscapeEntity {
private:
    long id;
    Vector2f position;
    Sprite sprite;
    LandscapeType type;
    
public:
    LandscapeEntity();
    LandscapeEntity(LandscapeType type, float x, float y);
    ~LandscapeEntity() = default;
    long getID();
    Vector2f getPosition();
    Sprite getSprite();
    LandscapeType getType();
    
};

#endif /* LandscapeEntity_hpp */
