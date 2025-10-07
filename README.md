# AI Turn-Based Game Using Minimax and Backtracking

## Overview

This project is a **C++ two-player strategic board game** that implements **Artificial Intelligence** using the **Minimax Algorithm** with **Alpha-Beta Pruning** and **Backtracking**.  
The game allows players to compete as **Blue** or **Red** in three modes:

-  Human vs Human  
-  Human vs AI  
-  AI vs AI  

The system evaluates moves, predicts player strategies, and finds the optimal path to victory based on heuristic scoring.

---

## ⚙️ Features

- 🧩 Interactive Windows Form GUI built with Visual Studio  
- ♟️ Three playable modes (Human vs Human, Human vs AI, AI vs AI)  
- 🧠 AI decision-making using **Minimax Algorithm** with **Alpha-Beta Pruning**  
- 🔁 Backtracking search for move exploration and optimization  
- 📊 Heuristic evaluation based on:
  - Distance to goal  
  - Jump opportunities  
  - Mobility  
  - Blocking opponent moves  
  - Goal proximity  

---

## 🧠 Algorithms Used

### 🔹 Minimax Algorithm
Used for optimal decision-making by simulating all possible game states and choosing the move that minimizes potential loss.

### 🔹 Alpha-Beta Pruning
Optimizes Minimax by eliminating branches that do not affect the final decision, improving performance.

### 🔹 Backtracking
Performs depth-first exploration of possible moves and undoing decisions to find the best sequence of actions.


## 🧰 Technologies Used

- **Language:** C++  
- **IDE:** Visual Studio  
- **Framework:** Windows Forms (.NET)  

---

## 🚀 How to Run

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/AI-TurnBased-Game-Minimax.git
