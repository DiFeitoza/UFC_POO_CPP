#ifndef NOTE_H
#define NOTE_H

#include "entry.h"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Note : public Entry { 
    string id;
    vector<string> itens;
public:
    Note(string id = ""):id(id){}
    ~Note(){ itens.clear(); }
    
    string getId(){ return id; }

    void addItem(string item){
        itens.push_back(item);
    }
    void rmItem(size_t indice){
        if(indice >= itens.size() || indice < 0)
            throw "indice fora do intervalo";
        itens.erase(itens.begin() + indice);
    }
    virtual string toString(){
        stringstream ss;
        int ind = 0;
        if(this->isFavorited()) ss << "@ ";
        else ss << "- ";
        ss << this->id + " N ";
        for(auto item : itens)
            ss << "[" + to_string(ind++) + ":" + item + "]";
        return ss.str();
    }
};

#endif