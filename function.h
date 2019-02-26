/// Function prototype: ===========================================================
int InitAllegroMountainPaths();
void CleanUpAllegro();
void printRow (const apmatrix<short> &m, int r);
short findMin (const apmatrix<short> &m);
short findMax(const apmatrix<short> &m);
float getGrayScale(float smallest, float largest, float n);
void drawMap(const apmatrix<short> &m);
unsigned int drawLowestElevPath (const apmatrix<short> &m, unsigned short startRow, ALLEGRO_COLOR color);
unsigned short indexOfLowestElevPath(const apmatrix<short> &m);




int InitAllegroMountainPaths()
///This function initialize allegro, create the display and initialize primitive addon.
{
    if(!al_init()) {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display) {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    if(!al_init_primitives_addon()){
        fprintf(stderr, "failed to initialize primitive addon!\n");
        return -1;
        }
    return 0;

}

void CleanUpAllegro()
///This function destroy allegro variables
{
    al_destroy_display(display);
}

short findMin(const apmatrix<short> &m)
/// This function finds minimum value in array MapData
{
    int smallest = m[0][0];
    for (int r=0; r<nRows ; r++ )
    {
        for (int c=1; c<nCols ; c++)
        {
            if (m[r][c] < smallest)
                smallest = m[r][c];
        }
    }
    return smallest;
}

short findMax(const apmatrix<short> &m)
/// This function finds maximum value in array MapData
{
    int largest = m[0][0];
    for (int r=0; r<nRows ; r++ )
    {
        for (int c=0; c<nCols ; c++)
        {
            if (m[r][c] > largest)
                largest = m[r][c];
        }
    }
    return largest;
}

float getGrayScale(float smallest, float largest, float n)
/// This function converts number n from range of numbers (smallest, largest element of 2D array[][]) to a grayscale (0, 255) and returns the result to the caller.
{
    /*
    For explanation, consider the following line:
    0--------------------50-----------------------100
    (min)               (number)                    (max)

    Our RBG scale
     0--------------------n-----------------------255
    (min)               (number)                    (max)

                        Now let 50 be our number. Solve for n.
    50 - 0                number - min        n - 0
    ______                    ______     = ________
    100 - 0               max - min       255 - 0

    Our Formula becomes (number- min)/(max - min)*n
    */
    float result = 255 * (n-smallest)/(largest - smallest);
    return result;
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
    al_flip_display();    /// this flip the display buffer to the screen.
}

unsigned int drawLowestElevPath (const apmatrix<short> &m, unsigned short currentRow, ALLEGRO_COLOR color)
///  1) it draws the path, 2) it calculates and returns to the total elevation change on that path.
{
    //total change of elevation within that path
    unsigned int totalChange = 0;
    //r is the index of our current row.
    unsigned short r = currentRow;
    //current column
    unsigned short c;
    //option of "flip coin"
    unsigned short option;

    /*
    Pretend that these are boxes of pixels that we use to choose our best path...

                                    [row - 1][column + 1]                           (upper box)
    [current row][current column]   [row][column + 1]       or...    (main box)     (middle box)
                                    [row + 1][column + 1]                           (lower box)

    */
    //for loop all index in the column to find the best horizontal path.
    for(c = 0; c < NUMCOLS-1; c++)
    {
        //special case when our current row is 0. No way of getting into the left box.
        if(r == 0)
        {
            al_draw_filled_rectangle(c, r, c+1, r+1, color);

            if(abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r+1][c+1]))  /// forward on the same row
            {
                totalChange += abs(m[r][c] - m[r][c+1]); // adds up elevation change from current to next column (same row).
                al_draw_filled_rectangle(c+1, r, c+2, r+1, color); // draws rectangle for next column, same row.
            }
            else /// forward down
                {
                    totalChange += abs(m[r][c] - m[r+1][c+1]); // adds up elevation change from current to next column lower row.
                    al_draw_filled_rectangle(c+1, r+1, c+2, r+2, color); // draws rectangle for next column, lower row.
                    r++;  // r increased by one to hold new value of lower row.
                }
        }

        else if(r == NUMROWS-1)   /// special case: startRow = NUMROWS-1. There will be no choice of forwarding down.
        {
            al_draw_filled_rectangle(c, r, c+1, r+1, color);

            if(abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r-1][c+1]))  /// forward, same row
            {
                totalChange += abs(m[r][c]- m[r][c+1]);
                al_draw_filled_rectangle(c+1, r, c+2, r+1, color);
             }
            else /// forward up
            {
                totalChange += abs(m[r][c] - m[r-1][c+1]);  // adds up elevation change from current to next column upper row.
                al_draw_filled_rectangle(c+1, r-1, c+2, r, color); //draws rectangle for next column, upper row.
                r--;   // r decreased by one to hold new value of the upper row.
            }
         }

          /// normal cases (4 cases)

           else if ((abs(m[r][c] - m[r+1][c+1]) < abs(m[r][c] - m[r][c+1])) &&
                (abs(m[r][c] - m[r+1][c+1]) < abs(m[r][c] - m[r-1][c+1])))  ///case 1: forward down
            {
                totalChange += abs(m[r][c] - m[r+1][c+1]);
                al_draw_filled_rectangle(c+1, r+1, c+2, r+2, color);
                r++;
            }
            else if(abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r+1][c+1]) &&
                    abs(m[r][c]- m[r][c+1]) <= abs(m[r][c] - m[r-1][c+1]))  /// case 2: forward,same row
            {
                totalChange += abs(m[r][c] - m[r][c+1]);
                al_draw_filled_rectangle(c+1, r, c+2, r+1, color);
            }
            else if( abs(m[r][c] - m[r-1][c+1]) < abs(m[r][c] - m[r][c+1]) &&
                     abs(m[r][c] - m[r-1][c+1]) < abs(m[r][c] - m[r+1][c+1])) /// case 3: forward up
            {
                totalChange += abs(m[r][c] - m[r-1][c+1]);
                al_draw_filled_rectangle(c+1, r-1, c+2, r, color);
                r--;
            }
            else if( abs(m[r][c] - m[r-1][c+1]) == abs(m[r][c] - m[r+1][c+1]) &&
                     abs(m[r][c] - m[r][c+1]) > abs(m[r][c] - m[r-1][c+1]))   /// case 5: forward up or forward down? flip a coin.
            {
                    srand (time(NULL));  /// Initiate random seed.
                    option = rand()% 2;   /// option will be 0 or 1 because rand number modulus 2 give remainder of 0 or 1.

                    if (option == 0)  ///  option = 0 is selected for forwarding down.
                    {
                        totalChange += abs(m[r][c] - m[r+1][c+1]);
                        al_draw_filled_rectangle(c+1, r+1, c+2, r+2, color);
                        r++;
                    }
                    else  /// option = 1 is forwarding up
                    {
                        totalChange += abs(m[r][c] - m[r-1][c+1]);
                        al_draw_filled_rectangle(c+1, r-1, c+2, r, color);
                        r--;
                    }

            }

            else;


     }
    al_flip_display();
    return totalChange;

}

unsigned short indexOfLowestElevPath(const apmatrix<short> &m)
/// It finds the overall lowest-elevation-change path and returns the index of the row it starts for that path.
{
    unsigned short r;   /// variable to hold the index of current row.
    unsigned int LowestTotalChange;
    unsigned short index;   /// index of the row

    //Just like finding the minimum value in an array, we always take the first element, pretending that it is the smallest value,
    // and compare it to the rest of the elements in a for loop.
    //We use the index that has the lowest number in the end.
    //The same method applies for two dimensional arrays below.
    LowestTotalChange = drawLowestElevPath (m, 0, RED);

    for (r=1 ; r < NUMROWS ; r++)
    {
        if(LowestTotalChange > drawLowestElevPath(m,r, RED))
        {
            LowestTotalChange = drawLowestElevPath(m,r, RED);
            index = r;
        }
        else;
    }

    return index;

}




