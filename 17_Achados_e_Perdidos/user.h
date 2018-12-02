#ifndef USER_H_
#define USER_H_

#include <iostream>
#include <vector>
#include "thing.h"

using namespace std;

class User{
    string id;
    string pass;
    vector<Thing*> v_lost;
    vector<Thing*> v_found;
public:
    User(string id = "", string pass = "")
        : id(id), pass(pass){}
    ~User(){}

    string getId(){ return id; }
    vector<Thing*> getVecLost(){ return v_lost; }
    vector<Thing*> getVecFound(){ return v_found; }
    
    Thing* getFound(string id){
        for(auto &found : v_found)
            if(id == found->getId())
                return found;
        throw "indice nao existe";
    }
        
    bool verifyPass(string pass){
        return this->pass == pass;
    }
    bool changePass(string oldPass, string newPass){
        if(this->pass != oldPass)
            throw "fail: senha invalida";
        this->pass = newPass;
        return true;
    }
    void addLost(Thing* thing){
        v_lost.push_back(thing);
    }
    void addFound(Thing* thing){
        v_found.push_back(thing);
    }
    string showLost(){
        stringstream ss;
        ss << "#Perdidos\n";
        for(auto lost : v_lost)
            ss << lost->toString();
        return ss.str();
    } 
    string showFound(){
        stringstream ss;
        ss << "#Achados\n";
        for(auto found : v_found)
            ss << found->toString();
        return ss.str();
    }    
    string toString(){
        stringstream ss;
        ss << showLost() + "\n\n" + showFound();
        return ss.str();
    }
};

#endif