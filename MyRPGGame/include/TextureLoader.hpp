#include <SFML/Graphics.hpp>
#include <iostream>
#include <string.h>


class TextureLoader {
private:
    static TextureLoader* instance;
    TextureLoader();
    
public:
     ~TextureLoader();
     TextureLoader(TextureLoader& loader) = delete;
     void operator=(const TextureLoader&) = delete;
     
     static TextureLoader* getInstance();
     static void disposeInstance();
     
     static Texture loadTexture(std::string path);
};
