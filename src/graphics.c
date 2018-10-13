#include "graphics.h"
#include "physics.h"

void fillFramebuffer(Game *game)
{   
    // copy the stack data and clear the frameBuffer at the same time
    uint8_t i = 0;
    for (; i < GAME_BOARD_HEIGHT; i++) {
        uint8_t j = 0;
        for (; j < GAME_BOARD_WIDTH; j++) {
            if (game->board[i][j]) {
                game->framebuffer[i][j] = STACK;
            } else {
                game->framebuffer[i][j] = EMPTY;
            }
        }
    }

    // make it easier to access thhe position of the active piece
    Position* absPos = &game->active_position;
    Position *posData = getCollisionData(game->active_piece, game->active_orientation);

    // draw the active piece
    for (i = 0; i < 4; i++) {
        Position relPos = posData[i];
        int8_t x = absPos->x + relPos.x;
        int8_t y = absPos->y + relPos.y;

        if (x >= 0 && y >= 0 && x < GAME_BOARD_WIDTH && y < GAME_BOARD_HEIGHT && game->framebuffer[y][x] != STACK) {
            game->framebuffer[y][x] = ACTIVE;
        }
    }
    
}