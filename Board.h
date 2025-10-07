#pragma once

#include "Move.h"
#include <vector>
#include <iostream>

class Board {
private:
    int dimension;               // The board will be dimension x dimension.
    std::vector<int> boardVector;   // 1D vector to hold board cells.

public:
    // Constructor: creates a board of size n x n.
    Board(int n);

    // Initialize or reset the board. All cells are set to 0.
    void InitializeBoard();

    // Retrieve the value of a cell at (row, col).
    // Returns -1 if (row, col) is out of bounds.
    int GetCell(int row, int col) const;

    // Update the value of the cell at (row, col).
    void SetCell(int row, int col, int value);

    // Print the board's mapped state to the console.
    void PrintBoard() const;

    // Helper to get the dimension.
    int GetDimension() const { return dimension; }

    // Access the underlying vector. The AI can get the board state from here.
    const std::vector<int>& GetBoardVector() const { return boardVector; }

    std::vector<GameMove> GetAllLegalMoves(int currentPlayer) const;
};
