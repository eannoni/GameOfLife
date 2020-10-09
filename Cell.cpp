#include "Cell.h"

// default constructor
Cell::Cell() {
    active = false;
}

// overloaded constructor
Cell::Cell(bool active) {
    this->active = active;
}

// destructor
Cell::~Cell() {}


// PUBLIC METHODS

// sets active
void Cell::SetActive(bool isActive) {
    active = isActive;
}

// sets active to true
void Cell::Activate() {
    active = true;
}

// sets active to false
void Cell::Deactivate() {
    active = false;
}

// returns true if active
bool Cell::IsActive() {
    return active;
}

// returns 'X' if active and '-' if not
char Cell::GetChar() {
    if(active) {
        return 'X';
    }
    return '-';
}

// compares active value of another Cell
bool Cell::Equals(Cell *c) {
    return (this->active == c->active);
}
