#include <iostream>
#include <sstream>
#include "controller.hpp"
#include "pet.hpp"

using namespace std;

Controller::Controller():
    pet("pet", 15, 10, 30){
};

string Controller::shell(string line){
    stringstream in(line);
    stringstream out;
    string op;

    in >> op;

    if(op == "show"){
        out << pet.toString();
    }
    else if(op=="init"){
        int e, h, c;
        in >> e >> h >> c;
        pet = pet.init(e, h, c);
    }
    else if(op == "rename"){
        string newName;
        in >> newName;
        pet.rename(newName);
        out << "Seu pet agora se chama: " << newName << "!";
    }
    else if(op == "eat"){
        if(pet.eat())
            out << "Yumyum, pancinha cheia!";
    }
    else if(op == "play"){
        if(pet.play())
            out << "Dancei, brinquei, corri, pulei!";
    }
    else if(op == "shower"){
        if(pet.shower())
            out << "Yuuupie, estou cheirosinho!";
    }
    else if(op == "sleep"){
        out << pet.sleep();
    }
    // else if(op == "end")
    else if(op == "help"){
        out << "init: recebe 3 dados na ordem 'energia', 'fome', 'limpeza'; maximos. \n  "
            << "rename: escreva um novo nome. \n  "
            << "show: exibe status do pet. \n  "
            << "eat: alimentar o pet. \n  "
            << "play: bricar com o pet. \n  "
            << "shower: banhar no pet. \n  "
            << "sleep: levar o pet para descansar. \n  "
            << "end: sair da execucao atual. \n  "
            << "*Dica: escreva 'manual' no pet.txt para iniciar execucao manual.";
    }        

    return out.str();
}

void Controller::exec(){
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