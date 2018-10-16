# ENCE260 Tetris

*Written by Ben Slattery and Jesse Sheehan*

## General Overview

The idea of this project is to give a basic implementation of classic Tetris.

## Game Controls

### Main Menu

- Left/Right navigation buttons navigate the menu
- Centre navigation button selects the menu option

You will be greeted with a *Tetris* title screen. You will be given the options to play a *1 Player Game* or a *2 Player Game*.

### In-Game

- Left/Right navigation buttons move the current piece left or right
- Up navigation button rotates a piece clockwise
- Down navigation button rotates a piece counter-clockwise
- Centre navigation button performs a holds a piece
- The side button makes the piece fall faster

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
- A 7-bag random piece selection system is used to compute the next random piece.
- You will lose if the next piece is unable to spawn or the the stack grows to high.
- You clear a line when you create a full row.
- When playing a two player game, a line of junk is sent to the opposing player when you clear two lines.
- You will be presented with a win or lose screen when the game is over and you will be returned to the main menu.
