TextureLoader* TextureLoader::instance = nullptr;

TextureLoader* TextureLoader::getInstance() {
    if (instance == nullptr) {
        instance = new TextureLoader();
    }
    return instance;
}

void TextureLoader::disposeInstance() {
    delete instance;
    instance = nullptr;
}

Texture TextureLoader::loadTexture(std::string path) {
    Texture texture;
    if (texture.loadFromFile(strcat("../../graphics/", path))) {
        std::cout < "Texture loaded properly." << endl;
        return texture;
    } else {
        std::cout << "Texture loaded unsuccessfuly." << endl;
        return NULL;
    } 
} 