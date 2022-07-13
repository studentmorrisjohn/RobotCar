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
    void simplifyRoute(char* path, int pathLength);
    char getCharEquivalent(int direction);
    char* getPath();
    char* sandwich(char* path, int pathLength, int b_index, char replacement);
    int hasB(char* path, int pathLength);
    bool compareChar(char* first, char* second);
  private:
    char* _path;
    int _pathLength;
};

#endif
