#include <stdio.h>
#include <string.h>

#include "../physics.h"

/**
 * Prints the game board to std out.
 */
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
    tetris_init();

    Game game = {0};
    char input[128] = {0};
    uint8_t numRead = 0;
    spawnNextTetromino(&game);

    display(&game);
    printf("> ");
    numRead = scanf("%s", input);
    input[9] = '\0';

    while (numRead > 0) {

        if (strcmp(input, "q") == 0 || strcmp(input, "quit") == 0) {
            break;
        } else if (strcmp(input, "?") == 0 || strcmp(input, "help") == 0) {
            printf("Options:\n");
            printf("?, help: displays this message\n");
            printf("q, quit: quits the program\n");
            printf("--------\n");
            printf("l, left: shifts the active tetromino left\n");
            printf("r, right: shifts the active tetromino right\n");
            printf("g, gravity: makes the active tetromino fall\n");
            printf("cw: rotate clockwise\n");
            printf("ccw: rotate counter-clockwise\n");
        } else if (strcmp(input, "gravity") == 0 || strcmp(input, "g") == 0) {
            applyGravity(&game);
            printf("Applying gravity...\n");
            display(&game);
        } else if (strcmp(input, "cw") == 0) {

            printf("Rotating clockwise...\n");
            display(&game);
        } else if (strcmp(input, "ccw") == 0) {

            printf("Rotating counter-clockwise...\n");
            display(&game);
        } else if (strcmp(input, "l") == 0 || strcmp(input, "left") == 0) {
            moveActivePiece(&game, LEFT);
            printf("Shifting left...\n");
            display(&game);
        } else if (strcmp(input, "r") == 0 || strcmp(input, "right") == 0) {
            moveActivePiece(&game, RIGHT);
            printf("Shifting right...\n");
            display(&game);
        }

        printf("> ");
        numRead = scanf("%s", input);
        input[127] = '\0';

    }

    return 0;
}