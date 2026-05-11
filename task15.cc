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
#include <algorithm>
using namespace std;
struct Cell {
    int num;
    bool occupied = false;
    int id = -1;
};
const int BOARD[8][9] = {
    {-2, -2, 0, 3, -1, 1, 5, -2, -2},
    {-2, -2, 2, 4, 3, 4, 6, -2, -2},
    {-1, 3, 6, 0, 5, 4, 6, 4, 4},
    {3, 6, 2, 3, 6, 6, 1, 5, 6},
    {3, 5, 1, 2, 0, 5, 2, 3, 6},
    {4, 2, 4, 1, 1, 3, 5, 2, 1},
    {-2, -2, 0, 1, 1, 2, 2, -2, -2},
    {-2, -2, 0, 5, -1, 4, 0, -2, -2},
};
const int ROW = sizeof(BOARD) / sizeof(BOARD[0]);
const int COL = sizeof(BOARD[0]) / sizeof(BOARD[0][0]);

void init(struct Cell (*puzzle)[COL]);
bool solve_puzzle(struct Cell (*puzzle)[COL], bool used[7][7], int m, int n, int &steps);
void user_solve(struct Cell (*puzzle)[COL], bool used[7][7]);
bool test_puzzle(struct Cell (*puzzle)[COL]);
int get_user_input(int min, int max);
int find_id(int i, int j);
int count_occupied(struct Cell (*puzzle)[COL]);
void print(struct Cell (*puzzle)[COL]);

/* ---------------------------------------------------------------------[<]-
Function: main
Synopsis: Displays the main menu and controls the program between the 
auto-solver with a timer and the manual game mode.
---------------------------------------------------------------------[>]-*/
int main() {
    Cell puzzle[ROW][COL];
    bool used[7][7]{false};
    int choice;
    
    cout << "Domino puzzle solver" << endl;
    cout << "------------------------------------" << endl;
    cout << "1. Start solver" << endl;
    cout << "2. Solve by yourself" << endl;
    cout << "3. Quit" << endl;
    
    while (true) {
        choice = get_user_input(1, 4);
        switch(choice) {
            case 1: {
                cout << "Start" << endl;
                init(puzzle);
                print(puzzle);
                for (int i = 0; i < 7; i++) {
                    for (int j = 0; j < 7; j++) {
                        used[i][j] = false;
                    }
                }
                int steps = 0;
                auto start = chrono::high_resolution_clock::now();
                if (solve_puzzle(puzzle, used, 0, 0, steps)) {
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
            case 2:
                user_solve(puzzle, used);
                break;
            case 3:
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
Function: init
Synopsis: Initializes the puzzle structure. Copies the layout from the 
constant source board, sets all cells to an unoccupied state and resets 
their domino id to prepare for a new game or auto-solver.
---------------------------------------------------------------------[>]-*/
void init(struct Cell (*puzzle)[COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            puzzle[i][j].num = BOARD[i][j];
            puzzle[i][j].occupied = false;
            puzzle[i][j].id = -1;
        }
    }
}

/* ---------------------------------------------------------------------[<]-
Function: solve_puzzle
Synopsis: Recursively attempts to place all 28 unique dominoes on the board 
using a backtracking algorithm. It fills empty cells, which are marked as -1, 
and checks both horizontal and vertical placements.
---------------------------------------------------------------------[>]-*/
bool solve_puzzle(struct Cell (*puzzle)[COL], bool used[7][7], int m, int n, int &steps) {
    steps++;
    if (n == COL) return solve_puzzle(puzzle, used, m + 1, 0, steps);
    if (m == ROW) return true;
    if (puzzle[m][n].occupied == true || puzzle[m][n].num == -2) return solve_puzzle(puzzle, used, m, n + 1, steps);
    if (n + 1 < COL && puzzle[m][n + 1].occupied == false && puzzle[m][n + 1].num != -2) {
        int copy1 = puzzle[m][n].num;
        int copy2 = puzzle[m][n + 1].num;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                int id = find_id(i, j);
                if (!used[min(i, j)][max(i, j)]) {
                    if ((copy1 != -1 && copy1 != i) || (copy2 != -1 && copy2 != j)) continue;
                    
                    puzzle[m][n].num = i;
                    puzzle[m][n + 1].num = j;
                    puzzle[m][n].occupied = true;
                    puzzle[m][n + 1].occupied = true;
                    puzzle[m][n].id = id;
                    puzzle[m][n + 1].id = id;
                    used[min(i, j)][max(i, j)] = true;
                    if (solve_puzzle(puzzle, used, m, n + 1, steps)) return true;
                    else {
                        puzzle[m][n].num = copy1;
                        puzzle[m][n + 1].num = copy2;
                        puzzle[m][n].occupied = false;
                        puzzle[m][n + 1].occupied = false;
                        puzzle[m][n].id = -1;
                        puzzle[m][n + 1].id = -1;
                        used[min(i, j)][max(i, j)] = false;
                    }
                }
            }
        }
    }
    if (m + 1 < ROW && puzzle[m + 1][n].occupied == false && puzzle[m + 1][n].num != -2) {
        int copy1 = puzzle[m][n].num;
        int copy2 = puzzle[m + 1][n].num;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                int id = find_id(i, j);
                if (!used[min(i, j)][max(i, j)]) {
                    if ((copy1 != -1 && copy1 != i) || (copy2 != -1 && copy2 != j)) continue;

                    puzzle[m][n].num = i;
                    puzzle[m + 1][n].num = j;
                    puzzle[m][n].occupied = true;
                    puzzle[m + 1][n].occupied = true;
                    puzzle[m][n].id = id;
                    puzzle[m + 1][n].id = id;
                    used[min(i, j)][max(i, j)] = true;
                    if (solve_puzzle(puzzle, used, m, n + 1, steps)) return true;
                    else {
                        puzzle[m][n].num = copy1;
                        puzzle[m + 1][n].num = copy2;
                        puzzle[m][n].occupied = false;
                        puzzle[m + 1][n].occupied = false;
                        puzzle[m][n].id = -1;
                        puzzle[m + 1][n].id = -1;
                        used[min(i, j)][max(i, j)] = false;
                    }
                }
            }
        }
    }
    return false;
}

