#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

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
using std::string; using std::stringstream;

void draw(vector<vector<string> > &map)
{
    for (auto it = map.rbegin(); it != map.rend(); ++it )
    {
        for (auto c : *it)
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

    // generate row labels
    stringstream row_labels;
    for (int i = 0; i < (size * scale); i += 3*scale)
    {
        row_labels << std::setw(6) << (std::to_string(x + i));
    }
    vector<string> temp_row_labels;
    temp_row_labels.push_back(row_labels.str());
    map.push_back(temp_row_labels);
    for (int i = 0; i < size; ++i)
    {
        stringstream row;
        // push back column axis labels
        if (i % 3 == 0)
        {
            double column_label = y + i * scale;
            row << std::setw(4) << std::setprecision(3) << column_label << " ";
        }
        else
            row << "     ";
        for (int j = 0; j < size; ++j)
        {
            // generate row by doing a check against coordinate information and
            // items
            row << " " << x + (j * scale) << " " << y + (i * scale);
        }
        vector<string> temp_row;
        temp_row.push_back(row.str());

        map.push_back(temp_row);
    }

    draw(map);
}

int main (void)
{
    generate(25, 2.0, -10, -10);

    std::cout << std::endl << std::endl;

    generate(25, 2.0, -20, -10);


}
