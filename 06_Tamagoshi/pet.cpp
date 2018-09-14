#include <iostream>
#include <sstream>
#include "pet.hpp"

using namespace std;

Pet::Pet(string nome, int energyMax, int hungryMax, int cleanMax):
    nome(nome), energyMax(energyMax), hungryMax(hungryMax), cleanMax(cleanMax),
    energy(energyMax), hungry(hungryMax), clean(cleanMax){
    this->age = 0;
    this->diamond = 0;
}

void Pet::status(int &status, int &statusMax, int value){
    if(status + value <= 0){
        this->alive = false;
        status = 0;
    }
    else if(status + value > statusMax)
        status = statusMax;
    else
        status = status + value;
}

bool Pet::testAlive(){
    if(alive) return true;
    cout << "  Fail: " << this->nome << " esta morto!";
    return false;
}

bool Pet::play(){
    if(!testAlive()) return false;
    status(energy, energyMax, -2);
    status(hungry, hungryMax, -1);
    status(age, ageMax, +1);
    status(diamond, diamondMax, +1);
    return true;
}

bool Pet::eat(){
    if(!testAlive()) return false;
    status(energy, energyMax, -1);
    status(hungry, hungryMax, +4);
    status(clean, cleanMax, -2);
    status(age, ageMax, +1);
    return true;
}

bool Pet::shower(){
    if(!testAlive()) return false;
    status(energy, energyMax, -3);
    status(hungry, hungryMax, -1);
    status(age, ageMax, +2);
    status(clean, cleanMax, cleanMax);
    return true;
}

string Pet::sleep(){
    if(!testAlive()) return "";
    if(energyMax - energy >= 5){
        status(hungry, hungryMax, -1);
        status(age, ageMax, +1);
        status(energy, energyMax, energyMax);
        return "zZzZz.. que soneca boa!";
    } else
        return "Estou sem sono, vamos brincar?";;
}

string Pet::toString(){
    stringstream ss;
    ss << "N:" << nome << ", " 
        << "E:" << energy << "/" << energyMax << ", "
        << "H:" << hungry << "/" << hungryMax << ", "
        << "C:" << clean  << "/" << cleanMax  << ", "
        << "D:" << diamond << ", A:" << age;
    return ss.str();
}
