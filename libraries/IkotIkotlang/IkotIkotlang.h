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
    char getCharEquivalent(int);
    char* getPath();
  private:
    char _path[100];
    int _pathLength;
};

#endif