/* ---------------------------------------------------------------------[<]-
Function: user_solve
Synopsis: Prompts user to input coordinates and missing numbers to place 
dominoes. When board is full, it triggers final validation
---------------------------------------------------------------------[>]-*/
void user_solve(struct Cell (*puzzle)[COL], bool used[7][7]) {
    init(puzzle);
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            used[i][j] = false;
        }
    }

    int r1, c1, r2, c2;
    int n1, n2;
    while (true) {
        print(puzzle);
        cout << "Enter row of first number (or 0 to exit): ";
        r1 = get_user_input(0, ROW);
        if (r1 == 0) break;
        cout << "Enter col of first number: ";
        c1 = get_user_input(1, COL);
        cout << "Enter row of second number: ";
        r2 = get_user_input(1, ROW);
        cout << "Enter col of second number: ";
        c2 = get_user_input(1, COL);

        r1--; c1--;
        if (puzzle[r1][c1].num == -2 || puzzle[r1][c1].occupied == true) {
            cout << "Error, this cell (" << r1++ << "," << c1++ << ") is not for playing" << endl;
            continue;
        }

        r2--; c2--;
        if (puzzle[r2][c2].num == -2 || puzzle[r2][c2].occupied == true) {
            cout << "Error, this cell (" << r2++ << "," << c2++ << ") is not for playing" << endl;
            continue;
        }

        if (abs(r1 - r2) + abs(c1 - c2) != 1) {
            cout << "Error, this cells (" << r1 << "," << c2 << "), (" << r2 << "," << c2 << ") must be neighbours" << endl;
            continue;
        }

        if (puzzle[r1][c1].num != -1) n1 = puzzle[r1][c1].num;
        else {
            cout << "Enter the number on first cell";
            n1 = get_user_input(0, 6);
        }
        

        if (puzzle[r2][c2].num != -1) n2 = puzzle[r2][c2].num;
        else {
            cout << "Enter the number on second cell";
            n2 = get_user_input(0, 6);
        }

        if (used[min(n1, n2)][max(n1, n2)]) {
            cout << "Error, this domino is already used";
            continue;
        }

        int id = find_id(n1, n2);
        puzzle[r1][c1].occupied = true;
        puzzle[r2][c2].occupied = true;
        puzzle[r1][c1].num = n1;
        puzzle[r2][c2].num = n2;
        puzzle[r1][c1].id = id;
        puzzle[r2][c2].id = id;
        used[min(n1, n2)][max(n1, n2)] = true;

        cout << "Successfully placed the domino" << endl;

        if (count_occupied(puzzle) == 56) {
            if (test_puzzle(puzzle)) {
                print(puzzle);
                cout << "You solved the puzzle!" << endl;
                break;
            } else {
                cout << "The board is full, but there some errors in your solution" << endl;

            }
        }
    }
}

