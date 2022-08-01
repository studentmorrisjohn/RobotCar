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

int IkotIkotlang::getPathLength() {
    return _pathLength;
}

int IkotIkotlang::getNextDirection() {
    if (_currentDirectionIndex > _pathLength) {
        _currentDirectionIndex = 0;
    }

    char direction_char = _path[_currentDirectionIndex];
    int direction = 0;

    switch (direction_char)
    {
    case 'S':
        direction = STRAIGHT;
        break;
    
    case 'B':
        direction = BACK;
        break;

    case 'L':
        direction = LEFT;
        break;

    case 'R':
        direction = RIGHT;
        break;
    
    default:
        direction = STRAIGHT;
        break;
    }

    _currentDirectionIndex ++;

    return direction;
}

char IkotIkotlang::getDirection(int index) {
    return _path[index];
}

void IkotIkotlang::addDirection(int direction) {
    char directionChar = getCharEquivalent(direction);
    
    _path[_pathLength] = directionChar;
    _pathLength ++;


}

int IkotIkotlang::hasB() {
    for (int i = 0; i < _pathLength; i++) {
        if (_path[i] == 'B') {
            return i;
        }
    }

    return 0;
}

char* IkotIkotlang::sandwich() {

    _path[_b_index - 1] = _replacement;

    for (int i = _b_index; i < (_pathLength - 2); i++) {
        _path[i] = _path[i + 2];
    }

    for (int i = _pathLength - 2; i < _pathLength; i++) {
        _path[i] = 'X';
    }
}

bool IkotIkotlang::compareChar(char* first, char* second) {
    return (first[0] == second[0]) && (first[1] == second[1]) && (first[2] == second[2]);
}

char IkotIkotlang::getReplacement(char to_replace[3]) {
    if (compareChar(to_replace, (char*) "LBR")) {
        Serial.println("LBR yung pinalitan");
        _replacement = 'B';
    } else if (compareChar(to_replace, (char*) "LBS")) {
        Serial.println("LBS yung pinalitan");
        _replacement = 'R';
    } else if (compareChar(to_replace, (char*) "RBL")) {
        Serial.println("RBL yung pinalitan");
        _replacement = 'B';
    } else if (compareChar(to_replace, (char*) "SBL")) {
        Serial.println("SBL yung pinalitan");
        _replacement = 'R';
    } else if (compareChar(to_replace, (char*) "SBS")) {
        Serial.println("SBS yung pinalitan");
        _replacement = 'B';
    } else if (compareChar(to_replace, (char*) "LBL")) {
        Serial.println("LBL yung pinalitan");
        _replacement = 'S';
    } else {
        _replacement = 'X';
    }
}

void IkotIkotlang::simplifyRoute() {
    Serial.println("simplifying route");
    _b_index =  hasB();    
    Serial.println(_path);

    if (_pathLength < 3) {
        return;
    }

    if (!_b_index) {
        return;
    } 

    char to_replace[3] = {_path[_b_index - 1], _path[_b_index], _path[_b_index + 1]};
    getReplacement(to_replace);
    sandwich();
    _pathLength -= 2;

    simplifyRoute();

}

