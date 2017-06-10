#include <iostream>
#include <vector>
#include <string>

/*
Example:

Time 0: Enter command: Display size: 25, scale: 2.00, origin: (-10.00, -10.00)
  38 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  32 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . Sh. . . . . . . . . . . . . . Va. . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  26 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . Xe. . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  20 . . . . . . . . . . . . . . . Be. . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  14 . . . . . . . . . . . . Aj. . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . Ex. . . . . . . . . . . . . .
   8 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
   2 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
  -4 . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
     . . . . . . . . . . . . . . . . . . . . . . . . .
 -10 . . . . . . . . . . . . . . . . . . . . . . . . .
   -10    -4     2     8    14    20    26    32    38
 */

/*
 * eldr:
 * - first 5 chars of each row are for axis values
 * - on bottom, the first number appears 6 chars in
 */

using std::vector;
using std::string;

void draw(vector<vector<string> > &map)
{
    for (auto row : map)
    {
        for (auto c : row)
            std::cout << c;
        std::cout << std::endl;
    }
}

/*
 * generate this map from the origin
 */
void generate (int size, double scale, int x, int y)
{
    string blank = ". ";
    vector<vector<string> > map;

    // push back row axis labels starting from sixth character
    string row_labels = "     ";
    for (int i = 0; i < (size * scale); i++)
    {
        row_labels += x + (i * scale);
        row_labels += "      "; // add six spaces
    }


    for (int i = 0; i < size; ++i)
    {
        vector<string> row;
        // push back mandatory column axis labels
        for (int j = 0; j < size; ++j)
        {
            // generate row by doing a check against coordinate information and
            // items
            row.push_back(blank);
        }
        map.push_back(row);
    }

    draw(map);
}

int main (void)
{
    int size = 25, x = -10, y = -10;
    double scale = 2.0;

    generate(size, scale, x, y);

}
