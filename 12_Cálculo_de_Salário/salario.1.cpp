#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

class Funcionario{
protected:
    string nome, profissao, salario;
public:
    Funcionario(string nome = ""):
        nome(nome){}
    virtual string toString() = 0;
};

class Professor : public Funcionario{
    string profissao = {"P"};
    char nivel;
public:
    Professor(string nm = "", char nv = 0):
        Funcionario(nm), nivel(nv){}

    string toString(){
        return nome + ":" + profissao + ":" + nivel;
    }
};

class SerTecAdm : public Funcionario{
    string profissao = {"A"};
public:
    SerTecAdm(string nome = ""):
        Funcionario(nome){}

    string toString(){
        return nome + ":" + profissao;
    }
};

class Terceirizado : public Funcionario{
    string profissao = {"T"};
    string horastrab;
    string salubridade;
public:
    Terceirizado(string nm = "", string ht = "", string as = ""):
        Funcionario(nm), horastrab(ht), salubridade(as){}
};

template<typename T>
class Sistema{
    map<string, T*> data;
public:
    bool exists(string k){
        auto it = data.find(k);
        if(it != data.end())
            return true;
        return false;
    }
    bool addUser(string k, T* v){
        if(!exists(k)){
            data[k] = v;
            return true;
        }
        throw "fail: usuario ja cadastrado";
    }
    void rmUser(string k){
        auto &user = getUser(k);
        data.erase(k);
        delete user;
    }
    T* getUser(string k){
        auto it = data.find(k);
        if(it != data.end())
            return it;
        throw "fail: usuario nao existe";
    }
    string toString(){
        stringstream ss;
        for(auto pair : data)
            ss << pair.second->toString()
               << endl << "  ";
        return ss.str();
    }
};

class Controlador{
    Sistema<Funcionario> mySis; //[?]
public:
    string executar(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;
    
        try{
            if(op == "addProf"){
                string name;
                char level;
                in >> name >> level;
                Professor* P = new Professor(name, level);
                if(!mySis.addUser(name, P))
                    delete P;
            }
            else if(op == "rmUser"){
                string name;
                in >> name;
                mySis.rmUser(name);
            }
            else if(op == "addSta"){
            }
            else if(op == "addTer"){
            }
            else if(op == "show"){
                out << mySis.toString();
            }
        }
        catch(char const* e){out << e;}
        catch(string e){out << e;}
        catch(...){out << "fail: ocorreu uma exceção";}
        
        return out.str();
    }

    void coletor(){
        ifstream arquivo ("salario.txt");
        string line;

        if(arquivo.is_open()){
            while(!arquivo.eof()){
                getline(arquivo, line);
                if(line == "manual"){
                    while(line != "end"){
                        getline(cin, line);
                        cout << "  " << executar(line) << endl;
                    }
                }
                else if(line == "end")
                    break;
                cout << line << endl;
                cout << "  " << executar(line) << endl;
            }
            arquivo.close();
        } else
            cout << "impossivel abrir o arquivo!";
    }
};

int main(){
    Controlador c;
    c.coletor();
    return 0;
}