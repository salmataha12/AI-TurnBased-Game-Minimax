#pragma once

/* ------------------------------------------------------------------ */
/*  ALL REQUIRED INCLUDES                                             */
/* ------------------------------------------------------------------ */
#include <vector>
#include <ctime>
#include <climits>                 // INT_MIN / INT_MAX
#include "Board.h"
#include "AI.h"
#include "Move.h"

/* ------------------------------------------------------------------ */
namespace AlgoPrj
{
    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Text;

    /* ================================================================== */
    /*                           MyForm CLASS                             */
    /* ================================================================== */
    public ref class MyForm : public Form
    {
    public:
        MyForm()
        {
            InitializeComponent();

            /* double‑buffer to reduce flicker */
            this->SetStyle(ControlStyles::DoubleBuffer, true);
            this->SetStyle(ControlStyles::AllPaintingInWmPaint, true);
            this->SetStyle(ControlStyles::UserPaint, true);
            this->UpdateStyles();

            /* ========= logical initialisation ========= */
            n = 5;
            gameBoard = new Board(n);
            aiLogic = new AI();
            gameMode = 1;           // 1 = H‑v‑H
            aiDifficulty = AIDifficulty::Easy;

            pieceSelected = false;
            currentPlayer = 1;
            blueTotal = redTotal = n - 2;
            blueInGoal = redInGoal = 0;
            gameOver = false;

            recFromRow = recFromCol = recToRow = recToCol = -1;
            recIsJump = false;
            humanColor = 0;

            /* timer for AI vs AI */
            aiTimer = gcnew Timer();
            aiTimer->Interval = 200;
            aiTimer->Tick += gcnew EventHandler(this, &MyForm::OnAITimerTick);

            InitializeTokens();

            if (comboBoxPlayer->SelectedItem->ToString() == "Blue")
            {
                humanColor = 1;
                aiLogic->SetAIColor(2);
                currentPlayer = 1;
            }
            else
            {
                humanColor = 2;
                aiLogic->SetAIColor(1);
                currentPlayer = 1;
                if (gameMode == 2) ExecuteAIMove();
            }

            labelGameOver->Text = "";
            buttonRestart->Visible = false;

            /* hook paint & click */
            this->Paint += gcnew PaintEventHandler(this, &MyForm::MyForm_Paint);
            this->MouseClick += gcnew MouseEventHandler(this, &MyForm::MyForm_MouseClick);
        }

        /* ---- destructor ---- */
    protected:
        ~MyForm()
        {
            if (components) delete components;
            if (gameBoard) { delete gameBoard;  gameBoard = nullptr; }
            if (aiLogic) { delete aiLogic;    aiLogic = nullptr; }
        }

        /* ================================================================== */
        /*                          DATA MEMBERS                              */
        /* ================================================================== */
    private:
        IContainer^ components;

        /* logical/game state */
        int                 n;
        Board* gameBoard;
        AI* aiLogic;

        int  currentPlayer;      // 1 blue, 2 red
        int  humanColor;         // human side (1/2)
        bool pieceSelected;
        int  selectedRow, selectedCol;

        int  gameMode;           // 1 HvH  2 HvAI  3 AIvAI
        bool gameOver;

        /* counts */
        int blueTotal, redTotal;
        int blueInGoal, redInGoal;

        /* recommendation tracking */
        int  recFromRow, recFromCol, recToRow, recToCol;
        bool recIsJump;

        /* UI controls */
        NumericUpDown^ numericUpDown1;
        ComboBox^ comboBoxPlayer;
        ComboBox^ comboBoxGameMode;
        ComboBox^ comboBoxAIDifficulty;

        Label^ labelStatus;
        Label^ labelBlueScore;
        Label^ labelRedScore;
        Label^ labelGameOver;
        Button^ buttonRestart;
        TextBox^ textBoxRecs;
        Button^ buttonApplyRec;

        /* timers */
        System::Windows::Forms::Timer^ aiTimer;

        /* enumeration for difficulty */
        enum class AIDifficulty { Easy, Hard };
        AIDifficulty aiDifficulty;

        /* ================================================================== */
        /*                     DESIGNER‑GENERATED CODE                        */
        /* ================================================================== */
#pragma region Windows Form Designer
        void InitializeComponent(void)
        {
            this->numericUpDown1 = gcnew NumericUpDown();
            this->comboBoxPlayer = gcnew ComboBox();
            this->comboBoxGameMode = gcnew ComboBox();
            this->comboBoxAIDifficulty = gcnew ComboBox();
            this->labelBlueScore = gcnew Label();
            this->labelRedScore = gcnew Label();
            this->labelGameOver = gcnew Label();
            this->buttonRestart = gcnew Button();
            this->textBoxRecs = gcnew TextBox();
            this->buttonApplyRec = gcnew Button();
            this->labelStatus = gcnew Label();

            (cli::safe_cast<ISupportInitialize^>(this->numericUpDown1))->BeginInit();
            this->SuspendLayout();

            /* numericUpDown */
            this->numericUpDown1->Location = Point(12, 12);
            this->numericUpDown1->Minimum = Decimal(2);
            this->numericUpDown1->Maximum = Decimal(30);
            this->numericUpDown1->Value = Decimal(5);
            this->numericUpDown1->Size = Drawing::Size(70, 22);
            this->numericUpDown1->Name = L"numericUpDown1";
            this->numericUpDown1->ValueChanged +=
                gcnew EventHandler(this, &MyForm::numericUpDown1_ValueChanged);

            /* comboBoxPlayer */
            this->comboBoxPlayer->Location = Point(100, 12);
            this->comboBoxPlayer->Size = Drawing::Size(80, 24);
            this->comboBoxPlayer->Name = L"comboBoxPlayer";
            this->comboBoxPlayer->Items->Add("Blue");
            this->comboBoxPlayer->Items->Add("Red");
            this->comboBoxPlayer->SelectedIndex = 0;
            this->comboBoxPlayer->SelectedIndexChanged +=
                gcnew EventHandler(this, &MyForm::comboBoxPlayer_SelectedIndexChanged);

            /* comboBoxGameMode */
            this->comboBoxGameMode->Location = Point(190, 12);
            this->comboBoxGameMode->Size = Drawing::Size(100, 24);
            this->comboBoxGameMode->Name = L"comboBoxGameMode";
            this->comboBoxGameMode->Items->Add("Human vs Human");
            this->comboBoxGameMode->Items->Add("Human vs AI");
            this->comboBoxGameMode->Items->Add("AI vs AI");
            this->comboBoxGameMode->SelectedIndex = 0;
            this->comboBoxGameMode->SelectedIndexChanged +=
                gcnew EventHandler(this, &MyForm::comboBoxGameMode_SelectedIndexChanged);

            /* comboBoxAIDifficulty */
            this->comboBoxAIDifficulty->Location = Point(300, 12);
            this->comboBoxAIDifficulty->Size = Drawing::Size(100, 24);
            this->comboBoxAIDifficulty->Name = L"comboBoxAIDifficulty";
            this->comboBoxAIDifficulty->Items->Add("Easy");
            this->comboBoxAIDifficulty->Items->Add("Hard");
            this->comboBoxAIDifficulty->SelectedIndex = 0;
            this->comboBoxAIDifficulty->SelectedIndexChanged +=
                gcnew EventHandler(this, &MyForm::comboBoxAIDifficulty_SelectedIndexChanged);

            /* labels */
            this->labelBlueScore->AutoSize = true;
            this->labelBlueScore->Location = Point(460, 12);
            this->labelBlueScore->Text = L"Blue: 0/0";

            this->labelRedScore->AutoSize = true;
            this->labelRedScore->Location = Point(460, 32);
            this->labelRedScore->Text = L"Red: 0/0";

            this->labelGameOver->AutoSize = true;
            this->labelGameOver->Location = Point(12, 540);

            /* restart button */
            this->buttonRestart->Location = Point(150, 535);
            this->buttonRestart->Size = Drawing::Size(100, 30);
            this->buttonRestart->Text = L"Restart";
            this->buttonRestart->Visible = false;
            this->buttonRestart->Click += gcnew EventHandler(this, &MyForm::buttonRestart_Click);

            /* recommendations textbox */
            this->textBoxRecs->Location = Point(12, 620);
            this->textBoxRecs->Size = Drawing::Size(760, 100);
            this->textBoxRecs->Multiline = true;
            this->textBoxRecs->ReadOnly = true;
            this->textBoxRecs->ScrollBars = ScrollBars::Vertical;

            /* apply‑rec button */
            this->buttonApplyRec->Location = Point(680, 730);
            this->buttonApplyRec->Size = Drawing::Size(92, 30);
            this->buttonApplyRec->Text = L"Apply Rec";
            this->buttonApplyRec->Click += gcnew EventHandler(this, &MyForm::buttonApplyRec_Click);

            /* status label */
            this->labelStatus->AutoSize = true;
            this->labelStatus->Location = Point(12, 565);

            /* add controls */
            this->Controls->Add(this->numericUpDown1);
            this->Controls->Add(this->comboBoxPlayer);
            this->Controls->Add(this->comboBoxGameMode);
            this->Controls->Add(this->comboBoxAIDifficulty);
            this->Controls->Add(this->labelBlueScore);
            this->Controls->Add(this->labelRedScore);
            this->Controls->Add(this->labelGameOver);
            this->Controls->Add(this->buttonRestart);
            this->Controls->Add(this->textBoxRecs);
            this->Controls->Add(this->buttonApplyRec);
            this->Controls->Add(this->labelStatus);

            this->ClientSize = Drawing::Size(800, 800);
            this->Name = L"MyForm";
            this->Text = L"MyForm";

            (cli::safe_cast<ISupportInitialize^>(this->numericUpDown1))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();
        }
#pragma endregion   /* designer */

        /* ================================================================== */
        /*                    ---  LOGIC  SECTION  ---                        */
        /* ================================================================== */
    private:

        /* ----------  token initialisation  ------------ */
        void InitializeTokens()
        {
            gameBoard->InitializeBoard();

            for (int c = 1; c < n - 1; ++c) gameBoard->SetCell(0, c, 1);
            for (int r = 1; r < n - 1; ++r) gameBoard->SetCell(r, 0, 2);

            blueTotal = redTotal = n - 2;
            blueInGoal = redInGoal = 0;
            UpdateGoalCounts();
        }

        /* ----------  board‑size spinner  --------------- */
        System::Void numericUpDown1_ValueChanged(System::Object^, EventArgs^)
        {
            n = Decimal::ToInt32(numericUpDown1->Value);
            ResetGame();
            RefreshRecsDisplay();
            this->Invalidate();
        }

        /* ----------  colour selector   ----------------- */
        System::Void comboBoxPlayer_SelectedIndexChanged(System::Object^, EventArgs^)
        {
            ResetGame();
            this->Invalidate();
        }

        /* ----------  game‑mode selector  --------------- */
        System::Void comboBoxGameMode_SelectedIndexChanged(System::Object^, EventArgs^)
        {
            String^ s = comboBoxGameMode->SelectedItem->ToString();
            if (s == "Human vs Human") gameMode = 1;
            else if (s == "Human vs AI")    gameMode = 2;
            else if (s == "AI vs AI")       gameMode = 3;
            else { gameMode = 1; comboBoxGameMode->SelectedIndex = 0; }

            ResetGame();
        }

        /* ----------  difficulty selector --------------- */
        System::Void comboBoxAIDifficulty_SelectedIndexChanged(System::Object^, EventArgs^)
        {
            aiDifficulty = (comboBoxAIDifficulty->SelectedItem->ToString() == "Easy")
                ? AIDifficulty::Easy : AIDifficulty::Hard;
            ResetGame();
        }

        /* ----------  game‑restart button --------------- */
        System::Void buttonRestart_Click(System::Object^, EventArgs^)
        {
            ResetGame();
        }

        /* ----------  timer tick (AIvAI)  --------------- */
        System::Void OnAITimerTick(System::Object^, EventArgs^)
        {
            if (!gameOver && gameMode == 3) ExecuteAIMove();
            else aiTimer->Stop();
        }

        /* ----------  skip‑turn helper  ----------------- */
        void CheckAndSkipTurn()
        {
            if (!gameBoard->GetAllLegalMoves(currentPlayer).empty() || gameOver) return;

            currentPlayer = (currentPlayer == 1 ? 2 : 1);
            UpdateGoalCounts();
            this->Invalidate();

            if (gameMode == 2 && currentPlayer != humanColor) ExecuteAIMove();
        }

        /* ----------  AI move execution  ---------------- */
        void ExecuteAIMove()
        {
            if (!aiLogic) return;

            GameMove aiMove;
            labelStatus->Text = "AI thinking...";
            Application::DoEvents();

            if (aiDifficulty == AIDifficulty::Easy)
                aiMove = aiLogic->GetBestMove(*gameBoard, currentPlayer);
            else
            {
                bool isAITurn = (currentPlayer == aiLogic->GetAIColor());
                aiMove = aiLogic->GetBestMove_AlphaBeta(*gameBoard,
                    currentPlayer, 4,
                    isAITurn);
            }
            labelStatus->Text = "";

            if (aiMove.fromRow == -1) return;   /* no move */

            int token = gameBoard->GetCell(aiMove.fromRow, aiMove.fromCol);
            gameBoard->SetCell(aiMove.fromRow, aiMove.fromCol, 0);
            gameBoard->SetCell(aiMove.toRow, aiMove.toCol, token);

            currentPlayer = (currentPlayer == 1 ? 2 : 1);
            pieceSelected = false;

            UpdateGoalCounts();
            this->Invalidate();
            CheckAndSkipTurn();
        }

        /* ----------  Apply‑Rec button  ----------------- */
        System::Void buttonApplyRec_Click(System::Object^, EventArgs^)
        {
            if (gameOver) return;

            int human = humanColor;
            auto moves = gameBoard->GetAllLegalMoves(human);
            if (moves.empty()) return;

            aiLogic->SetAIColor(human);

            GameMove best = moves[0];
            int      bestScore = INT_MIN;

            for (auto& mv : moves)
            {
                Board tmp = *gameBoard;
                tmp.SetCell(mv.fromRow, mv.fromCol, 0);
                tmp.SetCell(mv.toRow, mv.toCol, human);

                int score = aiLogic->AlphaBeta(tmp, 4, INT_MIN, INT_MAX, true, human, true);

                if (score > bestScore)
                {
                    bestScore = score;
                    best = mv;
                }
            }

            int tok = gameBoard->GetCell(best.fromRow, best.fromCol);
            gameBoard->SetCell(best.fromRow, best.fromCol, 0);
            gameBoard->SetCell(best.toRow, best.toCol, tok);

            currentPlayer = (currentPlayer == 1 ? 2 : 1);
            pieceSelected = false;

            UpdateGoalCounts();
            RefreshRecsDisplay();
            this->Invalidate();
            CheckAndSkipTurn();

            if (!gameOver && gameMode == 2 && currentPlayer != human)
                ExecuteAIMove();
        }

        /* ----------  recommendation list --------------- */
        void RefreshRecsDisplay()
        {
            int human = humanColor;
            auto moves = gameBoard->GetAllLegalMoves(human);
            aiLogic->SetAIColor(human);

            GameMove best = aiLogic->GetBestMove_AlphaBeta(*gameBoard,
                human, 4, true);

            StringBuilder^ sb = gcnew StringBuilder();
            sb->AppendLine("Recs (move : score)  [* = best]");

            for (auto& mv : moves)
            {
                Board tmp = *gameBoard;
                tmp.SetCell(mv.fromRow, mv.fromCol, 0);
                tmp.SetCell(mv.toRow, mv.toCol, human);

                int score = aiLogic->AlphaBeta(tmp, 4,
                    INT_MIN, INT_MAX,
                    true, human,
                    true);     // extra param

                bool isBest = (mv.fromRow == best.fromRow && mv.fromCol == best.fromCol
                    && mv.toRow == best.toRow && mv.toCol == best.toCol);

                sb->AppendFormat("({0},{1})->({2},{3}) : {4}{5}\r\n",
                    mv.fromRow, mv.fromCol,
                    mv.toRow, mv.toCol,
                    score,
                    isBest ? "  *" : "");
            }
            textBoxRecs->Text = sb->ToString();

            recFromRow = best.fromRow;
            recFromCol = best.fromCol;
            recToRow = best.toRow;
            recToCol = best.toCol;
            recIsJump = best.isJump;
        }

        /* ----------  goal‑count + win test ------------- */
        void UpdateGoalCounts()
        {
            blueInGoal = redInGoal = 0;
            for (int c = 0; c < n; ++c) if (gameBoard->GetCell(n - 1, c) == 1) ++blueInGoal;
            for (int r = 0; r < n; ++r) if (gameBoard->GetCell(r, n - 1) == 2) ++redInGoal;

            labelBlueScore->Text = "Blue: " + blueInGoal.ToString() + "/" + blueTotal.ToString();
            labelRedScore->Text = "Red: " + redInGoal.ToString() + "/" + redTotal.ToString();

            if (blueInGoal == blueTotal)
            {
                labelGameOver->Text = "Blue wins!"; buttonRestart->Visible = true; gameOver = true;
            }
            else if (redInGoal == redTotal)
            {
                labelGameOver->Text = "Red wins!";  buttonRestart->Visible = true; gameOver = true;
            }
            else
            {
                labelGameOver->Text = "";           buttonRestart->Visible = false;
            }
        }

        /* ----------  game reset ------------------------ */
        void ResetGame()
        {
            if (aiTimer) aiTimer->Stop();
            if (gameBoard) { delete gameBoard; gameBoard = nullptr; }
            gameBoard = new Board(n);

            pieceSelected = false;
            gameOver = false;

            InitializeTokens();

            if (comboBoxPlayer->SelectedItem->ToString() == "Blue")
            {
                humanColor = 1; aiLogic->SetAIColor(2); currentPlayer = 1;
            }
            else
            {
                humanColor = 2; aiLogic->SetAIColor(1); currentPlayer = 1;
                if (gameMode == 2) ExecuteAIMove();
            }

            RefreshRecsDisplay();
            this->Invalidate();

            if (gameMode == 3) aiTimer->Start();
        }

        /* ----------  Text helper for debugging --------- */
        String^ GetBoardStateText()
        {
            StringBuilder^ sb = gcnew StringBuilder();
            for (int r = 0; r < n; ++r)
            {
                for (int c = 0; c < n; ++c)
                    sb->Append(gameBoard->GetCell(r, c).ToString() + " ");
                sb->AppendLine();
            }
            return sb->ToString();
        }

        /* ----------  mouse handling  ------------------- */
        System::Void MyForm_MouseClick(System::Object^, MouseEventArgs^ e)
        {
            if (gameOver) return;
            if (gameMode == 2 && currentPlayer != humanColor) return;

            int cellSize = 40, offX = 50, offY = 50;
            if (e->X < offX || e->Y < offY ||
                e->X >= offX + n * cellSize || e->Y >= offY + n * cellSize) return;

            int col = (e->X - offX) / cellSize;
            int row = (e->Y - offY) / cellSize;

            if (gameMode == 3) return;   // ignore during AI‑vs‑AI

            if (!pieceSelected)
            {
                int tok = gameBoard->GetCell(row, col);
                if (tok == currentPlayer)
                {
                    pieceSelected = true;
                    selectedRow = row; selectedCol = col;
                }
            }
            else
            {
                int selTok = gameBoard->GetCell(selectedRow, selectedCol);
                bool moved = false;

                if (selTok == 1) /* blue */
                {
                    int nRow = selectedRow + 1, jRow = selectedRow + 2, midR = selectedRow + 1;
                    if (row == nRow && col == selectedCol && nRow < n &&
                        gameBoard->GetCell(nRow, col) == 0)
                    {
                        gameBoard->SetCell(nRow, col, 1);
                        gameBoard->SetCell(selectedRow, selectedCol, 0);
                        moved = true;
                    }
                    else if (row == jRow && col == selectedCol && jRow < n &&
                        gameBoard->GetCell(midR, col) != 0 &&
                        gameBoard->GetCell(jRow, col) == 0)
                    {
                        gameBoard->SetCell(jRow, col, 1);
                        gameBoard->SetCell(selectedRow, selectedCol, 0);
                        moved = true;
                    }
                }
                else if (selTok == 2) /* red */
                {
                    int nCol = selectedCol + 1, jCol = selectedCol + 2, midC = selectedCol + 1;
                    if (row == selectedRow && col == nCol && nCol < n &&
                        gameBoard->GetCell(row, nCol) == 0)
                    {
                        gameBoard->SetCell(row, nCol, 2);
                        gameBoard->SetCell(selectedRow, selectedCol, 0);
                        moved = true;
                    }
                    else if (row == selectedRow && col == jCol && jCol < n &&
                        gameBoard->GetCell(row, midC) != 0 &&
                        gameBoard->GetCell(row, jCol) == 0)
                    {
                        gameBoard->SetCell(row, jCol, 2);
                        gameBoard->SetCell(selectedRow, selectedCol, 0);
                        moved = true;
                    }
                }

                if (moved) currentPlayer = (currentPlayer == 1 ? 2 : 1);
                pieceSelected = false;
            }

            UpdateGoalCounts();
            RefreshRecsDisplay();
            this->Invalidate();
            CheckAndSkipTurn();

            if (!gameOver && gameMode == 2 && currentPlayer != humanColor)
                ExecuteAIMove();
        }

        /* ----------  painting / board render ----------- */
        System::Void MyForm_Paint(System::Object^, PaintEventArgs^ e)
        {
            int cellSize = 40, circ = 30, offX = 50, offY = 50;

            SolidBrush^ grey = gcnew SolidBrush(Color::LightGray);
            e->Graphics->FillRectangle(grey,
                offX, offY + (n - 1) * cellSize, n * cellSize, cellSize);
            e->Graphics->FillRectangle(grey,
                offX + (n - 1) * cellSize, offY, cellSize, n * cellSize);
            delete grey;

            Pen^ pen = gcnew Pen(Color::Black, 1);
            Brush^ blue = gcnew SolidBrush(Color::Blue);
            Brush^ red = gcnew SolidBrush(Color::Red);
            Brush^ hi = gcnew SolidBrush(Color::Yellow);

            for (int r = 0; r < n; ++r)
                for (int c = 0; c < n; ++c)
                    e->Graphics->DrawRectangle(pen,
                        offX + c * cellSize, offY + r * cellSize, cellSize, cellSize);

            if (pieceSelected)
                e->Graphics->FillRectangle(hi,
                    offX + selectedCol * cellSize,
                    offY + selectedRow * cellSize,
                    cellSize, cellSize);

            /* move‑indicator dots */
            if (pieceSelected)
            {
                int selTok = gameBoard->GetCell(selectedRow, selectedCol);
                Brush^ ind = gcnew SolidBrush(Color::FromArgb(150, 144, 238, 144));
                int d = 20;

                if (selTok == 1)
                {
                    int nRow = selectedRow + 1, jRow = selectedRow + 2;
                    if (nRow < n && gameBoard->GetCell(nRow, selectedCol) == 0)
                        e->Graphics->FillEllipse(ind,
                            offX + selectedCol * cellSize + (cellSize - d) / 2,
                            offY + nRow * cellSize + (cellSize - d) / 2, d, d);

                    if (jRow < n && gameBoard->GetCell(selectedRow + 1, selectedCol) != 0 &&
                        gameBoard->GetCell(jRow, selectedCol) == 0)
                        e->Graphics->FillEllipse(ind,
                            offX + selectedCol * cellSize + (cellSize - d) / 2,
                            offY + jRow * cellSize + (cellSize - d) / 2, d, d);
                }
                else if (selTok == 2)
                {
                    int nCol = selectedCol + 1, jCol = selectedCol + 2;
                    if (nCol < n && gameBoard->GetCell(selectedRow, nCol) == 0)
                        e->Graphics->FillEllipse(ind,
                            offX + nCol * cellSize + (cellSize - d) / 2,
                            offY + selectedRow * cellSize + (cellSize - d) / 2, d, d);

                    if (jCol < n && gameBoard->GetCell(selectedRow, selectedCol + 1) != 0 &&
                        gameBoard->GetCell(selectedRow, jCol) == 0)
                        e->Graphics->FillEllipse(ind,
                            offX + jCol * cellSize + (cellSize - d) / 2,
                            offY + selectedRow * cellSize + (cellSize - d) / 2, d, d);
                }
                delete ind;
            }

            for (int r = 0; r < n; ++r)
                for (int c = 0; c < n; ++c)
                {
                    int tok = gameBoard->GetCell(r, c);
                    if (tok == 0) continue;

                    Brush^ b = (tok == 1 ? blue : red);
                    e->Graphics->FillEllipse(b,
                        offX + c * cellSize + (cellSize - circ) / 2,
                        offY + r * cellSize + (cellSize - circ) / 2,
                        circ, circ);
                }

            delete pen; delete blue; delete red; delete hi;
        }

        /* ================================================================== */
    };
}   // namespace
