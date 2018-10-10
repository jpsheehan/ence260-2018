#include <stdio.h>
#include "../tetris.h"
#include "../physics.h"

void display(Game* game)
{
    fillFramebuffer(game);
    for (int i = 0; i < GAME_BOARD_HEIGHT; i++) {
        for (int j = 0; j < GAME_BOARD_WIDTH; j++) {
            switch (frameBuffer[i][j]) {
                case EMPTY: printf("."); break;
                case STACK: printf("#"); break;
                case ACTIVE: printf("@"); break;
                case GHOST: printf("G"); break;
                default: printf("?"); break;
            }
        }
        printf("\n");
    }
}

int main(void)
{
    Game game = {0};
    uint8_t clears = 0;

    game.active_position = (Position){ 2, 0 };
    game.active_orientation = ROTATE_90;
    applyGravity(&game);

    for (int i = 0; i < GAME_BOARD_WIDTH; i++) {
        game.board[GAME_BOARD_HEIGHT - 2][i] = 1;
        game.board[GAME_BOARD_HEIGHT - 3][i] = 1;
        if (i % 2 == 0) {
            game.board[GAME_BOARD_HEIGHT - 4][i] = 1;
            game.board[GAME_BOARD_HEIGHT - 1][i] = 1;
        }
    }
    printf("Board:\n");
    display(&game);

    clears = processLineClears(&game);
    printf("Found %d line clears:\n", clears);
    display(&game);

    return 0;
}