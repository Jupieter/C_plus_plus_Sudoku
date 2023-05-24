#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

const int BOARD_SIZE = 9;
const int SUBGRID_SIZE = 3;

class SudokuGenerator {
    public:
        int board[BOARD_SIZE][BOARD_SIZE] = {};
        
        void Generate() {
            FillDiagonalSubgrids();
            FillRemainingCells(0, SUBGRID_SIZE);
            // PrintBoard();
        }
    
        void PrintBoard() {
            for (int row = 0; row < BOARD_SIZE; row++) {
                for (int col = 0; col < BOARD_SIZE; col++) {
                    cout << board[row][col] << " ";
                }
                cout << endl;
            }
        }
           
        void randomlyRemoveValues() {
            srand(time(0));
            for (int i = 0; i < 9; i++) {
                int count = rand() % 2 + 2;  // Number of random values: 2 or 3
                cout<<i <<" : " << count<<endl;
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
                num3 = rand() % 9;
                while (num3 == num1 || num3 == num2) {
                    num3 = rand() % 9;
                }
                zero_one[num3] = 1;
                cout<< num1<< " "<< num2<<" "<< num3<<endl;
                cout << "Lista elemei: ";
                for (int k = 0; k < zero_one.size(); k++) {
                    cout << zero_one[k] << " ";
                }
                cout << endl;
                // for (int j = 0; j < 9; j++) {
                //     switch (j) {
                //         case num1:
                //             j++;
                //         case num2:
                //             j++;
                //         case num3:
                //             j++;
                //         default:
                //             board[i][j] = 0;
                //     }
                // }
            }
        }

    private:

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
    SudokuGenerator sudoku;
    sudoku.Generate();
    sudoku.PrintBoard();
    sudoku.randomlyRemoveValues();
    int tabla[9][9] ={};
    for (int row=0; row<9; row++){
        for (int col = 0; col<9; col++){
            tabla[row][col] = sudoku.board[row][col] ;
        }
    };
    cout<< "----------------------------------"<< endl;
    for (int row = 0; row < BOARD_SIZE; row++) {
            for (int col = 0; col < BOARD_SIZE; col++) {
                cout << tabla[row][col] << " ";
            }
            cout << endl;
    };

    return 0;
}