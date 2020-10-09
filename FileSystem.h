#ifndef FILESYSTEM
#define FILESYSTEM

#include <fstream>

using namespace std;

class FileSystem {

    public:
        FileSystem(); // default constructor
        ~FileSystem(); // destructor

        bool OpenInputFile(string inputFileName);
        bool OpenOutputFile(string outputFileName);
        void CloseInputFile();
        void CloseOutputFile();

        int GetHeight();
        int GetWidth();
        char** GetCharMap(int height, int width);

        void WriteLine(string line);

    private:
        ifstream inputFile;
        ofstream outputFile;
};

#endif
