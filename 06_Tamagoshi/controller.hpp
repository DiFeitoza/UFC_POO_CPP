#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <fstream>
#include "pet.hpp"

using namespace std;

class Controller{
public:
    Pet pet;
    Controller();
    string shell(string line);
    void exec();
};

#endif