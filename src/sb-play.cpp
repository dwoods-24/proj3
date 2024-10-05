#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <map>
#include <set>
#include <queue>
#include <stack>

using namespace std;

class Superball
{
public:
  Superball(int argc, char **argv);
  void findAndPrintMove();
  void printBoard();
  bool checkForScoringSets();
  void swapCells(int r1, int c1, int r2, int c2);
  bool isGameOver();
  void dfs(int row, int col, char color, vector<vector<bool>> &visited, int &count, vector<pair<int, int>> &scoringCells);

private:
  int r, c, mss;
  vector<char> board; // Store color or empty ('.' or '*')
  vector<int> goals;  // Goal positions
  int empty;          // Number of empty cells
  int score;          // Total score
  char getCellColor(int row, int col);
};

Superball::Superball(int argc, char **argv)
{
  // Initialize game parameters from command line arguments
  if (argc != 5)
  {
    cerr << "Usage: sb-play rows cols min-score-size colors" << endl;
    exit(1);
  }

  sscanf(argv[1], "%d", &r);
  sscanf(argv[2], "%d", &c);
  sscanf(argv[3], "%d", &mss);
  score = 0;
  empty = r * c;

  board.resize(r * c);
  goals.resize(r * c, 0);

  // Initialize board with user input
  for (int i = 0; i < r; i++)
  {
    string row;
    cin >> row;
    for (int j = 0; j < c; j++)
    {
      board[i * c + j] = row[j];
      if (row[j] == '*' || row[j] == '.')
        empty++;
      if (isupper(row[j]))
      {
        goals[i * c + j] = 1; // Goal cell
      }
    }
  }
}

void Superball::printBoard()
{
  std::cout << "Empty Cells: " << empty << "     Score: " << score << endl;
  for (int i = 0; i < r; i++)
  {
    for (int j = 0; j < c; j++)
    {
      std::cout << board[i * c + j];
    }
    std::cout << endl;
  }
}

char Superball::getCellColor(int row, int col)
{
  if (row < 0 || row >= r || col < 0 || col >= c)
    return '\0'; // Out of bounds
  return board[row * c + col];
}

// Add this function to your class
void Superball::dfs(int row, int col, char color, vector<vector<bool>> &visited, int &count, vector<pair<int, int>> &scoringCells)
{
  // Check if the cell is out of bounds, already visited, or not the same color
  if (row < 0 || row >= r || col < 0 || col >= c || visited[row][col] || getCellColor(row, col) != color)
    return;

  // Mark the cell as visited
  visited[row][col] = true;
  count++;
  scoringCells.push_back(make_pair(row, col)); // Store the cell for scoring

  // Explore adjacent cells (up, down, left, right)
  dfs(row - 1, col, color, visited, count, scoringCells); // Up
  dfs(row + 1, col, color, visited, count, scoringCells); // Down
  dfs(row, col - 1, color, visited, count, scoringCells); // Left
  dfs(row, col + 1, color, visited, count, scoringCells); // Right
}

bool Superball::checkForScoringSets()
{
  vector<vector<bool>> visited(r, vector<bool>(c, false)); // To track visited cells
  bool scoringSetFound = false;

  // Iterate through each cell on the board
  for (int i = 0; i < r; ++i)
  {
    for (int j = 0; j < c; ++j)
    {
      // Check only if the cell is not visited and contains a color
      if (!visited[i][j] && isalpha(getCellColor(i, j)))
      {
        char color = getCellColor(i, j);
        int count = 0;
        vector<pair<int, int>> scoringCells; // To store the coordinates of the scoring cells

        // Perform DFS to find connected cells of the same color
        dfs(i, j, color, visited, count, scoringCells);

        // If we found 5 or more connected cells of the same color
        if (count >= 5)
        {
          scoringSetFound = true; // A scoring set has been found
          break;                  // No need to check further
        }
      }
    }
    if (scoringSetFound)
      break; // Exit outer loop as well
  }

  return scoringSetFound; // Return whether a scoring set was found
}

void Superball::swapCells(int r1, int c1, int r2, int c2)
{
  char temp = board[r1 * c + c1];
  board[r1 * c + c1] = board[r2 * c + c2];
  board[r2 * c + c2] = temp;
}

bool Superball::isGameOver()
{
  // Check if the game is over (e.g., no empty cells or no possible moves)
  return empty < 5; // Example condition
}

void Superball::findAndPrintMove()
{
  while (!isGameOver())
  {
    printBoard(); // Print the current state of the board

    // Ask the player for their action
    string action;
    std::cout << "\nChoose an action (SWAP or SCORE): ";
    while (cin >> action)
    {
      if (action == "SCORE" && checkForScoringSets())
      {
        // Step 1: Identify scoring sets and collect their cells
        vector<vector<bool>> visited(r, vector<bool>(c, false)); // To track visited cells
        int scoreGained = 0;                                     // Variable to track score gained in this turn

        for (int i = 0; i < r; ++i)
        {
          for (int j = 0; j < c; ++j)
          {
            // Check only unvisited color cells
            if (!visited[i][j] && isalpha(getCellColor(i, j)))
            {
              char color = getCellColor(i, j);
              int count = 0;
              vector<pair<int, int>> scoringCells; // To store the cells to be scored

              // Perform DFS to find connected cells of the same color
              dfs(i, j, color, visited, count, scoringCells);

              // If we found 5 or more connected cells of the same color, we score them
              if (count >= 5)
              {
                scoreGained += count; // Update score
                for (size_t k = 0; k < scoringCells.size(); ++k)
                {
                  // Remove scored cells from the board
                  int row = scoringCells[k].first;
                  int col = scoringCells[k].second;
                  board[row * c + col] = '.'; // Set to empty
                  empty++;                    // Update the count of empty cells
                }
              }
            }
          }
        }

        // Step 2: Update the total score
        score += scoreGained; // Assuming you have a `score` variable to keep track of the total score

        // Step 3: Output the results
        std::cout << "Score action performed. You scored " << scoreGained << " points!" << endl;
        std::cout << "Total score: " << score << endl;

        // Print the updated board (implement this function to display the board state)
        printBoard(); // Ensure you have a printBoard() function defined to show the current state of the board
      }
      else if (action == "SWAP")
      {
        int r1, c1, r2, c2;
        std::cout << "Enter the row and column of the first cell to swap (format: r1 c1): ";
        cin >> r1 >> c1;
        std::cout << "Enter the row and column of the second cell to swap (format: r2 c2): ";
        cin >> r2 >> c2;

        // Perform the swap
        swapCells(r1, c1, r2, c2);
        // Update emptyCells if necessary
        empty--; // Assuming a swap could lead to one less empty cell
      }
      else
      {
        std::cout << "Invalid action or no scoring sets available!" << endl;
      }
    }

    // Wait for user input before proceeding
    std::cout << "Type Return to continue..." << endl;
    cin.ignore(); // Ignore the newline character from previous input
    cin.get();    // Wait for user to press Enter
  }

  std::cout << "Game Over!" << endl;
}

int main(int argc, char **argv)
{
  Superball *s = new Superball(argc, argv);
  s->findAndPrintMove();
  delete s;
  return 0;
}