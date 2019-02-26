#include <time.h>
#include <iostream>
using namespace std; //We have to put the following line after iostream or else it will not work.
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "apmatrix.h"
#define RED al_map_rgb(255,0,0)
/// Constant & Global variables used in main and functions  =====================
extern const int NUMROWS = 480;
extern const int NUMCOLS = 844;
extern int nRows, nCols;         //if there is no const, extern will work fine without assigning it a value.
extern int minVal, maxVal;
extern unsigned short IndexOfLowest;
extern apmatrix<short> MapData; // Using a 2D matrix for getting data from our file.

/// Functions:===========================================================
float getGrayScale(float smallest, float largest, float n)
/// This function converts number n from range of numbers (smallest, largest element of 2D array[][]) to a grayscale (0, 255) and returns the result to the caller.
{
    /*For explanation, consider the following line:
    0--------------------50-----------------------100
    (min)               (number)                    (max)
    Our RBG scale
     0--------------------n-----------------------255
    (min)               (number)                    (max)
                        Now let 50 be our number. Solve for n.
    50 - 0                number - min        n - 0
    ______                    ______     = ________
    100 - 0               max - min       255 - 0
    Our Formula becomes (number- min)/(max - min)*n */
    return 255 * (n-smallest)/(largest - smallest);
}

void drawMap(const apmatrix<short> &m)
/// This function draw a rectangular of 1 X 1 pixel of the map on the grayscale (0, 255).
{
    unsigned short x;
    al_clear_to_color(al_map_rgb(0,0,0));

    for (int r=0; r<NUMROWS ; r++)
    {
        for (int c=0; c<NUMCOLS ; c++)
        {
            //x receives the value converted to the number between 0 to 255.
            x = getGrayScale(minVal, maxVal, m[r][c]);
            //Colour with r=x, b=x, g=x.
            ALLEGRO_COLOR color = al_map_rgb(x,x,x);
            //Draw a coloured box with one pixel
            al_draw_filled_rectangle(c, r, c+1, r+1, color);
        }
    }
    al_flip_display();
}

