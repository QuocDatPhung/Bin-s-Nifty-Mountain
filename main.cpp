/**************************************************************
Nifty mountain path

Student Name: Bin (Quoc Dat Phung)           Ms. Lindsay Cullum
Grade       : 11                            ICSU

                                            Due  :  Feb 25, 2019

    This program helps you find the easiest path with the lowest
    elevation in the Colorado mountains.

****************************************************************/

#include <time.h>

#include <iostream>
///We have to put the following line after iostream or else it will not work.
using namespace std;

#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "apmatrix.h"
#include "object.h"


int main()

{
    ///******************************INPUT********************************///


    ifstream inFile;

    ///Following lines open the file and load data from the file into 2D array MapData[][]
    inFile.open("Colorado_844x480.dat");
    //If file has an error
    if(!inFile)
        return false;
    //for loop to take in data
    for (int r=0 ; r<NUMROWS ; r++)
    {
        for(int c=0 ; c<NUMCOLS; c++)
        {
            inFile >> MapData[r][c];
        }
    }
    //Closing file
    inFile.close();

    /// Finding the number of rows and columns
    nRows = MapData.numrows();
    nCols = MapData.numcols();

    /// Finding the min and max values of our 2D array
    minVal = findMin(MapData);
    maxVal = findMax(MapData);

    ///******************************OUTPUT********************************///

    cout << "Number of rows: " << nRows << "\n";
    cout << "Number of columns: " << nCols << "\n";
    cout << "Minimum value: " << minVal << "\n";
    cout << "Maximum value: " << maxVal << "\n";

    ///Initialize Allegro
    InitAllegroMountainPaths();

    ///Draw Map
    drawMap(MapData);

    ///Getting the index of the best horizontal path
    IndexOfLowest = indexOfLowestElevPath(MapData);

    ///Following lines print out the index of lowest path and draw it in yellow.
    cout << "Lowest Elevation Change Path start on row: " << IndexOfLowest << "\n";
    cout << "Its total elevation change is: " << drawLowestElevPath(MapData,IndexOfLowest, YELLOW) << "\n";

    /// Show on screen for 5 seconds
    al_rest(5);

    ///Destroying display
    CleanUpAllegro();

    return 0;
}


