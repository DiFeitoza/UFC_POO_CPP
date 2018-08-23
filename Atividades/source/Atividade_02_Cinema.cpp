#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

struct Cliente{
    string id;
    string fone;

    Cliente(string id, string fone){
        this->id = id;
        this->fone = fone;
    }

    string toString(){
        stringstream ss;

        if(poltrona[ind] != NULL){
            ss << "-";
        }
        else {
            ss << this->id << ":" << this->fone;
        }

        return ss.str();
    }
};

struct Sala{
    poltronas = new Cliente*[qtd];
    
    for(int i=0; i<qtd; i++){
        poltronas[ind] = NULL;
    }

    Sala(int qtd=0){
        for(int i=0; i<qtd; i++)
            poltronas.push_back(Cliente("", ""));
    }

    bool reservar(Cliente cliente, int ind){
        int qtd = poltronas.size();

        if((ind<0) || (ind >= qtd)){
            cout << "fail: essa poltrona nao existe" << endl;
            return false;
        }
        if(poltronas[ind] != NULL){
            cout << "fail: essa poltrona ja esta ocupada" << endl;
            return false;
        }
        for(int i=0; i<qtd; i++){
            if(poltronas[ind] != NULL && (poltronas[ind].id == cliente.id)){
                cout << "fail: voce ja esta no cinema" << endl;
                return false;
            }
        }

        poltronas[ind] = cliente;
        return true;
    }

    string toString(){
        stringstream ss;

        ss << "[ ";

        for(Cliente cliente : poltronas){
            ss << cliente.toString() << " ";
        }

        ss << "]";

        return ss.str();
    }
};

int main(){
    Sala sala;
    string op;

    while(true){
        cin >> op;

        if(op == "show"){
            cout << sala.toString() << endl;
        }

        else if(op == "init"){
            int qtd;
            cin >> qtd;
            sala = Sala(qtd);
            cout << "sala criada com " << qtd << " poltronas" << endl;
        }

        else if(op == "reservar"){
            string nome, fone;
            int ind;

            cin >> nome >> fone >> ind;

            if(sala.reservar(Cliente(nome, fone), ind)){
                cout << "done" << endl;
            }
        }
    }
}