#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <unistd.h>

using namespace std;

const int BOARD_SIZE = 9;
const int SUBGRID_SIZE = 3;


// Class to generate and play Sudoku
class SudokuGenerator {
    public:
        int board[BOARD_SIZE][BOARD_SIZE] = {}; // Sudoku board
        int originalBoard[BOARD_SIZE][BOARD_SIZE] = {}; // Original Sudoku board
        
        // Generate a Sudoku game
        void Generate() {
            FillDiagonalSubgrids();
            FillRemainingCells(0, SUBGRID_SIZE);
            PrintBoard();
            randomlyRemoveValues();
            originalFill();
            PrintBoard();
        }
        // Print the Sudoku board
        void PrintBoard() {
            sleep(1);
            system("CLS");
            cout << "Welcome to Sudoku!" << endl;
            cout << "Enter 'i' to insert, 'd' to delete a value or 's' to autofill the board or 'q' to quit." << endl;
            cout << "Enter row, column, and value (separated by spaces) to fill a cell." << endl;
            cout << "---1-2-3---4-5-6---7-8-9--" << endl;
            cout << "--------------------------" << endl;
            for (int row = 0; row < BOARD_SIZE; row++) {
                cout << (row + 1);
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
                    cout << "--------------------------" << endl;
                }
            }
        }
        // Play the Sudoku game
        void Play() {
            char action;
            int row, col, value;

            while (true) {    
                cout << "Enter action: ";
                cin >> action;
                if (action != 'q' &&  action != 'd' && action != 'i' && action != 's' &&
                    action != 'Q' &&  action != 'D' && action != 'I' && action != 'S') {
                    cout << "Invalid action." << endl;
                        PrintBoard();
                }
                else if (action == 'q'|| action == 'Q') {
                    cout << "Quitting the game. Goodbye!" << endl;
                    break;
                }
                else if (action == 'i'|| action == 'I') {
                    cout << "Enter row, column, and value (separated by spaces): ";
                    cin >> row >> col >> value;
                    if (row >= 1 && row <= 9 && col >= 1 && col <= 9 && value >= 1 && value <= 9) {
                        std::cout << "The input data are valid." << std::endl;
                        row -=1, col -= 1;
                        if  (IsValidMove(row, col, value)) {
                            board[row][col] = value;
                            PrintBoard();
                        } 
                        else {
                            cout << "Invalid move. Try again." << endl;
                            PrintBoard();
                        }
                    } else {
                        std::cout << "Incorrect input data." << std::endl;
                        PrintBoard();
                    }
                }                 
                else if (action == 'd' || action == 'D') {
                    cout << "Enter row, column (separated by spaces): ";
                    cin >> row >> col;
                    if (row >= 1 && row <= 9 && col >= 1 && col <= 9) {
                        std::cout << "The input data are valid." << std::endl;
                        row -=1, col -= 1;
                        if (originalBoard[row][col] != 0) {
                            cout << "This number cannot be deleted because it is part of the original sudoku." << endl;
                            PrintBoard();
                        }
                        else {
                            cout << "The number deleted" << endl;
                            board[row][col] = 0;
                            PrintBoard();
                        }
                    } 
                }
                else if (action == 's'|| action == 'S') {
                    cout << "You have choosed the autofill." << endl;
                    sleep(1);
                    Solve();
                }
            }
        }
           
        

    private:

        void originalFill() {
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    originalBoard[i][j] = board[i][j];
                }
            }
        }
        void backFill() {
            for (int i = 0; i < 9; i++){
                for (int j = 0; j < 9; j++){
                    board[i][j] = originalBoard[i][j];
                }
            }
        }

        void randomlyRemoveValues() {
            srand(time(0));
            for (int i = 0; i < 9; i++) {
                int count = rand() % 2 + 2;  // Number of random values: 2 or 3
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

        // Fill the diagonal subgrids with random numbers
        void FillDiagonalSubgrids() {
            for (int i = 0; i < BOARD_SIZE; i += SUBGRID_SIZE) {
                FillSubgrid(i, i);
            }
        }

        // Fill a subgrid with shuffled numbers
        void FillSubgrid(int startRow, int startCol) {
            // Create a vector of numbers from 1 to 9
            vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
            shuffle(nums.begin(), nums.end(), random_device());

            int numIndex = 0;
            for (int row = 0; row < SUBGRID_SIZE; row++) {
                for (int col = 0; col < SUBGRID_SIZE; col++) {
                    // Assign the shuffled numbers to the subgrid cells
                    board[startRow + row][startCol + col] = nums[numIndex++];
                }
            }
        }


        // Fill the remaining cells of the Sudoku board using backtracking
        bool FillRemainingCells(int row, int col) {
            // Check if the column index is out of range and move to the next row
            if (col >= BOARD_SIZE && row < BOARD_SIZE - 1) {
                row++;
                col = 0;
            }
            // Check if all rows and columns are filled
            if (row >= BOARD_SIZE && col >= BOARD_SIZE) {
                return true;
            }
            // Determine the starting column for each subgrid
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
            // Try placing numbers in the cell
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

    // Check if it is safe to place a number in a specific cell
    bool IsSafe(int row, int col, int num) {
        return (IsRowSafe(row, num) && IsColumnSafe(col, num) && IsSubgridSafe(row - (row % SUBGRID_SIZE), col - (col % SUBGRID_SIZE), num));
        }

    // Check if a move is valid
    bool IsValidMove(int row, int col, int value) {
        if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE || value < 1 || value > BOARD_SIZE) {
            return false;
        }
        // Check if the cell is an original value (cannot be modified)
        if (originalBoard[row][col] != 0) {
            cout << "The entry cannot be modified!" << endl;
            return false;  
        }
        // Check if the value is already present in the row, column, or subgrid
        if (!IsRowSafe(row, value) || !IsColumnSafe(col, value) || !IsSubgridSafe(row - (row % SUBGRID_SIZE), col - (col % SUBGRID_SIZE), value)) {
            cout << "This number is already included in the row, column or sub-grid!" << endl;
            return false; 
        }

        return true;
    }

    // Check if a value is safe to place in a row
    bool IsRowSafe(int row, int num) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            if (board[row][col] == num) {
                return false;
            }
        }
        return true;
    }

    // Check if a value is safe to place in a column
    bool IsColumnSafe(int col, int num) {
        for (int row = 0; row < BOARD_SIZE; row++) {
            if (board[row][col] == num) {
                return false;
            }
        }
        return true;
    }

    // Check if a value is safe to place in a subgrid
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

    //Automatic solution of the game by the program
    void Solve() {
        backFill();
        if (SolveHelper(0, 0)) {
            cout << "Solution:" << endl;
            PrintBoard();
        } else {
            cout << "No solution found!" << endl;
        }
    }

    bool SolveHelper(int row, int col) {
        if (row == BOARD_SIZE) {
            return true;  // Solution found
        }

        if (col == BOARD_SIZE) {
            return SolveHelper(row + 1, 0);  // Go to the next row
        }

        if (board[row][col] != 0) {
            return SolveHelper(row, col + 1);  // Go to the next col
        }

        for (int num = 1; num <= BOARD_SIZE; num++) {
            if (IsSafe(row, col, num)) {
                board[row][col] = num;
                if (SolveHelper(row, col + 1)) {
                    return true;
                }
                board[row][col] = 0;  // Reset to 0 if the setting does not lead to a solution
            }
            // PrintBoard();
        }

        return false;  // No solution found
    }
 
};


int main() {
    SudokuGenerator sudoku;
    char actn;
    while (actn != 'q' || actn != 'g') {
        system("CLS");
        std::cout << "Welcome to Sudoku!" << std::endl;
        std::cout << "Enter 'g' to generate a sudoku game or 'q' to quit." << std::endl;
        std::cout << "Enter action: ";
        std::cin >> actn;
        if (actn == 'q' || actn == 'Q') {
                    cout << "Quitting the game. Goodbye!" << endl;
                    break;
                }
        if (actn == 'g' || actn == 'G'){
            sudoku.Generate();
            sudoku.Play();
        }
    }

    return 0;
}