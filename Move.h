#pragma once

// Represents a single legal move for a piece on the board.
struct GameMove {
    // The (row, col) from which the piece moves.
    int fromRow;
    int fromCol;

    // The (row, col) to which the piece moves.
    int toRow;
    int toCol;
        
    int aiMove;

    // True if this move is a jump (skipping over one opponent piece).
    // False if it's just a normal one-step move.
    bool isJump;
};
