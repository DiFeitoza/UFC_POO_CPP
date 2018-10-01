#include<iostream>
#include<sstream>
#include<fstream>
#include<algorithm>
#include<vector>

using namespace std;

class Cliente{
public:
    string id;
    string nome;
    float saldo;
    bool alive;

    Cliente(string id = "", string nome = "", float saldo = 0):
        id(id), nome(nome), saldo(saldo), alive(true){}
};


class Transacao{
public:
    int id_tr;
    string id_cli;
    float value;

    Transacao(int idt, string idc, float val):
        id_tr(idt), id_cli(idc), value(val){}
};


class Sistema{
public:
    float value;
    vector<Cliente> clientes;
    vector<Transacao> transacoes;
    int cont_id_tr = 0;

    Sistema(float value):
        value(value){}

    Sistema init(float value){
        return Sistema(value);
    }
    void add_cli(string id, string nome){
        clientes.push_back(Cliente(id, nome));
    }
    void add_tr(string myid, float value){
        for(Cliente &cli : clientes){
            if(cli.id == myid){
                cli.saldo += value;
                transacoes.push_back(Transacao(cont_id_tr, cli.id, value));
                cont_id_tr++;
            }
        }
    }
    void show_cli(){
        for(Cliente cli : clientes){
            cout << "[cid:" + cli.id + "] " + cli.nome + ":" + to_string(cli.saldo) + "\n";
        }         
    }
    void show_tr(){
        for(Transacao tr : transacoes){
            cout << "[id:" + to_string(tr.id_tr) + "]" + " " + tr.id_cli + ":" + to_string(tr.value) + "\n";
        }        
    }
    void show_id(string myid){
        for(Cliente cli : clientes){
            if(cli.id == myid){
                cout << cli.nome + ":" + to_string(cli.saldo) << endl;
            }
        }
    }
    
    /* template<typename T>
    T toString(T coisa){
        stringstream ss;
        ss << coisa;
        return ss;
    } */
};


class Agiota{
public:
    string nome;
    Sistema* psis;

    Agiota(string nome, Sistema* psis){
        this->nome = nome;
        this->psis = psis;
    }
    
    void matar(){
    }
    void toString(){
        cout << "nome: " + nome + " | saldo: " + to_string(psis->value);
    }
};


class Controller{
public:
    Sistema mainSis;
    Agiota mainAge;

    Controller():
        mainSis(0), mainAge("Ptolomeu", &mainSis){}

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;

        if(op == "show"){
            in >> op;
            if(op == "tr"){
                mainSis.show_tr();
            }
            else if(op == "cli"){
                mainSis.show_cli();
            }
            else{
                mainSis.show_id(op);
            }
        }
        else if(op == "init"){
            float initValue;
            in >> initValue;
            mainSis = mainSis.init(initValue);
        }
        else if(op == "add"){
            in >> op;
            if(op == "cli"){
                string id, nome;
                in >> id;
                getline(in, nome);
                mainSis.add_cli(id, nome);
            }
            else if(op == "tr"){
                string id;
                float value;
                in >> id;
                in >> value;
                mainSis.add_tr(id, value);
            }
        }
        else if(op == "matar"){
        }
        else if(op == "help"){
            out << "";
        }

        return out.str();
    }

    void exec(){
        ifstream arquivo ("agiota.txt");
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
};

main(){
    Controller c;
    c.exec();

    return 0;
}