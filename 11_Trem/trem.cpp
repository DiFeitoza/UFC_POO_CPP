#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

class Emb{ 
public:
    virtual string getId() = 0;
    virtual string toString() = 0;
    virtual ~Emb(){}
};

class Pass: public Emb {
    string id;
public:
    Pass(string id = ""):
        id(id){}

    virtual ~Pass(){}

    virtual string getId(){
        return id;
    }
    virtual string toString(){
        return id;
    }
};

class Carga: public Emb {
    string id;
    float peso;
public:
    Carga(string id = "", float peso = 0.0):
        id(id), peso(peso){}
    
    virtual ~Carga(){}

    virtual string getId(){
        return id;
    }
    virtual float getPeso(){
        return peso;
    }
    virtual string toString(){
        stringstream ss;
        ss << id + ":" << peso;
        return ss.str();
    }
};

class Vagao{ //contratos
public:
    virtual bool embarcar(Emb * emb) = 0;
    virtual bool desembarcar(string id) = 0;
    virtual bool exists(string id) = 0;
    virtual string toString() = 0;
    virtual ~Vagao(){};
};

class VagaoPessoas : public Vagao {
    vector<Pass*> passageiros;
public:
    VagaoPessoas(int capacidade):
        passageiros(capacidade, nullptr){}

    ~VagaoPessoas(){
        for(size_t i = 0; i < passageiros.size(); i++)
            if(passageiros[i] != nullptr)
                delete passageiros[i];
    }

    virtual bool embarcar(Emb * emb){
        if(Pass * pass = dynamic_cast<Pass*>(emb)){
            for(size_t i = 0; i < passageiros.size(); i++)
                if(passageiros[i] == nullptr){
                    passageiros[i] = pass;
                    return true;
                }
        }
        return false;
    }

    virtual bool desembarcar(string idPass){
        for(auto *& pass : passageiros)
            if(pass != nullptr && pass->getId() == idPass){
                pass = nullptr;
                delete pass;
                return true;
            }
        return false;
    }

    virtual bool exists(string idPass){
        for(size_t i=0; i<=passageiros.size(); i++)
            if(passageiros[i]->getId() == idPass)
                return true;
        return false;
    }

    virtual string toString(){
        stringstream ss;
        ss << "[ ";
        for(auto* pass : passageiros){
            if(pass == nullptr) ss << "- ";
            else ss << pass->toString() << " ";
        }
        ss << "]";
        return ss.str();
    }
};

class VagaoCarga : public Vagao {
    float capacidade;
    map<string, Carga*> cargas;
public:
    VagaoCarga(float capacidade):
        capacidade(capacidade){}

    virtual ~VagaoCarga(){
        for(auto & pair : cargas)
            delete pair.second;
    }

    virtual bool embarcar(Emb * emb){
        if(Carga * carga = dynamic_cast<Carga*>(emb))
            if(carga->getPeso() <= capacidade){
                cargas[carga->getId()] = carga;
                capacidade-=carga->getPeso();
                return true;
            }
        return false;
    }

    virtual bool desembarcar(string idCarga){
        if(exists(idCarga)){
            auto it = cargas.find(idCarga);
            capacidade += it->second->getPeso();
            delete it->second;
            cargas.erase(it);
            return true;
        }
        return false;
    }

    virtual bool exists(string idCarga){
        if(cargas.find(idCarga) != cargas.end())
            return true; 
        return false;
    }

    virtual string toString(){
        stringstream ss;
        ss << "( ";
        for(auto pair : cargas)
            ss << pair.second->toString() << " ";
        ss << "_" << capacidade << " )";
        return ss.str();
    }
};

class Trem{
    vector<Vagao*> vagoes;
    int forca;
public:
    Trem(int forca = 0){
        this->forca = forca;
    }

    ~Trem(){
        for(auto *& vagao : vagoes)
            delete vagao;
    }

    bool addVagao(Vagao * vagao){
        if((int) vagoes.size() < forca){
            vagoes.push_back(vagao);
            return true;
        }
        return false;
    }

    bool embarcar(Emb * emb){
        for(auto * vagao : vagoes)
            if(vagao->embarcar(emb))
                return true;
        return false;
    }

    bool desembarcar(string idPass){
        for(auto * vagao : vagoes)
            if(vagao->desembarcar(idPass))
                return true;
        return false;
    }

    bool exists(string id){
        for(auto * vagao : vagoes)
            if(vagao->exists(id))
                return true;
        return false;
    }

    string toString(){
        stringstream ss;
        ss << "Trem: ";
        for(auto * vagao : vagoes)
            ss << vagao->toString() << " ";
        return ss.str();
    }
};

template<typename T>
T read(stringstream& in){
    T t;
    in >> t;
    return t;
}

class Controller{
    Trem trem;
public:
    Controller(){}

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;

        try{
            if(op == "init"){
                trem = Trem(read<int>(in));
                out << "done";
            }
            else if(op == "nwvp"){
                Vagao * vagao = new VagaoPessoas(read<int>(in));
                if(!trem.addVagao(vagao)){
                    delete vagao;
                    throw "fail: limite de vagoes atingido";
                } out << "done";
            }
            else if(op == "nwvc"){
                Vagao * vagao = new VagaoCarga(read<float>(in));
                if(!trem.addVagao(vagao)){
                    delete vagao;
                    throw "fail: limite de vagoes atingido";
                } out << "done";
            }
            else if(op == "addp"){
                string idPass;
                in >> idPass;
                Pass * pass = new Pass(idPass);
                if(trem.exists(idPass)){
                    throw "fail: " + idPass + "ja esta no trem";
                }else if(!trem.embarcar(pass)){
                    delete pass;
                    throw "fail: trem lotado";
                } out << "done";
            }
            else if(op == "addc"){
                string idCarga;
                in >> idCarga;
                Carga * carga = new Carga(read<string>(in), read<float>(in));
                if(trem.exists(idCarga)){
                    throw "fail: " + idCarga + "ja esta no trem";
                } else if(!trem.embarcar(carga)){
                    delete carga;
                    throw "fail: capacidade maxima atingida";
                } out << "done";
            }
            else if(op == "sair"){
                string idPass = read<string>(in);
                if(!trem.desembarcar(idPass))
                    throw "fail: " + idPass + " nao esta no trem";
                out << "done";
            }
            else if(op == "la"){
                out << trem.toString() << endl;
            }
            else
                throw "fail: comando invalido";
        }
        catch(char const* e){out << e;}
        catch(string e){out << e;}
        catch(...){out << "ocorreu uma excecao";}
        
        return out.str();
    } //>^o.O^<

    void coletor(){
        ifstream arquivo ("trem.txt");
        string line;

        if(arquivo.is_open()){
            while(!arquivo.eof()){
                getline(arquivo, line);
                if(line == "manual")
                    while(line != "end"){
                        getline(cin, line);
                        cout << "  " << shell(line) << endl;
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
    Controller c;
    c.coletor();

    return 0;
}

/* falta avisar que há duplicadas! na carga, porque senao ele substitui o valor da chave */