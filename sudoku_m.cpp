#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <unistd.h>

using namespace std;

const int BOARD_SIZE = 9;
const int SUBGRID_SIZE = 3;

class SudokuGenerator {
    public:
        int board[BOARD_SIZE][BOARD_SIZE] = {};
        int originalBoard[BOARD_SIZE][BOARD_SIZE] = {};
        
        void Generate() {
            FillDiagonalSubgrids();
            FillRemainingCells(0, SUBGRID_SIZE);
            PrintBoard();
            sleep(3);
            system("CLS");
            randomlyRemoveValues();
            PrintBoard();
        }
        void PrintBoard() {
            cout << "Welcome to Sudoku!" << endl;
            cout << "Enter 'i' to insert, 'd' to delete a value or 'q' to quit." << endl;
            cout << "Enter row, column, and value (separated by spaces) to fill a cell." << endl;
            cout << "--1-2-3---4-5-6---7-8-9--" << endl;
            cout << "-------------------------" << endl;
            for (int row = 0; row < BOARD_SIZE; row++) {
                for (int col = 0; col < BOARD_SIZE; col++) {
                    if (col % SUBGRID_SIZE == 0) {
                        cout << "| ";
                    }
                    if (board[row][col] != 0) {
                        cout << board[row][col] << " ";
                    } else {
                        cout << "  ";
                    }
                }
                cout << "|" << endl;
                if ((row + 1) % SUBGRID_SIZE == 0) {
                    cout << "-------------------------" << endl;
                }
            }
        }
        void PrintBoard_2() {
            for (int row = 0; row < BOARD_SIZE; row++) {
                for (int col = 0; col < BOARD_SIZE; col++) {
                    cout << board[row][col] << " ";
                }
                cout << endl;
            }
        }

        void Play() {
            

            char action;
            int row, col, value;

            while (true) {
                cout << "Enter action: ";
                cin >> action;

                if (action == 'q') {
                    cout << "Quitting the game. Goodbye!" << endl;
                    break;
                }
                cout << "Enter row, column, and value (separated by spaces): ";
                cin >> row >> col >> value;



                // if (IsValid(row, col, value)) {
                //     board[row][col] = value;
                //     PrintBoard();
                // } else if (action == 'd') {
                //     board[row][col] = 0;
                //     PrintBoard();
                // } else {
                //     cout << "Invalid move. Try again." << endl;
                // }
            }
        }
           
        

    private:

        void randomlyRemoveValues() {
            srand(time(0));
            for (int i = 0; i < 9; i++) {
                int count = rand() % 2 + 2;  // Number of random values: 2 or 3
                // cout<<i <<" : " << count<<endl;
                vector<int>zero_one(9, 0);
                int num1, num2, num3;
                // Generate three different random numbers
                num1 = rand() % 9;  // generate a number between 0-8
                zero_one[num1] = 1;
                num2 = rand() % 9;
                while (num2 == num1) {
                    num2 = rand() % 9;
                }
                zero_one[num2] = 1;
                if (count > 2) {
                num3 = rand() % 9;
                while (num3 == num1 || num3 == num2) {
                    num3 = rand() % 9;
                }
                zero_one[num3] = 1;
                }

                for (int j = 0; j < 9; j++) {
                    board[i][j] = board[i][j] * zero_one[j];
                }
            }
        }

        void FillDiagonalSubgrids() {
            for (int i = 0; i < BOARD_SIZE; i += SUBGRID_SIZE) {
                FillSubgrid(i, i);
            }
        }

        void FillSubgrid(int startRow, int startCol) {
            vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            shuffle(nums.begin(), nums.end(), random_device());

            int numIndex = 0;
            for (int row = 0; row < SUBGRID_SIZE; row++) {
                for (int col = 0; col < SUBGRID_SIZE; col++) {
                    board[startRow + row][startCol + col] = nums[numIndex++];
                }
            }
        }

        bool IsSafe(int row, int col, int num) {
        return (IsRowSafe(row, num) && IsColumnSafe(col, num) && IsSubgridSafe(row - (row % SUBGRID_SIZE), col - (col % SUBGRID_SIZE), num));
    }

    bool IsValidMove(int row, int col, int value) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || value < 1 || value > BOARD_SIZE) {
            return false;
        }

        if (originalBoard[row][col] != 0) {
            return false;  // A bejegyzés nem módosítható
        }

        if (!IsRowSafe(row, value) || !IsColumnSafe(col, value) || !IsSubgridSafe(row - (row % SUBGRID_SIZE), col - (col % SUBGRID_SIZE), value)) {
            return false;  // Az érvényes szám már szerepel a sorban, oszlopban vagy részrácsban
        }

        return true;
    }

    bool IsRowSafe(int row, int num) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == num) {
                return false;
            }
        }
        return true;
    }

    bool IsColumnSafe(int col, int num) {
        for (int row = 0; row < BOARD_SIZE; row++) {
            if (board[row][col] == num) {
                return false;
            }
        }
        return true;
    }

    bool IsSubgridSafe(int startRow, int startCol, int num) {
        for (int row = 0; row < SUBGRID_SIZE; row++) {
            for (int col = 0; col < SUBGRID_SIZE; col++) {
                if (board[startRow + row][startCol + col] == num) {
                    return false;
                }
            }
        }
        return true;
    }

    bool FillRemainingCells(int row, int col) {
        if (col >= BOARD_SIZE && row < BOARD_SIZE - 1) {
            row++;
            col = 0;
        }

        if (row >= BOARD_SIZE && col >= BOARD_SIZE) {
            return true;
        }

        if (row < SUBGRID_SIZE) {
            if (col < SUBGRID_SIZE) {
                col = SUBGRID_SIZE;
            }
        } else if (row < BOARD_SIZE - SUBGRID_SIZE) {
            if (col == static_cast<int>(row / SUBGRID_SIZE) * SUBGRID_SIZE) {
                col += SUBGRID_SIZE;
            }
        } else {
            if (col == BOARD_SIZE - SUBGRID_SIZE) {
                row++;
                col = 0;
                if (row >= BOARD_SIZE) {
                    return true;
                }
            }
        }

        for (int num = 1; num <= BOARD_SIZE; num++) {
            if (IsSafe(row, col, num)) {
                board[row][col] = num;
                if (FillRemainingCells(row, col + 1)) {
                    return true;
                }
                board[row][col] = 0;
            }
        }

        return false;
    }

 
};


int main() {
    system("CLS");
    SudokuGenerator sudoku;
    sudoku.Generate();
    sudoku.Play();

    return 0;
}