# ENCE260 Tetris

*Written by Ben Slattery and Jesse Sheehan*

## General Overview

The idea of this project is to give a basic implementation of classic Tetris.

## Game Controls

### Main Menu

- Left/Right navigation buttons navigate the menu
- Centre navigation button selects the menu option

### In-Game

- Left/Right navigation buttons move the current piece left or right
- Down navigation button rotates a piece clockwise
- Up navigation button rotates a piece counter-clockwise
- Centre navigation button performs a non-locking soft-drop
- The side button holds a piece

## Rules

As we have limited hardware (most obviously, a 5x7 display) we cannot adhere to the [Tetris Guideline](http://tetris.wikia.com/wiki/Tetris_Guideline). The following rules apply:
- Playfield is 5x12, where rows above 7 are hidden from the player. This is required so that we don't get any nasty errors when placing an I piece at the very top of the stack.
- The following Tetrominos are used:
  - I piece
  - O piece
  - T piece
  - S piece
  - Z piece
  - J piece
  - L piece
- All pieces spawn at position (1, 0)
- SRS rotations and kicks are used