#ifndef USER_H_
#define USER_H_

#include <iostream>
#include <vector>
#include "thing.h"

using namespace std;

class User{
protected:
    string id;
    string pass;
    vector<Thing*> v_lost;
    vector<Thing*> v_found;
public:
    User(string id, string pass)
        : id(id), pass(pass){}
    ~User(){}

    virtual string getId(){ return id; }
    virtual vector<Thing*> getVecLost(){ return v_lost; }
    virtual vector<Thing*> getVecFound(){ return v_found; }
    
    virtual Thing* getFound(string id){
        for(auto &found : v_found)
            if(id == found->getId())
                return found;
        throw "indice nao existe";
    }
        
    virtual bool verifyPass(string pass){
        return this->pass == pass;
    }
    virtual bool changePass(string oldPass, string newPass){
        if(this->pass != oldPass)
            throw "fail: senha invalida";
        this->pass = newPass;
        return true;
    }
    virtual void addLost(Thing* thing){
        v_lost.push_back(thing);
    }
    virtual void addFound(Thing* thing){
        v_found.push_back(thing);
    }
    virtual string showLost(){
        stringstream ss;
        ss << "#Perdidos\n";
        for(auto lost : v_lost)
            ss << lost->toString();
        return ss.str();
    } 
    virtual string showFound(){
        stringstream ss;
        ss << "#Achados\n";
        for(auto found : v_found)
            ss << found->toString();
        return ss.str();
    }    
    virtual string toString(){
        stringstream ss;
        ss << "user: " + id + "\n";
        ss << showLost() + "\n" + showFound();
        return ss.str();
    }
};

class FisicPlace : public User{    
    string cnpj, andress, contact;
public:
    FisicPlace(string id, string pass, string cnpj, string andr, string cont)
        : User(id, pass), cnpj(cnpj), andress(andr), contact(cont){}
    ~FisicPlace(){}

    string getCnpj(){ return cnpj; }
    string getAndress(){ return andress; }
    string getContact(){ return contact; }

    string toString(){
        stringstream ss;
        ss << "name: " + id + "\n"
        + "cnpj: " + cnpj + "\n"
        + "endereco: " + andress + "\n"
        + "contato: " + contact;
        return ss.str();
    }
};

#endif