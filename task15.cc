/* ----------------------------------------------------------------<Header>-
 Name: task15.cc
 Title: Solver for domino puzzles

 Group: TB-52
 Student: Demianenko Y. O.
 Written: 2025-05-03
 Revised: 2025-05-03
 Description: Develop an algorithm for solving the domino puzzle and implement it
as a program in ANSI C++. The game board contains a set of dominoes, where each 
domino appears exactly once. The objective is to find the position of each domino.
In this specific task, some numbers on the board have been erased and must be 
restored alongside finding the correct domino placement.
------------------------------------------------------------------</Header>-*/
#include <iostream>
#include <iomanip>
#include <chrono>
#include <cstdlib>
using namespace std;
struct Cell {
    int num;
    bool occupied = false;
};
const int board[8][9] = {
    {-2, -2, 0, 3, -1, 1, 5, -2, -2},
    {-2, -2, 2, 4, 3, 4, 6, -2, -2},
    {-1, 3, 6, 0, 5, 4, 6, 4, 4},
    {3, 6, 2, 3, 6, 6, 1, 5, 6},
    {3, 5, 1, 2, 0, 5, 2, 3, 6},
    {4, 2, 4, 1, 1, 3, 5, 2, 1},
    {-2, -2, 0, 1, 1, 2, 2, -2, -2},
    {-2, -2, 0, 5, -1, 4, 0, -2, -2},
};
const int ROW = size(board);
const int COL = size(board[0]);
int steps = 0;

void description();
void init(struct Cell (*puzzle)[COL]);
bool solvePuzzle(struct Cell (*puzzle)[COL], bool used[7][7], int m, int n);
void print(const int (*board)[COL]);
void print(struct Cell (*puzzle)[COL]);
int main() {
    Cell puzzle[ROW][COL];
    bool used[7][7]{false};
    int choice;
    
    cout << "Domino puzzle solver" << endl;
    cout << "------------------------------------" << endl;
    cout << "1. Description" << endl;
    cout << "2. Start solver" << endl;
    cout << "3. Solve by yourself" << endl;
    cout << "4. Quit" << endl;
    
    while (true) {
        cout << "Your choice: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error, your number must be an integer" << endl;
            continue;
        }

        switch(choice) {
            case 1:
                description();
                break;
            case 2: {
                auto start = chrono::high_resolution_clock::now();
                cout << "Start" << endl;
                print(board);
                init(puzzle);
                for (int i = 0; i < 7; i++) {
                    for (int j = 0; j < 7; j++) {
                        used[i][j] = false;
                    }
                }
                steps = 0;
                if (solvePuzzle(puzzle, used, 0, 0)) {
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::duration<double>>(end - start);
                    cout << "End" << endl;
                    print(puzzle);
                    cout << "Amount of combinations: " << steps << endl;
                    cout << "Time: " << fixed << setprecision(5) << duration.count() << " seconds" << endl;
                } else {
                    cout << "There is no solution" << endl;
                }
                break;
            }
            case 3:
                //solvePerson();
                //TODO
                break;
            case 4:
                cout << "Goodbye!" << endl;
                exit(1);
            default:
                cout << "You need to write from 1 to 4" << endl;
                break;
        }
    }
    return 0;
}

/* ---------------------------------------------------------------------[<]-
Function: description
Synopsis: TODO
---------------------------------------------------------------------[>]-*/
void description() {
    cout << "\n======================================================\n";
    cout << "                    DOMINO PUZZLE                     \n";
    cout << "======================================================\n\n";
    
    cout << "1. The game board contains a standard set of dominoes.\n";
    cout << "   Each domino appears exactly once.\n\n";
    
    cout << "2. In a classic game, the goal is to draw lines to mark\n";
    cout << "   the position of each domino.\n\n";
    
    cout << "3. Task for this solver:\n";
    cout << "   Some numbers on the game board have been erased.\n";
    cout << "   The goal is to restore these missing numbers while\n";
    cout << "   finding the correct placement of the entire set.\n\n";

    cout << "Author: Demianenko Yevhenii\n";
    cout << "Group: TV-52\n"; 
    
    cout << "======================================================\n";
}

