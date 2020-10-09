#ifndef CELL
#define CELL

using namespace std;

class Cell {

    public:
        Cell(); // default constructor
        Cell(bool isActive); // overloaded constructor
        ~Cell(); // destructor

        void SetActive(bool isActive); // sets active
        void Activate(); // sets active to true
        void Deactivate(); // sets active to false
        bool IsActive(); // returns true if active
        char GetChar(); // returns 'X' if active and '-' if not
        bool Equals(Cell *c); // compares active value of another Cell

    private:
        bool active; // represents if the Cell is alive or not
};

#endif
