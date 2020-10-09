#include "FileSystem.h"

// default constructor
FileSystem::FileSystem() {}

// destructor
FileSystem::~FileSystem() {
    CloseInputFile();
    CloseOutputFile();
}

bool FileSystem::OpenInputFile(string inputFileName) {
    inputFile.open(inputFileName);
    return inputFile.is_open();
}

bool FileSystem::OpenOutputFile(string outputFileName) {
    outputFile.open(outputFileName);
    return outputFile.is_open();
}

void FileSystem::CloseInputFile() {
    inputFile.close();
}

void FileSystem::CloseOutputFile() {
    outputFile.close();
}

int FileSystem::GetHeight() {
    int height = 0;
    inputFile >> height;
    return height;
}

int FileSystem::GetWidth() {
    int width = 0;
    inputFile >> width;
    return width;
}

char** FileSystem::GetCharMap(int height, int width) {

    char **charMap = new char *[height];
    string charString;

    for(int i = 0; i < height; ++i) {
        inputFile >> charString;
        charMap[i] = new char[width];
        if(!inputFile.fail()) {
            for(int j = 0; j < charString.length(); ++j) {
                charMap[i][j] = charString[j];
            }
        }
    }
    return charMap;
}

void FileSystem::WriteLine(string line) {
    line = line + "\n";
    outputFile << line;
}
