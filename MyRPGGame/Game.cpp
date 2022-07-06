#include "Game.hpp"

using namespace std;

Game::Game(const char* str, int width, int height) {
    screenWidth = width;
    screenHeight = height;
    title = str;
    this->videoMode = new VideoMode(screenWidth, screenHeight);
    std::string s(title);
    this->window = new RenderWindow(*videoMode, s);
    
    state = GameState::IN_MENU;
    
    currentGameMapRow = 0;
    currentGameMapCol = 0;
    
    worldMap = new GameMap**[3];
    for (int r = 0; r < 3; r++) {
        worldMap[r] = new GameMap*[3];
        for (int c = 0; c < 3; c++) {
            worldMap[r][c] = new GameMap(r, c);
        }
    }
}

void Game::update() {
    
}

void Game::render() {
    
}

void Game::changeState(GameState state) {
    this->state = state;
}

int Game::getScreenWidth() {
    return screenWidth;
}

int Game::getScreenHeight() {
    return screenHeight;
}

RenderWindow* Game::getWindow() {
    return window;
}

GameState Game::getState() {
    return state;
}

GameMap*** Game::getWorldMap() {
    return worldMap;
}

GameMap* Game::getCurrentGameMap() {
    return worldMap[currentGameMapRow][currentGameMapCol];
}
