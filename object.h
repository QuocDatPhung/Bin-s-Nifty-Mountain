
///Defines the RPG colors ======================================================

#define YELLOW   al_map_rgb(255,255,51)



/// Constant & Global variables used in main and functions  =====================
const int SCREEN_W = 844; // defines the width of the display
const int SCREEN_H = 480; // defines the height of the display
const int NUMROWS = 480;  // defines the number of rows of 2D array.
const int NUMCOLS = 844;  // defines the number of columns of 2D array.
int nRows, nCols;           // declare the variable to hold the value of number of row and column
int minVal, maxVal;         // declare the variable to hold the value of min element and max element of 2D array.
unsigned int totalElevChange;   // variable to hold total elevation change of a path.
unsigned short IndexOfLowest;   // variable to hold the index of the lowest elevation change path.

/// Using a 2D matrix for getting data from our file.
    apmatrix<short> MapData(NUMROWS,NUMCOLS);


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
