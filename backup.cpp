#include <iostream>
#include <fstream>
#include <sstream>
#include <clocale> // acentuação ptbr
#include "pet.hpp"
#include "controller.hpp"
using namespace std;

class Pet{
    public:
    string nome;
    int energyMax, hungryMax, cleanMax, ageMax = 999, diamondMax = 999;
    int energy, hungry, clean;   
    int age; //{0}
    int diamond; //{0}
    bool alive {true};

    Pet(string nome, int energyMax, int hungryMax, int cleanMax):
    nome(nome), energyMax(energyMax), hungryMax(hungryMax), cleanMax(cleanMax),
    energy(energyMax), hungry(hungryMax), clean(cleanMax)
    {
    this->age = 0;
    this->diamond = 0;
    }

    void status(int &status, int &statusMax, int value){
        if(status + value <= 0){
            this->alive = false;
            status = 0;
        }
        else if(status + value > statusMax)
            status = statusMax;
        else
            status = status + value;
    }

    bool testAlive(){
        if(alive)
            return true;
        cout << "  fail: " << this->nome << " esta morto!";
        return false;
    }

    bool play(){
        if(!testAlive()) return false;
        status(energy, energyMax, -2);
        status(hungry, hungryMax, -1);
        status(age, ageMax, +1);
        status(diamond, diamondMax, +1);
        return true;
    }

    bool eat(){
        if(!testAlive()) return false;
        status(energy, energyMax, -1);
        status(hungry, hungryMax, +4);
        status(clean, cleanMax, -2);
        status(age, ageMax, +1);
        return true;
    }

    bool shower(){
        if(!testAlive()) return false;
        status(energy, energyMax, -3);
        status(hungry, hungryMax, -1);
        status(age, ageMax, +2);
        status(clean, cleanMax, cleanMax);
        return true;
    }

    bool sleep(){
        if(!testAlive()) return false;
        // if dif > 5
        status(hungry, hungryMax, -1);
        status(age, ageMax, +1);
        status(energy, energyMax, energyMax);
        return true;
    }

    string toString(){
        stringstream ss;
        ss << "N:" << nome << ", " 
           << "E:" << energy << "/" << energyMax << ", "
           << "H:" << hungry << "/" << hungryMax << ", "
           << "C:" << clean  << "/" << cleanMax  << ", "
           << "D:" << diamond << ", A:" << age;
        return ss.str();
    }
};

struct Controller{
    Pet pet;

    Controller():
        pet("pet", 15, 10, 30){
    };

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;

        if(op == "show"){
            out << pet.toString();
        }
        else if(op == "rename"){
            in >> pet.nome;
        }
        else if(op == "eat"){
            if(pet.eat())
                out << "Yumyum, pancinha cheia!";
        }
        else if(op == "play"){
            if(pet.play())
                out << "dancei, brinquei, corri, pulei!";
        }
        else if(op == "shower"){
            if(pet.shower())
                out << "Yuuupie, estou cheirosinho!";
        }
        else if(op == "sleep"){
            if(pet.sleep())
                out << "zZzZz.. que soneca boa!";
        }
        // else if(op == "end")
        else if(op == "help"){
            out << "show: exibe status do pet \n  "
                << "eat: alimenta o pet \n  "
                << "play: bricar com o pet \n  "
                << "shower: dar banho no pet \n  "
                << "sleep: colocar o pet para descansar \n  "
                << "end: sair da execucao atual \n  "
                << "*Dica: para execucao manual basta adicionar 'manual' em uma das linha do contao.txt";
        }        

        return out.str();
    }

    void exec(){
        ifstream arquivo ("pet.txt");
        string line;

        if(arquivo.is_open()){
            while(!arquivo.eof()){
                getline(arquivo, line);
                if(line == "manual"){
                    while(line != "end"){
                        getline(cin, line);
                        cout << "  " << shell(line) << endl;
                    }
                }
                else if(line == "end")
                    break;
                cout << line << endl;
                cout << "  " << shell(line) << endl;
            }
            arquivo.close();
        } else
            cout << "impossivel abrir o arquivo!";
    }
};

int main(){
    setlocale(LC_ALL,"Portuguese");
    Controller controller;
    controller.exec();

    return 0;
}