unsigned int drawLowestElevPath (const apmatrix<short> &m, unsigned short currentRow, ALLEGRO_COLOR color)
///  1) it draws the path, 2) calculates and returns to the total elevation change on that path.
{

    unsigned int totalChange = 0;   //total change of elevation within that path
    unsigned short r = currentRow;  //r is the index of our current row.
    unsigned short c;               //current column
    unsigned short option;          //option of "flip coin"
    /*Pretend that these are boxes of pixels that we use to choose our best path...
                                    [row - 1][column + 1]                           (upper box)
    [current row][current column]   [row][column + 1]       or...    (current box)     (middle box)
                                    [row + 1][column + 1]                           (lower box)*/
    //for loop all index in the column to find the best horizontal path.
    for(c = 0; c < NUMCOLS-1; c++)
    {
        //special case when our current row is 0. No way of getting into the left box.
        if(r == 0)
        {
            //make our current box coloured
            al_draw_filled_rectangle(c, r, c+1, r+1, color);
            /*if the difference between the current box and middle box is smaller or equal...
            to the difference between the current box and the lower box, then we go straight
            (choose to step onto the middle box)because it is the easier road:*/
            if(abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r+1][c+1]))  /// going foward
            {
                //Get elevation change between those two boxes
                totalChange += abs(m[r][c] - m[r][c+1]);
                //color fill in our chosen box
                al_draw_filled_rectangle(c+1, r, c+2, r+1, color);
            }
            /* if the difference between the current box and the lower box is smaller...*/
            else /// going down
            {
                //Get elevation change between those two boxes
                totalChange += abs(m[r][c] - m[r+1][c+1]);
                //color fill in our chosen box
                al_draw_filled_rectangle(c+1, r+1, c+2, r+2, color);
                r++;  // r increased by one to hold new value of lower row, which means now our path is on a new row.
            }
        }

        //Another special case: If our row is at the bottom, there's no choice of choosing the lower box.
        else if(r == NUMROWS-1)
        {
            //color fill our current box
            al_draw_filled_rectangle(c, r, c+1, r+1, color);
            ///same logic applies here that we go forward.
            if(abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r-1][c+1]))
            {
                totalChange += abs(m[r][c]- m[r][c+1]);
                al_draw_filled_rectangle(c+1, r, c+2, r+1, color);
            }
            ///if the difference between current box and upper box is smaller, then we choose our upper box.
            else
            {
                totalChange += abs(m[r][c] - m[r-1][c+1]);
                al_draw_filled_rectangle(c+1, r-1, c+2, r, color);
                r--;    //because we now choose to step onto the box in the upper row...
            }
        }
        /// normal cases (4 cases). SAME LOGIC APPLIES
        ///Case 1 = step onto the lower box because it is the best. (go forward down)
        else if ((abs(m[r][c] - m[r+1][c+1]) < abs(m[r][c] - m[r][c+1])) &&
                 (abs(m[r][c] - m[r+1][c+1]) < abs(m[r][c] - m[r-1][c+1])))
        {
            totalChange += abs(m[r][c] - m[r+1][c+1]);
            al_draw_filled_rectangle(c+1, r+1, c+2, r+2, color);
            r++;
        }
        ///Case 2 = step onto the middle box because it is the best. (go straight)
        else if(abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r+1][c+1]) &&
                abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r-1][c+1]))
        {
            totalChange += abs(m[r][c] - m[r][c+1]);
            al_draw_filled_rectangle(c+1, r, c+2, r+1, color);
        }
        ///Case 3 = step onto the upper box because it is the best (go up)
        else if( abs(m[r][c] - m[r-1][c+1]) < abs(m[r][c] - m[r][c+1]) &&
                 abs(m[r][c] - m[r-1][c+1]) < abs(m[r][c] - m[r+1][c+1]))
        {
            totalChange += abs(m[r][c] - m[r-1][c+1]);
            al_draw_filled_rectangle(c+1, r-1, c+2, r, color);
            r--;
        }
        ///Case 4 = Upper and Lower box have the same value, and the middle box isn't the best..flip a coin.
        else if( abs(m[r][c] - m[r-1][c+1]) == abs(m[r][c] - m[r+1][c+1]) &&
                 abs(m[r][c] - m[r][c+1]) > abs(m[r][c] - m[r-1][c+1]))
        {
            //our time seed
            srand (time(NULL));
            //get random number between 0 and 1.
            option = rand()% 2;
            //if 0 go down
            if (option == 0)
            {
                totalChange += abs(m[r][c] - m[r+1][c+1]);
                al_draw_filled_rectangle(c+1, r+1, c+2, r+2, color);
                r++;
            }
            // if 1 go up.
            else
            {
                totalChange += abs(m[r][c] - m[r-1][c+1]);
                al_draw_filled_rectangle(c+1, r-1, c+2, r, color);
                r--;
            }
        }
        else;
    }
    al_flip_display();
    return totalChange; //our total change in elevation
}

unsigned short indexOfLowestElevPath(const apmatrix<short> &m)
/// It finds the overall lowest-elevation-change path and returns the index of the row it starts for that path.
{

    unsigned short r;               // the index of current row.
    unsigned int LowestTotalChange; // lowest elevation change.
    unsigned short index;

    /*Just like finding the minimum value in an array, we always take the first element, pretending that it is the smallest value,
    and compare it to the rest of the elements in a for loop. We use the index that has the lowest number in the end.
    The same method applies for two dimensional arrays below. */
    LowestTotalChange = drawLowestElevPath (m, 0, RED);
    //for loop that calculates path for every single box/pixel of the rows.
    for (r=1 ; r < NUMROWS ; r++)
    {
        //color them red.
        if(LowestTotalChange > drawLowestElevPath(m,r, RED))
        {
            LowestTotalChange = drawLowestElevPath(m,r, RED);
            index = r;
        }
        else;
    }
    return index;
}

