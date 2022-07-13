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

int IkotIkotlang::hasB(char* path, int pathLength) {
    for (int i = 0; i < pathLength; i++) {
        if (path[i] == 'B') {
            return i;
        }
    }

    return 0;
}

char* IkotIkotlang::sandwich(char* path, int pathLength, int b_index, char replacement) {
    Serial.println("Printing Path");

    char* sandwiched_path = path;

    // sandwiched_path[0] = 't';
    // Serial.println(sandwiched_path);

    for (int i = 0; i <= (b_index - 2); i++) {
        sandwiched_path[i] = path[i];
    }

    sandwiched_path[b_index - 1] = replacement;

    for (int i = b_index; i < (pathLength - 2); i++) {
        sandwiched_path[i] = path[i + 2];
    }

    

    return sandwiched_path;
}

bool IkotIkotlang::compareChar(char* first, char* second) {

    return (first[0] == second[0]) && (first[1] == second[1]) && (first[2] == second[2]);
}

void IkotIkotlang::simplifyRoute(char* path, int pathLength) {
    Serial.println("Tangina simplifying route");
    char* shortest_path = path;
    int shortest_path_length = pathLength;
    int b_index = hasB(shortest_path, shortest_path_length);    

    if (shortest_path_length < 3) {
        _path = shortest_path;
        _pathLength = shortest_path_length;
        return;
    }

    if (!b_index) {
        _path = shortest_path;
        _pathLength = shortest_path_length;
        return;
    } else {
        char to_replace[3] = {shortest_path[b_index - 1], shortest_path[b_index], shortest_path[b_index + 1]};
        char replacement;

        Serial.println(to_replace);


        if (compareChar(to_replace, (char*) "LBR")) {
            Serial.println("LBR yung pinalitan");
            replacement = 'B';
        } else if (compareChar(to_replace, (char*) "LBS")) {
            Serial.println("LBS yung pinalitan");
            replacement = 'R';
        } else if (compareChar(to_replace, (char*) "RBL")) {
            Serial.println("RBL yung pinalitan");
            replacement = 'B';
        } else if (compareChar(to_replace, (char*) "SBL")) {
            Serial.println("SBL yung pinalitan");
            replacement = 'R';
        } else if (compareChar(to_replace, (char*) "SBS")) {
            Serial.println("SBS yung pinalitan");
            replacement = 'B';
        } else if (compareChar(to_replace, (char*) "LBL")) {
            Serial.println("LBL yung pinalitan");
            replacement = 'S';
        } else {
            replacement = 'X';
        }



        shortest_path = sandwich(shortest_path, shortest_path_length, b_index, replacement);
        
    }

    _path = shortest_path;
    _pathLength = shortest_path_length - 2;

    Serial.println(_path);

    simplifyRoute(_path, _pathLength);

}