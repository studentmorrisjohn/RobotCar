#ifndef ikotikotlang
#define ikotikotlang

#if (ARDUINO > 100 )
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class IkotIkotlang {
  public:
    // Constructor
    IkotIkotlang();
    // Methods
    void addDirection(int direction);
    int getNextDirection();
    void simplifyRoute();
    char getCharEquivalent(int direction);
    char* getPath();
    int getPathLength();
    char getDirection(int index);
    char* sandwich();
    int hasB();
    bool compareChar(char* first, char* second);
    char getReplacement(char to_replace[3]);
  private:
    char _path[100];
    int _pathLength;
    int _b_index;
    int _currentDirectionIndex;
    char _replacement;
};

#endif
