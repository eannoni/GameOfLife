#include "Map.h"

// default constructor
Map::Map() {
    numRows = 10;
    numCols = 10;
    mode = 0;
    CreateMap();
    InitializeMap(0.5);
}

// default overloaded constructor
Map::Map(int height, int width) {
    numRows = height;
    numCols = width;
    mode = 0;
    CreateMap();
}

// overloaded constructor: randomized Map
Map::Map(int height, int width, float density) {
    numRows = height;
    numCols = width;
    mode = 0;
    CreateMap();
    InitializeMap(density);
}

// overloaded constructor: file Map
Map::Map(int height, int width, char **charMap) {
    numRows = height;
    numCols = width;
    mode = 0;
    CreateMap();
    InitializeMap(charMap);
}

// copy constructor
Map::Map(Map *m) {
    numRows = m->numRows;
    numCols = m->numCols;
    mode = m->mode;
    CreateMap();
    InitializeMap(m);
}

// destructor
Map::~Map() {
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            delete map[i][j];
        }
        delete [] map[i];
    }
    delete [] map;
}


// PUBLIC METHODS

// sets game mode
void Map::SetMode(unsigned int mode) {
    this->mode = mode;
}

// prints the generation count and the map contents using 'X's and '-'s
void Map::Print() {
    cout << GENERATION_COUNT << endl;
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            cout << map[i][j]->GetChar();
        }
        cout << endl;
    }
}

// writes output to file in FileSystem
void Map::WriteToFile(FileSystem *fm) {
    string line = to_string(GENERATION_COUNT);
    fm->WriteLine(line);
    for(int i = 0; i < numRows; ++i) {
        line = "";
        for(int j = 0; j < numCols; ++j) {
            line += map[i][j]->GetChar();
        }
        fm->WriteLine(line);
    }
}

// updates this map based off of shadowMap
void Map::UpdateMap(Map *shadowMap) {

    // holds coordinate displacement pairs for the 8 possible neighboring cells of currCell
    short int yOffsets[] = { 1, 1, 1, 0, -1, -1, -1, 0 };
    short int xOffsets[] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    int activeCount = 0; // counts how many neighoring cells are active
    Cell *currCell = NULL; // cell being read from in map
    Cell *currShadowCell = NULL; // cell to change in shadowMap

    int yPos = 0; // y-value of cell that is being checked
    int xPos = 0; // x-value of cell that is being checked

    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            activeCount = 0; // reset activeCount
            currShadowCell = shadowMap->map[i][j];
            currCell = map[i][j];

            // check 8 neighboring cells
            for(int k = 0; k < 8; ++k) {
                yPos = i + yOffsets[k];
                xPos = j + xOffsets[k];
                // if out-of-bounds
                if((yPos < 0) || (yPos >= numRows)
                || (xPos < 0) || (xPos >= numCols)) {
                    if(mode == 1) { // Doughnut Mode
                        // if coordinate is out-of-bounds, wrap around to other side using modulus
                        yPos = (yPos + numRows) % numRows;
                        xPos = (xPos + numCols) % numCols;
                    } else if(mode == 2) { // Mirror Mode
                        // if coordinate is out-of-bounds, set yPos/xPos to reflect its neighbor
                        if(yPos == -1)
                            yPos++;
                        else if(yPos == numRows)
                            yPos--;
                        if(xPos == -1)
                            xPos++;
                        else if(xPos == numCols)
                            xPos--;
                    }
                    if(mode != 0) // Classic Mode ignores out-of-bounds regions
                        activeCount += shadowMap->map[yPos][xPos]->IsActive(); // add IsActive boolean result to activeCount
                } else { // if within bounds
                    activeCount += shadowMap->map[yPos][xPos]->IsActive(); // add IsActive boolean result to activeCount
                }
            }

            // decide fate of currCell
            // < 2 or > 3: die, == 3: born, == 2: no change
            if(activeCount > 3 || activeCount < 2)
                currCell->Deactivate();
            else if(activeCount == 3)
                currCell->Activate();
            else
                currCell->SetActive(currShadowCell->IsActive());
        }
    }
}

// returns true if the map is either unchanged or empty
bool Map::IsStabilized(Map *shadowMap) {
    return (!IsAlive() || Equals(shadowMap));
}

// returns true if contents of each map are identical
bool Map::Equals(Map *m) {
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            if(!(map[i][j]->Equals(m->map[i][j]))) {
                return false;
            }
        }
    }
    return true;
}

// returns true if at least one cell in the map is active
bool Map::IsAlive() {
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            if(map[i][j]->IsActive())
                return true;
        }
    }
    return false;
}

// PRIVATE METHODS

// creates empty map of Cells
void Map::CreateMap() {
    map = new Cell **[numRows];
    for(int i = 0; i < numRows; ++i) {
        map[i] = new Cell *[numCols];
        for(int j = 0; j < numCols; ++j) {
            map[i][j] = new Cell;
        }
    }
}

// initialize randomly by density (0-1)
void Map::InitializeMap(float density) {
    srand(time(0));
    float randomNum;
    int x;
    int y;
    // find the amount of elements needed to distribute randomly
    int numElements = (int) (density * (numRows * numCols));
    while(numElements != 0) {
        // generate random values for y and x within the bounds
        randomNum = ( (float) rand() / (float) RAND_MAX );
        int y = randomNum * numRows;
        randomNum = ( (float) rand() / (float) RAND_MAX );
        int x = randomNum * numCols;

        // if the cell isn't already active, activate and decrement numElements
        if(!map[y][x]->IsActive()) {
            map[y][x]->Activate();
            numElements--;
        }
    }
}

// initialize from pre-existing char map
void Map::InitializeMap(char **charMap) {
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            if(charMap[i][j] == 'X')
                map[i][j]->Activate();
            else
                map[i][j]->Deactivate();
        }
    }
}

// copy values from another map
void Map::InitializeMap(Map *m) {
    for(int i = 0; i < numRows; ++i) {
        for(int j = 0; j < numCols; ++j) {
            if(m->map[i][j]->IsActive())
                map[i][j]->Activate();
        }
    }
}
