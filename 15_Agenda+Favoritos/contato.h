#ifndef CONTATO_H
#define CONTATO_H

#include "entry.h"
#include <vector>
#include <sstream>

using namespace std;

class Contato : public Entry {
    string name;
    vector<Fone> fones;
public:
    Contato(string name = ""):name(name){}
    ~Contato(){ fones.clear(); }

    string getId(){ return name; }

    void addFone(Fone fone){
        if(!Fone::validate(fone.getNumber()))
            throw "fone " + fone.getNumber() + " invalido";
        fones.push_back(fone);
    }
    void rmFone(size_t indice){
        if(indice < 0 || indice >= fones.size())
            throw "indice " + to_string(indice) + " nao existe";
        fones.erase(fones.begin() + indice);
    }
    vector<Fone> getFones(){
        vector<Fone> resp;
        for(auto fone : fones)
            resp.push_back(fone);
        return resp;
    }
    virtual string toString(){
        stringstream ss;
        if(this->isFavorited())
            ss << "@ ";
        else
            ss << "- ";
        ss << this->name << " C ";
        int i = 0;
        for(auto fone : getFones())
            ss << "[" + to_string(i++) + ":" + fone.getId() + ":" + fone.getNumber() + "]";
        return ss.str();
    }
};

#endif