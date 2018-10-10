#include <stdio.h>
#include <string.h>

#include "../physics.h"
#include "../tetris.h"

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
            printf("d, drop: non-locking soft-drop\n");
            printf("h, hold: holds the current piece\n");

        } else if (strcmp(input, "gravity") == 0 || strcmp(input, "g") == 0) {
            printf("Applying gravity...\n");
            if (!applyGravity(&game)) {
                if (!commitActiveTetrominoToStack(&game)) {
                    break;
                }
                
                uint8_t clears = processLineClears(&game);
                if (clears) {
                    printf("Cleared %d lines!\n", clears);
                }
                if (!spawnNextTetromino(&game)) {
                    break;
                }
            }

        } else if (strcmp(input, "cw") == 0) {
            printf("Rotating clockwise...\n");
            rotateActivePiece(&game, CLOCKWISE);

        } else if (strcmp(input, "ccw") == 0) {
            printf("Rotating counter-clockwise...\n");
            rotateActivePiece(&game, COUNTERCLOCKWISE);

        } else if (strcmp(input, "l") == 0 || strcmp(input, "left") == 0) {
            printf("Shifting left...\n");
            moveActivePiece(&game, LEFT);

        } else if (strcmp(input, "r") == 0 || strcmp(input, "right") == 0) {
            printf("Shifting right...\n");
            moveActivePiece(&game, RIGHT);
        
        } else if (strcmp(input, "d") == 0 || strcmp(input, "drop") == 0) {
            printf("Non-locking soft-drop...\n");
            softDrop(&game);

        } else if (strcmp(input, "h") == 0 || strcmp(input, "hold") == 0) {
            
            if (holdPiece(&game) == true) {
                printf("Held the active piece :)\n");
            } else {
                printf("Could not hold the piece :(\n");
            }
        }
        
        display(&game);
        printf("> ");
        numRead = scanf("%s", input);
        input[127] = '\0';

    }

    printf("Game Over!\n");
    display(&game);

    return 0;
}