/* ---------------------------------------------------------------------[<]-
Function: init
Synopsis: TODO
---------------------------------------------------------------------[>]-*/
void init(struct Cell (*puzzle)[COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            puzzle[i][j].num = board[i][j];
            puzzle[i][j].occupied = false;
        }
    }
}

/* ---------------------------------------------------------------------[<]-
Function: solvePuzzle
Synopsis: Recursively attempts to place all 28 unique dominoes on the board 
using a backtracking algorithm. It fills empty cells, which marked as -1, and checks 
both horizontal and vertical placements.
---------------------------------------------------------------------[>]-*/
bool solvePuzzle(struct Cell (*puzzle)[COL], bool used[7][7], int m, int n) {
    steps++;
    if (n == COL) return solvePuzzle(puzzle, used, m + 1, 0);
    if (m == ROW) return true;
    if (puzzle[m][n].occupied == true || puzzle[m][n].num == -2) return solvePuzzle(puzzle, used, m, n + 1);
    if (n + 1 < COL && puzzle[m][n + 1].occupied == false && puzzle[m][n + 1].num != -2) {
        int copy1 = puzzle[m][n].num;
        int copy2 = puzzle[m][n + 1].num;
        for (int i = 0; i <= 6; i++) {
            for (int j = 0; j <= 6; j++) {
                if (!used[min(i, j)][max(i, j)]) {
                    if ((copy1 != -1 && copy1 != i) || (copy2 != -1 && copy2 != j)) continue;
                    
                    puzzle[m][n].num = i;
                    puzzle[m][n + 1].num = j;

                    puzzle[m][n].occupied = true;
                    puzzle[m][n + 1].occupied = true;
                    used[min(i, j)][max(i, j)] = true;
                    if (solvePuzzle(puzzle, used, m, n + 1)) return true;
                    else {
                        puzzle[m][n].occupied = false;
                        puzzle[m][n + 1].occupied = false;
                        used[min(i, j)][max(i, j)] = false;
                        puzzle[m][n].num = copy1;
                        puzzle[m][n + 1].num = copy2;
                    }
                }
            }
        }
    }
    if (m + 1 < ROW && puzzle[m + 1][n].occupied == false && puzzle[m + 1][n].num != -2) {
        int copy1 = puzzle[m][n].num;
        int copy2 = puzzle[m + 1][n].num;
        for (int i = 0; i <= 6; i++) {
            for (int j = 0; j <= 6; j++) {
                if (!used[min(i, j)][max(i, j)]) {
                    if ((copy1 != -1 && copy1 != i) || (copy2 != -1 && copy2 != j)) continue;

                    puzzle[m][n].num = i;
                    puzzle[m + 1][n].num = j;

                    puzzle[m][n].occupied = true;
                    puzzle[m + 1][n].occupied = true;
                    used[min(i, j)][max(i, j)] = true;
                    if (solvePuzzle(puzzle, used, m, n + 1)) return true;
                    else {
                        puzzle[m][n].occupied = false;
                        puzzle[m + 1][n].occupied = false;
                        used[min(i, j)][max(i, j)] = false;
                        puzzle[m][n].num = copy1;
                        puzzle[m + 1][n].num = copy2;
                    }
                }
            }
        }
    }
    return false;
}

/* ---------------------------------------------------------------------[<]-
Function: print
Synopsis: TODO
---------------------------------------------------------------------[>]-*/
void print(const int (*board)[COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int num = board[i][j];
            if (num == -2) {
                cout << "    ";
            } else if (num == -1) {
                cout << "[ .]";
            } else {
                cout << "[" << setw(2) << num << "]";
            }
        }
        cout << endl;
    }
    cout << endl;
}

/* ---------------------------------------------------------------------[<]-
Function: print
Synopsis: TODO
---------------------------------------------------------------------[>]-*/
void print(struct Cell (*puzzle)[COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            int num = puzzle[i][j].num;
            if (num == -2) {
                cout << "    ";
            } else if (num == -1) {
                cout << "[ .]";
            } else {
                cout << "[" << setw(2) << num << "]";
            }
        }
        cout << endl;
    }
    cout << endl;
}