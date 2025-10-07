#include "AI.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

/* ----------------------------------------------------------- */
AI::AI()
{
    aiColor = 1;
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

/* ----------------------------------------------------------- */
std::vector<int> AI::GetBoardState(const Board& b)
{
    return b.GetBoardVector();
}

/* ----------------------------------------------------------- */
GameMove AI::GetBestMove(const Board& board, int currentPlayer)
{
    auto moves = board.GetAllLegalMoves(currentPlayer);
    if (moves.empty()) return GameMove{ -1,-1,-1,-1,false };
    return moves[std::rand() % moves.size()];
}

/* ----------------------------------------------------------- */
GameMove AI::GetBestMove_AlphaBeta(const Board& board,
    int currentPlayer,
    int depth,
    bool maximizingPlayer)
{
    /* push root on explicit stack */
    StackNode root{ board, GameMove{ -1,-1,-1,-1,false } };
    pathStack.push_back(root);

    auto moves = board.GetAllLegalMoves(currentPlayer);
    if (moves.empty())
    {
        pathStack.pop_back();
        return GameMove{ -1,-1,-1,-1,false };
    }

    int bestScore = maximizingPlayer ? INT_MIN : INT_MAX;
    GameMove bestMove = moves.front();

    for (auto& mv : moves)
    {
        Board next = board;
        next.SetCell(mv.fromRow, mv.fromCol, 0);
        next.SetCell(mv.toRow, mv.toCol, currentPlayer);

        StackNode child{ next, mv };
        pathStack.push_back(child);

        int score = AlphaBeta(next,
            depth - 1,
            INT_MIN,
            INT_MAX,
            !maximizingPlayer,
            aiColor,
            maximizingPlayer /*rootIsMax*/);

        if (maximizingPlayer)
        {
            if (score > bestScore)
            {
                bestScore = score;
                bestMove = mv;
                principalVariation.assign(pathStack.begin(), pathStack.end());
            }
        }
        else
        {
            if (score < bestScore)
            {
                bestScore = score;
                bestMove = mv;
                principalVariation.assign(pathStack.begin(), pathStack.end());
            }
        }
        pathStack.pop_back();
    }

    pathStack.pop_back();     // pop root
    return bestMove;
}

/* ----------------------------------------------------------- */
int AI::AlphaBeta(const Board& node,
    int depth,
    int alpha,
    int beta,
    bool maximizingPlayer,
    int aiPlayer,
    bool rootIsMax)
{
    int currentPlayer = maximizingPlayer ? aiColor : GetOpponent(aiColor);
    auto moves = node.GetAllLegalMoves(currentPlayer);

    if (depth == 0 || IsGameOver(node) || moves.empty())
        return EvaluateBoard(node, aiPlayer);

    if (maximizingPlayer)
    {
        int value = INT_MIN;
        for (auto& mv : moves)
        {
            Board nb = node;
            nb.SetCell(mv.fromRow, mv.fromCol, 0);
            nb.SetCell(mv.toRow, mv.toCol, currentPlayer);

            StackNode sn{ nb, mv };
            pathStack.push_back(sn);

            int eval = AlphaBeta(nb, depth - 1,
                alpha, beta,
                false, aiPlayer, rootIsMax);

            pathStack.pop_back();

            value = std::max(value, eval);
            alpha = std::max(alpha, eval);
            if (alpha >= beta) break;
        }
        return value;
    }
    else
    {
        int value = INT_MAX;
        for (auto& mv : moves)
        {
            Board nb = node;
            nb.SetCell(mv.fromRow, mv.fromCol, 0);
            nb.SetCell(mv.toRow, mv.toCol, currentPlayer);

            StackNode sn{ nb, mv };
            pathStack.push_back(sn);

            int eval = AlphaBeta(nb, depth - 1,
                alpha, beta,
                true, aiPlayer, rootIsMax);

            pathStack.pop_back();

            value = std::min(value, eval);
            beta = std::min(beta, eval);
            if (alpha >= beta) break;
        }
        return value;
    }
}

/* ----------------------------------------------------------- */
/* -------  Heuristic evaluation (unchanged)  ---------------- */
int AI::EvaluateBoard(const Board& board, int aiPlayer)
{
    int n = board.GetDimension();
    int opponent = GetOpponent(aiPlayer);
    int totalTokens = n - 2;

    /* win / loss check */
    int blueInGoal = 0, redInGoal = 0;
    for (int c = 0; c < n; ++c) if (board.GetCell(n - 1, c) == 1) ++blueInGoal;
    for (int r = 0; r < n; ++r) if (board.GetCell(r, n - 1) == 2) ++redInGoal;

    if (aiPlayer == 1 && blueInGoal == totalTokens) return  100000;
    if (aiPlayer == 2 && redInGoal == totalTokens) return  100000;
    if (opponent == 1 && blueInGoal == totalTokens) return -100000;
    if (opponent == 2 && redInGoal == totalTokens) return -100000;

    int exitScore = (aiPlayer == 1 ? blueInGoal : redInGoal) * 1500
        - (aiPlayer == 1 ? redInGoal : blueInGoal) * 1500;

    auto myMoves = board.GetAllLegalMoves(aiPlayer);
    auto oppMoves = board.GetAllLegalMoves(opponent);

    int jumpToExit = 0;
    for (auto& m : myMoves)
        if (m.isJump && ((aiPlayer == 1 && m.toRow == n - 1) ||
            (aiPlayer == 2 && m.toCol == n - 1)))
            ++jumpToExit;
    int jumpToExitScore = jumpToExit * 5000;

    int myJ = 0, oppJ = 0;
    for (auto& m : myMoves) if (m.isJump) ++myJ;
    for (auto& m : oppMoves) if (m.isJump) ++oppJ;
    int jumpScore = myJ * 4000 - oppJ * 3000;

    int dist = 0;
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
        {
            int cell = board.GetCell(r, c);
            if (cell == aiPlayer)
                dist += (n - (aiPlayer == 1 ? n - 1 - r : n - 1 - c)) * 5;
            else if (cell == opponent)
                dist -= (n - (opponent == 1 ? n - 1 - r : n - 1 - c)) * 5;
        }

    int mobility = ((int)myMoves.size() - (int)oppMoves.size()) * 10;

    int basic = 0;
    for (auto& m : myMoves) if (!m.isJump) ++basic;
    int basicScore = basic * 50;

    return jumpToExitScore + jumpScore + exitScore
        + dist + mobility + basicScore;
}
/* ----------------------------------------------------------- */
bool AI::IsGameOver(const Board& board)
{
    int n = board.GetDimension();
    int blue = 0, red = 0;
    for (int c = 0; c < n; ++c) if (board.GetCell(n - 1, c) == 1) ++blue;
    for (int r = 0; r < n; ++r) if (board.GetCell(r, n - 1) == 2) ++red;
    return (blue == n - 2) || (red == n - 2);
}
/* ----------------------------------------------------------- */
int AI::GetOpponent(int p) { return p == 1 ? 2 : 1; }
/* ----------------------------------------------------------- */
