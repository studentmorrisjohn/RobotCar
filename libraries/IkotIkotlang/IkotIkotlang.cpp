#include "Arduino.h"
#include "IkotIkotlang.h"
#include <Directions.h>

IkotIkotlang::IkotIkotlang() {}

char IkotIkotlang::getCharEquivalent(int direction) {
    char directionChar = 'S';
    switch (direction) {
        case STRAIGHT:
            directionChar = 'S';
            break;
        case BACK:
            directionChar = 'B';
            break;
        case LEFT:
            directionChar = 'L';
            break;
        case RIGHT:
            directionChar = 'R';
            break;
    }
    return directionChar;
}

char* IkotIkotlang::getPath() {
    return _path;
}

void IkotIkotlang::addDirection(int direction) {
    char directionChar = getCharEquivalent(direction);
    
    _path[_pathLength] = directionChar;
    _pathLength ++;


}