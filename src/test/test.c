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
    char input[10] = {0};

    game.active_piece = I;
    game.active_position = (Position){ 2, 0 };
    game.active_orientation = ROTATE_0;

    // add sample stack
    // for (int i = 0; i < GAME_BOARD_WIDTH; i++) {
    //     game.board[GAME_BOARD_HEIGHT - 2][i] = 1;
    //     game.board[GAME_BOARD_HEIGHT - 3][i] = 1;
    //     if (i % 2 == 0) {
    //         game.board[GAME_BOARD_HEIGHT - 4][i] = 1;
    //         game.board[GAME_BOARD_HEIGHT - 1][i] = 1;
    //     }
    // }

    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);

    game.active_piece = I;
    game.active_position = (Position){ 2, 0 };
    game.active_orientation = ROTATE_0;


    printf("Board:\n");
    display(&game);


    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);
    applyGravity(&game);


    game.active_piece = I;
    game.active_position = (Position){ 2, 0 };
    game.active_orientation = ROTATE_0;
    printf("Board:\n");
    display(&game);


    return 0;
}