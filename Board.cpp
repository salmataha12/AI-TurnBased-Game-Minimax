#include "Board.h"

// Constructor: Initializes the board's dimension and vector.
Board::Board(int n) {
    dimension = n;
    boardVector = std::vector<int>(n * n, 0);
    InitializeBoard();
}

// Sets all cells of the board to 0.
void Board::InitializeBoard() {
    for (int i = 0; i < dimension * dimension; i++) {
        boardVector[i] = 0;
    }
}

// Converts the (row, col) coordinates to a 1D index and returns the cell's value.
int Board::GetCell(int row, int col) const {
    if (row >= 0 && row < dimension && col >= 0 && col < dimension)
        return boardVector[row * dimension + col];
    else
        return -1;  // Return -1 if indices are out of bounds.
}

// Sets the cell at (row, col) to the given value.
void Board::SetCell(int row, int col, int value) {
    if (row >= 0 && row < dimension && col >= 0 && col < dimension)
        boardVector[row * dimension + col] = value;
}

// Prints the board's mapped state as a 2D grid to the console.
void Board::PrintBoard() const {
    for (int row = 0; row < dimension; row++) {
        for (int col = 0; col < dimension; col++) {
            std::cout << boardVector[row * dimension + col] << " ";
        }
        std::cout << std::endl;
    }
}
#include "Board.h"
#include "Move.h"
#include <iostream> // if you use std::cout anywhere

std::vector<GameMove> Board::GetAllLegalMoves(int currentPlayer) const
{
    std::vector<GameMove> moves;

    // We'll iterate over every cell. If it belongs to currentPlayer, 
    // we check for normal/jump moves in the correct direction.
    for (int row = 0; row < dimension; ++row)
    {
        for (int col = 0; col < dimension; ++col)
        {
            if (GetCell(row, col) == currentPlayer)
            {
                if (currentPlayer == 1)  // Blue moves down
                {
                    // 1) Normal down: (row+1, col)
                    int nr = row + 1;
                    int nc = col;
                    if (nr < dimension && GetCell(nr, nc) == 0)
                    {
                        // fromRow= row, fromCol= col -> toRow= nr, toCol= nc
                        GameMove m{ row, col, nr, nc, false };
                        moves.push_back(m);
                    }

                    // 2) Jump down: (row+2, col), if the middle cell is occupied
                    //    and the landing cell is empty
                    int jr = row + 2;
                    int mr = row + 1;  // middle row
                    if (jr < dimension
                        && GetCell(mr, col) != 0   // occupant in the middle
                        && GetCell(jr, col) == 0) // landing empty
                    {
                        GameMove m{ row, col, jr, col, true };
                        moves.push_back(m);
                    }
                }
                else if (currentPlayer == 2) // Red moves right
                {
                    // 1) Normal right: (row, col+1)
                    int nr = row;
                    int nc = col + 1;
                    if (nc < dimension && GetCell(nr, nc) == 0)
                    {
                        GameMove m{ row, col, nr, nc, false };
                        moves.push_back(m);
                    }

                    // 2) Jump right: (row, col+2)
                    int jc = col + 2;
                    int mc = col + 1;  // middle col
                    if (jc < dimension
                        && GetCell(row, mc) != 0
                        && GetCell(row, jc) == 0)
                    {
                        GameMove m{ row, col, row, jc, true };
                        moves.push_back(m);
                    }
                }
            }
        }
    }

    return moves;
}
