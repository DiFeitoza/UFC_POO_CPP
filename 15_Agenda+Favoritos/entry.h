#ifndef ENTRY_H
#define ENTRY_H

#include <iostream>

using namespace std;

class Entry {
    bool favorited = false;
public:
    Entry(){};
    virtual ~Entry(){};
    virtual string getId() = 0;
    void setFavorited(bool value){ this->favorited = value; }    
    bool isFavorited(){ return favorited; }
    virtual string toString() = 0;
};

#endif