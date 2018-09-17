#include <iostream>
#include <sstream>
#include "pet.hpp"
#include "controller.hpp"

using namespace std;

Pet::Pet(string nome, int energyMax, int hungryMax, int cleanMax):
    nome(nome), energyMax(energyMax), hungryMax(hungryMax), cleanMax(cleanMax),
    energy(energyMax), hungry(hungryMax), clean(cleanMax){
    this->age = 0;
    this->diamond = 0;
}

void Pet::status(int &status, int &statusMax, int value, bool &test){
    string stra;
    if(status + value <= 0){
        this->alive = false;
        if(status == energy)
            stra = "fraqueza";
        else if(status == hungry)
            stra = "fome";
        else if(status == clean)
            stra = "sujeira";
        cout << "  " << this->nome << " morreu de " << stra << ".";
        test = false;
        status = 0;
    }
    else if(status + value > statusMax)
        status = statusMax;
    else
        status = status + value;
}

Pet Pet::init(int e, int h, int c){
    Pet pet = Pet("pet", e, h, c);
    cout << "  Pet criado com sucesso!";
    return pet;
}

bool Pet::testAlive(){
    if(alive) return true;
    cout << "  Fail: " << this->nome << " esta morto!";
    return false;
}

bool Pet::play(){
    bool test = true;
    if(!testAlive()) return false;
    status(energy, energyMax, -2, test);
    status(hungry, hungryMax, -1, test);
    status(age, ageMax, +1, test);
    status(diamond, diamondMax, +1, test);
    return test;
}

bool Pet::eat(){
    bool test = true;
    if(!testAlive()) return false;
    status(energy, energyMax, -1, test);
    status(hungry, hungryMax, +4, test);
    status(clean, cleanMax, -2, test);
    status(age, ageMax, +1, test);
    return test;
}

bool Pet::shower(){
    bool test = true;
    if(!testAlive()) return false;
    status(energy, energyMax, -3, test);
    status(hungry, hungryMax, -1, test);
    status(age, ageMax, +2, test);
    status(clean, cleanMax, cleanMax, test);
    return test;
}

string Pet::sleep(){
    bool test = true;
    if(!testAlive()) return "";
    if(energyMax - energy >= 5){
        status(hungry, hungryMax, -1, test);
        status(age, ageMax, +1, test);
        status(energy, energyMax, energyMax, test);
        if(test)
            return "zZzZz.. que soneca boa!";
        else
            return "";
    } else
        return "Estou sem sono, vamos brincar?";;
}

void Pet::rename(string nome){
    this->nome = nome;
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