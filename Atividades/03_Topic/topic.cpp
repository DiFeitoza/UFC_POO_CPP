#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

struct Passageiro{
    string nome;
    int idade;

    Passageiro(string nome = "fulano", int idade = 65){
        this->nome = nome;
        this->idade = idade;
    }
    string toString(){
        stringstream ss;
        ss << this->nome << ":" << this->idade;
        return ss.str();
    }
};

struct Topic{
    vector<Passageiro*> poltronas;

    /*
    Topic(int qtd = 0){
        for(int i = 0; i < qtd; i++)
            poltronas.push_back(nullptr);
    }*/

    Topic(int qtd = 0):
        poltronas(qtd, nullptr)
    {
    }

   ~Topic(){
        for(Passageiro * pol : poltronas)
            delete(pol);
    }

    bool reservar(Passageiro * passageiro, int qtd, int pri){
        if(qtd==0){
            cout << "Fail: solicite uma topic (dica: use init)." << endl;
            return false;
        }

        for(Passageiro * pass : poltronas){
            if(pass != nullptr && pass->nome == passageiro->nome){
                cout << "Fail: passageiro ja embarcado." << endl;
                return false;
            }
        }

        if(passageiro->idade < 65){
            for(int i=pri; i<qtd; i++){
                if(poltronas[i] == nullptr){
                    poltronas[i] = passageiro;                 
                    return true;
                }
            }
            for(int i=pri; i>=0; i--){
                if(poltronas[i] == nullptr){
                    poltronas[i] = passageiro;
                    return true;
                }                
            }
        }
        else if(passageiro->idade >= 65){
            for(int i=0; i<qtd; i++){
                if(poltronas[i] == nullptr){
                    poltronas[i] = passageiro;
                    return true;
                }
            }
        }

        cout << "Fail: nenhuma poltrona disponivel" << endl;

        return false;
    }

    string toString(int qtd, int pri){
        stringstream ss;
        ss << "[ ";
        for(int i=0; i<qtd; i++)
            if(poltronas[i] == nullptr && i < pri){
                ss << "@ ";
            }
            else if(poltronas[i] == nullptr && i >= pri){
                ss << "# ";
            }
            else {
                ss << poltronas[i]->toString() << " ";
            }
        ss << "]";
        return ss.str();
    }
};

int main(){ 
    Topic topic;
    string op;
    int qtd=0, pri=0;

    while(true){
        cin >> op;
        if(op == "end"){
            break;
        }
        if(op == "help"){
            cout << "show; init _qtd; reservar _nome _idade; cancelar _nome" << endl;
        }
        else if(op == "show"){
            cout << topic.toString(qtd, pri) << endl;
        }
        else if(op == "init"){
            cin >> qtd >> pri;
            topic = Topic(qtd);
            cout << "Topic criada com " << qtd << " poltronas, sendo " << pri << " prioritaria(s)."<< endl;
        }
        else if(op == "reservar"){
            string nome;
            int idade;
            cin >> nome >> idade;
            if(topic.reservar(new Passageiro(nome, idade), qtd, pri)){
                cout << "done" << endl;
            }
        }
    }
}