/* ---------------------------------------------------------------------[<]-
Function: test_puzzle
Synopsis: Validates the solved puzzle by checking full board coverage, 
domino uniqueness and adjacency rules 
---------------------------------------------------------------------[>]-*/
bool test_puzzle(struct Cell (*puzzle)[COL]) {
    bool coverage = true;
    bool unique = true;
    bool neighbours = true;

    int counts[28]{0};
    bool used_id[28]{false};

    if (count_occupied(puzzle) != 56) coverage = false;

    for (int i1 = 0; i1 < ROW; i1++) {
        for (int j1 = 0; j1 < COL; j1++) {
            Cell cell = puzzle[i1][j1];

            if (cell.occupied == false || cell.id == -1 || used_id[cell.id] == true) {
                continue;
            }

            int id = cell.id;
            int v1 = cell.num;

            for (int i2 = 0; i2 < ROW; i2++) {
                for (int j2 = 0; j2 < COL; j2++) {
                    if (puzzle[i2][j2].id == id && (i2 != i1 || j2 != j1)) {
                        int v2 = puzzle[i2][j2].num;

                        if (abs(i1 - i2) + abs(j1 - j2) != 1) {
                            neighbours = false;
                        }

                        int idx = find_id(v1, v2);

                        counts[idx]++;
                        used_id[id] = true;
                        break;
                    }
                }
            }
        }
    }

    for (int i = 0; i < 28; i++) {
        if (counts[i] != 1) unique = false;
    }

    cout << "Coverage: " << (coverage ? "all right" : "not all right") << endl;
    cout << "unique: " << (unique ? "all right" : "not all right") << endl;
    cout << "neighbours: " << (neighbours ? "all right" : "not all right") << endl;

    if (coverage && unique && neighbours) {
        cout << "All correct!" << endl;
        return true;
    } else {
        cout << "Something is not ok, try again" << endl;
        return false;
    }
}

/* ---------------------------------------------------------------------[<]-
Function: get_user_input
Synopsis: Safely reads an integer from input within a specified range. 
Handles input stream failures by clearing the error flag and ignoring 
invalid characters
---------------------------------------------------------------------[>]-*/
int get_user_input(int min, int max) {
    int num = 0;
    while (true) {
        cin >> num;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Error, you need to enter an integer" << endl;
            continue;
        }
        if (num >= min && num <= max) return num;
        cout << "Number must be from " << min << " to " << max << endl; 
    }
    return num;
}

/* ---------------------------------------------------------------------[<]-
Function: find_id
Synopsis: Computes a unique index for any given domino pair using 
an arithmetic progression formula.
---------------------------------------------------------------------[>]-*/
int find_id(int i, int j) {
    int minV = min(i, j);
    int maxV = max(i, j);
    int idx = (minV * (15 - minV)) / 2 + (maxV - minV);
    return idx;
}

/* ---------------------------------------------------------------------[<]-
Function: count_occupied
Synopsis: Iterates through the entire matrix and calculates the total 
number of cells which are marked as occupied.
---------------------------------------------------------------------[>]-*/
int count_occupied(struct Cell (*puzzle)[COL]) {
    int count = 0;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (puzzle[i][j].occupied == true) count++;
        }
    }
    return count;
}

/* ---------------------------------------------------------------------[<]-
Function: print
Synopsis: Outputs the board and highlights placed dominoes with \033 code 
for green color and marks missing numbers for clear navigation
---------------------------------------------------------------------[>]-*/
void print(struct Cell (*puzzle)[COL]) {
    cout << "    ";
    for (int i = 1; i <= COL; i++) {
        cout << setw(3) << i << " ";
    }
    cout << "\n    ";
    for (int i = 0; i < COL * 4; i++) {
        cout << "-";
    }
    cout << endl;

    for (int i = 0; i < ROW; i++) {
        cout << setw(2) << i + 1 << " |";

        for (int j = 0; j < COL; j++) {
            int num = puzzle[i][j].num;
            bool occupied = puzzle[i][j].occupied;

            if (num == -2) {
                cout << "    ";
            } else if (occupied) {
                cout << "\033[1;32m[" << setw(2) << num << "]\033[0m";
            } else {
                if (num == -1) cout << "  ? ";
                else cout << "  " << num << " ";
            }
        }
        cout << endl;
    }
    cout << "    ";
    for (int i = 0; i < COL * 4; i++) {
        cout << "-";
    }
    cout << endl;
}