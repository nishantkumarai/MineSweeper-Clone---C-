#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class Cell {
public:
    bool isMine;
    bool isRevealed;
    int adjacentMines;

    Cell() : isMine(false), isRevealed(false), adjacentMines(0) {}
};

class Board {
private:
    int size;
    int mineCount;
    vector<vector<Cell>> cells;

    
    void placeMines(int initialX, int initialY) {
        srand(time(0));
        mineCount = size * size / 6; // Arbitrary choice of mines
        int placedMines = 0;

        while (placedMines < mineCount) {
            int x = rand() % size;
            int y = rand() % size;
            if (!cells[x][y].isMine && (x != initialX || y != initialY)) {
                cells[x][y].isMine = true;
                placedMines++;
            }
        }

        calculateAdjacentMines();
    }

    
    void calculateAdjacentMines() {
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                if (cells[x][y].isMine) continue;
                int mineCount = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        int nx = x + dx, ny = y + dy;
                        if (isInBounds(nx, ny) && cells[nx][ny].isMine) {
                            mineCount++;
                        }
                    }
                }
                cells[x][y].adjacentMines = mineCount;
            }
        }
    }

    // Check if coordinates are within bounds
    bool isInBounds(int x, int y) const {
        return x >= 0 && x < size && y >= 0 && y < size;
    }

public:
    Board(int n) : size(n), cells(n, vector<Cell>(n)) {}

    void initializeBoard(int initialX, int initialY) {
        placeMines(initialX, initialY);
    }

    // Reveal a cell
    bool revealCell(int x, int y) {
        if (!isInBounds(x, y) || cells[x][y].isRevealed) return true;
        cells[x][y].isRevealed = true;
        if (cells[x][y].isMine) return false;

        // Reveal adjacent cells if there are no nearby mines
        if (cells[x][y].adjacentMines == 0) {
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx, ny = y + dy;
                    revealCell(nx, ny);
                }
            }
        }
        return true;
    }

    
    bool isWin() const {
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                if (!cells[x][y].isMine && !cells[x][y].isRevealed) {
                    return false;
                }
            }
        }
        return true;
    }

    
    void displayBoard(bool revealAll = false) const {
        cout << "   ";
        for (int i = 0; i < size; i++) cout << i << " ";
        cout << endl;

        for (int x = 0; x < size; x++) {
            cout << x << " |";
            for (int y = 0; y < size; y++) {
                if (revealAll && cells[x][y].isMine) {
                    cout << "* ";
                } else if (cells[x][y].isRevealed) {
                    cout << cells[x][y].adjacentMines << " ";
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }
};

class MineSweeper {
private:
    Board board;
    bool isFirstMove;

public:
    MineSweeper(int size) : board(size), isFirstMove(true) {}

    void play() {
        int x, y;
        while (true) {
            board.displayBoard();
            cout << "Enter your move (x y): ";
            cin >> x >> y;

            if (isFirstMove) {
                board.initializeBoard(x, y);
                isFirstMove = false;
            }

            if (!board.revealCell(x, y)) {
                cout << "You hit a mine! Game over." << endl;
                board.displayBoard(true);
                break;
            }

            if (board.isWin()) {
                cout << "Congratulations! You've won the game!" << endl;
                board.displayBoard(true);
                break;
            }
        }
    }
};

int main() {
    int boardSize;
    cout << "Welcome to Minesweeper!" << endl;
    cout << "Enter board size (n >= 9): ";
    cin >> boardSize;

    if (boardSize < 9) {
        cout << "Invalid board size. Exiting." << endl;
        return 1;
    }

    MineSweeper game(boardSize);
    game.play();

    return 0;
}
