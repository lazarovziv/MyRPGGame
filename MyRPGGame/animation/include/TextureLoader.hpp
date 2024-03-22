#include "SFML/Graphics.hpp"
#include <iostream>
#include <string.h>

class TextureLoader {
private:
    static TextureLoader *instance;
    TextureLoader();
    
public:
     ~TextureLoader();
     TextureLoader(TextureLoader& loader) = delete;
     void operator=(const TextureLoader&) = delete;
     
     static TextureLoader* getInstance() {
         if (instance == nullptr) {
             instance = new TextureLoader();
         }
         return instance;
     }

     static void disposeInstance() {
         delete instance;
         instance = nullptr;
     }
     
     static sf::Texture loadTexture(const char* path) {
         sf::Texture texture;
         if (texture.loadFromFile(path)) {
             std::cout << "Texture loaded properly." << std::endl;
             return texture;
         } else {
             std::cout << "Texture loaded unsuccessfuly." << std::endl;
             return texture;
         }
     }
};
