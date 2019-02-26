#include <time.h>
#include <iostream>
///We have to put the following line after iostream or else it will not work.
using namespace std;
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "apmatrix.h"

/// Constant & Global variables used in main and functions  =====================
extern const int SCREEN_W = 844; //if we put extern into const int, we have to assign it with a constant number or it will not work.
extern const int SCREEN_H = 480;
extern int nRows, nCols;         //if there is no const, extern will work fine without assigning it a value.
extern int minVal, maxVal;
extern unsigned short IndexOfLowest;

/// Using a 2D matrix for getting data from our file.
extern apmatrix<short> MapData;

 ///Allegro Variables =============================================================
ALLEGRO_DISPLAY *display = NULL;            /// defines the pointer to allegro display



/// Functions:===========================================================
int InitAllegroMountainPaths()
///This function initialize allegro, create the display and initialize primitive addon.
{
    if(!al_init())
    {
        fprintf(stderr, "failed to initialize allegro!\n");
        return -1;
    }

    display = al_create_display(SCREEN_W, SCREEN_H);
    if(!display)
    {
        fprintf(stderr, "failed to create display!\n");
        return -1;
    }

    if(!al_init_primitives_addon())
    {
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
