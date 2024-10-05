#include "disjoint.h" // Assuming this is the correct header file
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <vector>
#include <map>
#include <set>
using namespace std;

#define talloc(type, num) (type *)malloc(sizeof(type) * (num))

class Superball
{
public:
  Superball(int argc, char **argv);
  int r;
  int c;
  int mss;
  int empty;
  vector<int> board;                  // stores the color or empty ('*' or '.') for each cell
  vector<int> goals;                  // stores the locations of the goals (r*c) either '.' or '*'
  vector<int> colors;                 // vector of 256 elements which are indexed by a letter for example colors[p], where p represents purple
  DisjointSetByRankWPC *disjointSets; // Disjoint set to handle unions
};

void usage(const char *s)
{
  fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
  if (s != NULL)
    fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5)
    usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0)
    usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0)
    usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0)
    usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < (int)strlen(argv[4]); i++)
  {
    if (!isalpha(argv[4][i]))
      usage("Colors must be distinct letters");
    if (!islower(argv[4][i]))
      usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0)
      usage("Duplicate color");
    colors[argv[4][i]] = 2 + i; // Assign distinct numeric values
  }

  disjointSets = new DisjointSetByRankWPC(r * c);

  board.resize(r * c);
  goals.resize(r * c, 0);
  empty = 0;

  for (i = 0; i < r; i++)
  {
    if (!(cin >> s))
    {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != (size_t)c)
    {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++)
    {
      char tile = s[j];
      int idx = i * c + j;

      // Check if the tile is valid (either empty, a goal, or a valid color)
      if (tile != '*' && tile != '.' && colors[tile] == 0 && colors[tolower(tile)] == 0)
      {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, tile);
        exit(1);
      }

      board[idx] = tolower(tile); // Store the lowercase version of the tile

      if (tile == '.' || tile == '*')
        empty++;
      if (isupper(tile) || tile == '*')
      {
        goals[idx] = 1;
        board[idx] = tolower(tile); // Always store as lowercase
      }
    }
  }
}

int main(int argc, char **argv)
{
  Superball *s = new Superball(argc, argv);

  // Iterate through each cell in the board
  for (int i = 0; i < s->r; i++)
  {
    for (int j = 0; j < s->c; j++)
    {
      int currentIdx = i * s->c + j;

      // Skip empty cells
      if (s->board[currentIdx] == '.' || s->board[currentIdx] == '*')
        continue;

      int directions[4][2] = {
          {0, 1},  // right
          {1, 0},  // down
          {0, -1}, // left
          {-1, 0}  // up
      };

      for (int d = 0; d < 4; d++)
      {
        int newRow = i + directions[d][0];
        int newCol = j + directions[d][1];

        if (newRow >= 0 && newRow < s->r && newCol >= 0 && newCol < s->c)
        {
          int neighborIdx = newRow * s->c + newCol;

          // Check if the neighboring cell has the same color
          if (s->board[neighborIdx] == s->board[currentIdx])
          {
            int rootCurrent = s->disjointSets->Find(currentIdx);
            int rootNeighbor = s->disjointSets->Find(neighborIdx);

            if (rootCurrent != rootNeighbor)
            {
              s->disjointSets->Union(rootCurrent, rootNeighbor);
            }
          }
        }
      }
    }
  }

  // Create a map to store the sizes of each disjoint set
  map<int, pair<int, char> > setInfo;  // Maps root -> (size, color)
  map<int, vector<int> > scoringCells; // Maps root -> list of scoring cell indices

  // Iterate through each cell in the board
  for (int i = 0; i < s->r; i++)
  {
    for (int j = 0; j < s->c; j++)
    {
      int currentIdx = i * s->c + j;

      if (s->board[currentIdx] == '.' || s->board[currentIdx] == '*')
      {
        continue;
      }

      int root = s->disjointSets->Find(currentIdx);

      setInfo[root].first++;
      char cellColor = s->board[currentIdx];

      if (setInfo[root].second == 0)
      {
        setInfo[root].second = cellColor;
      }

      if (s->goals[currentIdx] == 1)
      {
        scoringCells[root].push_back(currentIdx);
      }
    }
  }

  // Print out the disjoint sets
  printf("Scoring sets:\n");
  for (map<int, pair<int, char> >::iterator it = setInfo.begin(); it != setInfo.end(); ++it)
  {
    int root = it->first;
    int size = it->second.first;
    char color = it->second.second;

    if (size >= s->mss && !scoringCells[root].empty())
    {
      int cellIdx = scoringCells[root][0]; // Take the first goal as a representative
      int row = cellIdx / s->c;
      int col = cellIdx % s->c;
      printf("  Size: %2d  Char: %c  Scoring Cell: %d,%d\n", size, color, row, col);
    }
  }

  delete s;
  return 0;
}