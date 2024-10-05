#include "disjoint.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
using namespace std;

#define talloc(type, num) (type *)malloc(sizeof(type) * (num))

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>

using namespace std;

class Superball
{
public:
  Superball(int argc, char **argv);
  int r, c, mss;
  vector<int> board;
  vector<int> goals;
  vector<int> colors;

  void findAndPrintMove();
};

Superball::Superball(int argc, char **argv)
{
  // Initialization code as in sb-analyze...
}

void Superball::findAndPrintMove()
{
  // First, try to find a scoring set
  // (You can reuse your disjoint set logic here to identify a scoring set)

  // If a scoring set is found
  // Example output for scoring:
  // cout << "SCORE " << row << " " << col << endl;

  // If no scoring sets are found, and empty cells are > 5, perform a SWAP
  // Find two adjacent non-empty cells to swap
  for (int i = 0; i < r; ++i)
  {
    for (int j = 0; j < c - 1; ++j)
    {
      if (board[i * c + j] != '.' && board[i * c + j + 1] != '.')
      {
        cout << "SWAP " << i << " " << j << " " << i << " " << j + 1 << endl;
        return;
      }
    }
  }
}

int main(int argc, char **argv)
{
  Superball *s = new Superball(argc, argv);
  s->findAndPrintMove();
  delete s;
  return 0;
}
