#include <iostream>
#include <vector>
#include <sstream>
using namespace std;

struct Cliente{
    string id;
    string fone;
    bool existe;

    Cliente(string id="fulano", string fone="0000", bool existe=true){
        this->id = id;
        this->fone = fone;
        this->existe = existe;
    }

    string toString(){
        stringstream ss;

        if(!existe){
            ss << "-";
        }
        else {
            ss << this->id << ":" << this->fone;
        }

        return ss.str();
    }
};

struct Sala{
    vector<Cliente> poltronas;

    Sala(int qtd=0){
        for(int i=0; i<qtd; i++)
            poltronas.push_back(Cliente("", "", false));
    }

    bool reservar(Cliente cliente, int i){
        int qtd = poltronas.size();

        if((i<0) || (i >= qtd)){
            cout << "fail: essa poltrona nao existe" << endl;
            return false;
        }
        if(poltronas[i].existe){
            cout << "fail: essa poltrona ja esta ocupada" << endl;
            return false;
        }
        for(int i=0; i<qtd; i++){
            if(poltronas[i].existe && (poltronas[i].id == cliente.id)){
                cout << "fail: voce ja esta no cinema" << endl;
                return false;
            }
        }

        poltronas[i] = cliente;
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
            int i;

            cin >> nome >> fone >> i;

            if(sala.reservar(Cliente(nome, fone), i)){
                cout << "done" << endl;
            }
        }
    }
}