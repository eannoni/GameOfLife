#include "Map.h"
#include "FileSystem.h"
#include <iostream>
#include <limits> // for max int size
#include <unistd.h> // for program sleep function

using namespace std;

// variables
int Map::GENERATION_COUNT = 0;
static const float WAIT_TIME_IN_MICROSECONDS = 100000.0;

// method declarations
static void SwapMapPointers(Map *&map, Map *&shadowMap);

template<typename T>
static T GetUserInput(T lowerBound, T upperBound);

static void OpenUserInputFile(FileSystem *fileSystem);

static void PerformWaitFunction(int viewMode);

/*------------------------------- MAIN METHOD -------------------------------*/

int main(int argc, char **argv) {

    Map *map;
    Map *shadowMap;
    string fileName = "";
    FileSystem *fileSystem = new FileSystem();

    int height = 0;
    int width = 0;

    int userInput = 0;
    int viewMode = 0; // 1: pause, 2: enter key, 3: write to file

    cout << "\n\n\n|------------------------| The Game of Life |------------------------|\n" << endl;

    cout << "How would you like to proceed? (Please enter the corresponding number)" << endl;
    cout << "\t1. Read map from file" << endl;
    cout << "\t2. Generate random map" << endl;
    userInput = GetUserInput(1, 2);

    if(userInput == 1) { // read map from file

        // get file name and open file
        OpenUserInputFile(fileSystem);

        // char map
        height = fileSystem->GetHeight();
        width = fileSystem->GetWidth();
        char **charMap = fileSystem->GetCharMap(height, width);
        fileSystem->CloseInputFile();

        // create map
        map = new Map(height, width, charMap);

        // clean up
        delete charMap;

    } else if(userInput == 2) { // generate random map
        // get map dimensions
        cout << "Enter map height: ";
        height = GetUserInput(1, numeric_limits<int>::max());
        cout << "Enter map width: ";
        width = GetUserInput(1, numeric_limits<int>::max());

        // get map density
        cout << "Enter density (0 - 1) value of map: ";
        float density = GetUserInput(0.0, 1.0);

        // create map
        map = new Map(height, width, density);
    }

    // prompt for boundary mode
    cout << "Select a boundary mode:" << endl;
    cout << "\t1. Classic Mode" << endl;
    cout << "\t2. Doughnut Mode" << endl;
    cout << "\t3. Mirror Mode" << endl;

    // set mode
    userInput = GetUserInput(1, 3);
    map->SetMode(userInput - 1);

    // create shadowMap
    shadowMap = new Map(map);

    // ask for brief pause, enter key, or write to file
    cout << "Select a viewing option: " << endl;
    cout << "\t1. Brief pause between generations" << endl;
    cout << "\t2. Press enter key to update generations" << endl;
    cout << "\t3. Write results to a file" << endl;
    viewMode = GetUserInput(1, 3);

    if(viewMode == 3) { // initialize file
        cout << "Input file name: ";
        cin >> fileName;
        fileSystem->OpenOutputFile(fileName); // ACCOUNT FOR EXCEPTIONS
        cout << "\nSimulating and writing to \'" << fileName << "\'..." << endl;
        cout << "If simulation does not complete within a few seconds, please enter \'Ctrl+C\' to manually terminate the program.\n" << endl;
    }

    // run until world stabilizes (empty or unchanging)
    bool isStabilized = false;
    while(!isStabilized) {
        // display map
        if(viewMode == 3) {
            map->WriteToFile(fileSystem);
        } else {
            map->Print();
        }
        // swap map pointers
        SwapMapPointers(map, shadowMap);
        // wait for set time/user input
        PerformWaitFunction(viewMode);
        // increment generation count
        Map::GENERATION_COUNT++;
        // calculate next generation
        map->UpdateMap(shadowMap);
        // check if map has stabilized since prev generation
        isStabilized = map->IsStabilized(shadowMap);
    }

    // clean-up
    delete map;
    delete shadowMap;
    delete fileSystem;

    // ask for user to press enter to quit
    cout << "Map has stabilized." << endl;
    if(viewMode == 3) {
        cout << "Successfully wrote to \'" << fileName << "\'." << endl;
    }
    cout << "Press Enter key to quit." << endl;
    cin.get();

    return 0;
}

/*----------------------------- EXTERNAL METHODS -----------------------------*/

static void SwapMapPointers(Map *&map, Map *&shadowMap) {
    Map *temp = map;
    map = shadowMap;
    shadowMap = temp;
}

template<typename T>
static T GetUserInput(T lowerBound, T upperBound) {
    T input;
    cin >> input;
    // while input is wrong type or is not within bounds
    while(!input || (input < lowerBound || input > upperBound)) {
        cout << "Invalid input. Please enter a number between " << lowerBound << " and " << upperBound << ": ";
        cin.clear(); // flush cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // ignore rest of input stream
        cin >> input;
        cin.ignore();
    }
    return input;
}

static void OpenUserInputFile(FileSystem *fileSystem) {
    string fileName = "";
    cout << "Please input name of .txt file: ";
    cin >> fileName;
    bool isOpen = fileSystem->OpenInputFile(fileName);
    while(!isOpen) {
        cout << "Unable to locate file \'" << fileName << "\'. Please input name of .txt file: ";
        cin >> fileName;
        isOpen = fileSystem->OpenInputFile(fileName);
    }
}

static void PerformWaitFunction(int viewMode) {
    if(viewMode == 1) { // brief pause
        usleep(WAIT_TIME_IN_MICROSECONDS);
    } else if(viewMode == 2) {
        cin.get();
    }
}
