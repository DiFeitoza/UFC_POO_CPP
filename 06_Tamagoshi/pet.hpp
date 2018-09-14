#ifndef PET_H
#define PET_H

using namespace std;

class Pet{
public:
    string nome;
    int energyMax, hungryMax, cleanMax, ageMax = 999, diamondMax = 999;
    int energy, hungry, clean;   
    int age; //{0}
    int diamond; //{0}
    bool alive {true};

    Pet(string nome, int energyMax, int hungryMax, int cleanMax);
    void status(int &status, int &statusMax, int value);
    bool testAlive();
    bool play();
    bool eat();
    bool shower();
    string sleep();
    string toString();
};

#endif