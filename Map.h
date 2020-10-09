#ifndef MAP
#define MAP

#include "Cell.h"
#include "FileSystem.h"
#include <iostream>
#include <cmath>
#include <ctime> // for generating seed

using namespace std;

class Map {

    public:
        Map(); // default constructor
        Map(int height, int width); // overloaded constructor: shadow Map
        Map(int height, int width, float density); // overloaded constructor: randomized Map
        Map(int height, int width, char **charMap); // overloaded constructor: file Map
        Map(Map *p); // copy constructor
        ~Map(); // destructor

        static int GENERATION_COUNT;

        void SetMode(unsigned int mode); // sets game mode
        void Print(); // prints the generation count and the map contents using 'X's and '-'s
        void WriteToFile(FileSystem *fs); // writes output to file in FileSystem
        void UpdateMap(Map *shadowMap); // updates this map based off of shadowMap
        bool IsStabilized(Map *shadowMap); // returns true if the map is either unchanged or empty
        bool Equals(Map *m); // returns true if contents of each map are identical
        bool IsAlive(); // returns true if at least one cell in the map is active

    private:
        Cell ***map; // 2D array of pointers to Cells
        int numRows; // amount of rows in map
        int numCols; // amount of columns in map
        short mode; // 0: classic, 1: doughnut, 2: mirror

        void CreateMap(); // creates empty map of Cells

        void InitializeMap(float density); // initialize randomly by density (0-1)
        void InitializeMap(char **charMap); // initialize from pre-existing char map
        void InitializeMap(Map *p); // copy values from another map
};

#endif
