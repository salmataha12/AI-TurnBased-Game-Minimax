#pragma once
#include <vector>
#include <climits>
#include "Board.h"
#include "Move.h"

class AI
{
public:
    AI();

    /* ------- public helpers ---------- */
    std::vector<int> GetBoardState(const Board& board);
    GameMove         GetBestMove(const Board& board, int currentPlayer);

    /* main AB search with explicit stack */
    GameMove GetBestMove_AlphaBeta(const Board& board,
        int currentPlayer,
        int depth,
        bool maximizingPlayer);

    int AlphaBeta(const Board& node,
        int depth,
        int alpha,
        int beta,
        bool maximizingPlayer,
        int aiPlayer,
        bool rootIsMax);

    int  EvaluateBoard(const Board& board, int aiPlayer);
    bool IsGameOver(const Board& board);
    int  GetOpponent(int player);

    void SetAIColor(int c) { aiColor = c; }
    int  GetAIColor() const { return aiColor; }

    /* principal variation for debugging */
    struct StackNode
    {
        Board    board;
        GameMove moveFromParent;
    };
    const std::vector<StackNode>& GetPV() const { return principalVariation; }

private:
    int aiColor;

    /* explicit DFS stack used during search */
    std::vector<StackNode> pathStack;
    /* best line found so far */
    std::vector<StackNode> principalVariation;
};
