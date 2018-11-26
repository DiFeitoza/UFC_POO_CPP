#ifndef FONE_H
#define FONE_H

#include <iostream>
using namespace std;

class Fone {
    string id;
    string number;
public:
    Fone(string id = "", string number = ""){
        this->id = id;
        this->number = number;
    }

    string getId(){ return id; }
    string getNumber(){ return number; }

    static bool validate(string number) {
        string data = "1234567890()- ";
        for(auto c : number)
            if(data.find(c) == string::npos)
                return false;
        return true;
    }
};

#endif