#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL.h>

// Resolution
#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// Overscan compensation
#define SAFE_WIDTH 1728
#define SAFE_HEIGHT 972
#define SAFE_X ((SCREEN_WIDTH-SAFE_WIDTH)/2)
#define SAFE_Y ((SCREEN_HEIGHT-SAFE_HEIGHT)/2)

// Game numbers
#define MAP_X (SAFE_X + 64)
#define MAP_Y (SAFE_Y + 128)

#define MAP_COLUMNS 25
#define MAP_ROWS 13

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define PLAYER_WIDTH 64
#define PLAYER_HEIGHT 128

#define SUBTILE_WIDTH 8

SDL_Rect GetSafeArea1920();

#endif // UTILS_HPP
