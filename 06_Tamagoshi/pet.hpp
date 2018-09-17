#ifndef PET_H
#define PET_H

#include <iostream>
using namespace std;

class Pet{
    public:
        Pet(string nome, int energyMax, int hungryMax, int cleanMax);
        void status(int &status, int &statusMax, int value, bool &test);
        Pet init(int e, int h, int c);
        bool testAlive();
        bool play();
        bool eat();
        bool shower();
        void rename(string nome);
        string sleep();
        string toString();
    private:
        string nome;
        int energyMax, hungryMax, cleanMax, ageMax = 999, diamondMax = 999;
        int energy, hungry, clean;   
        int age; //{0}
        int diamond; //{0}
        bool alive {true};
};

#endif