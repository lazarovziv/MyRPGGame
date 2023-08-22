#ifndef Constants_h
#define Constants_h

class Constants {
public:
    static const int SCREEN_WIDTH = 800;
    static const int SCREEN_HEIGHT = 640;
    static const int FULL_SCREEN_WIDTH = 4 * SCREEN_WIDTH;
    static const int FULL_SCREEN_HEIGHT = 3 * SCREEN_HEIGHT;
    static const int TILE_SIZE = 64;
    static const int BASE_ENTITY_SPEED = 16;
    static const int FPS = 60;
    static const int NUM_ROWS = 3;
    static const int NUM_COLS = 3;

    // enum for distinguishing moves - CHANGE_* values means needs to change to relevant map
    enum MoveSuccessValues {
        NOT_MOVED, FAILURE, SUCCESS, CHANGE_UP, CHANGE_DOWN, CHANGE_RIGHT, CHANGE_LEFT
    };
};

#endif /* Constants_h */
