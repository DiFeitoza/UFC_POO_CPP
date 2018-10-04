#include<iostream>
#include<sstream>
#include<fstream>
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
    bool validaTransacao(string cid, float value){
        if(this->value + value <= 0)
            throw "fail: fundos insuficientes\n";
        for(auto cli : clientes){
            if(cli.id == cid && cli.saldo + value > 0)
                throw "fail: valor maior que a divida\n";
        }
        return true;
    }
    bool validaCliente(string cid){
        for(Cliente cli : clientes){
            if(cid == cli.id)
                return true;
        }
        return false;
    }
    void add_cli(string cid, string nome){
        if(validaCliente(cid))
            throw "fail: cliente " + cid + " ja existe\n";
        clientes.push_back(Cliente(cid, nome));
    }
    void add_tr(string cid, float value){            
        if(validaCliente(cid)){
            for(Cliente &cli : clientes){
                if(cli.id == cid){
                    cli.saldo += value;
                    this->value += value;
                    transacoes.push_back(Transacao(cont_id_tr, cli.id, value));
                    cont_id_tr++;
                    break;
                }
            }
        } else
            throw "fail: cliente " + cid + " nao existe\n";
    }
    void show_cli(){
        for(Cliente cli : clientes){
            cout << "  [cid:" + cli.id + "] " + cli.nome + ":"
            + to_string((int)cli.saldo) + "\n";
        }         
    }
    void show_tr(){
        for(Transacao tr : transacoes){
            cout << "  [id:" + to_string(tr.id_tr) + "]" + " "
            + tr.id_cli + ":" + to_string((int)tr.value) + "\n";
        }        
    }
    void show_id(string cid){
        for(Transacao tr : transacoes){
            if(tr.id_cli == cid){
                cout << "  [id:" + to_string(tr.id_tr) + "]" + " "
                + tr.id_cli + ":" + to_string((int)tr.value) + "\n";
            }
        }
    }
};


class Agiota{
public:
    string nome;
    Sistema* psis;

    Agiota(string nome, Sistema* psis){
        this->nome = nome;
        this->psis = psis;
    }
    
    void matar(string cid){
        bool b = false;
        int cont = 0;
        for(auto &cli: psis->clientes){
            if(cli.id == cid){
                psis->clientes.erase(psis->clientes.begin()+cont);
                cont = 0;
                b = true;
            }
            if(b) break;
            cont++;
        }
        /* for(int i = psis->transacoes.size(); i>=0; i--){
            if(psis->transacoes[i].id_cli == cid){
                psis->transacoes.erase(psis->transacoes.begin()+i);
            }
        } */
        for(auto &tr: psis->transacoes){
            if(tr.id_cli == cid){
                psis->transacoes.erase(psis->transacoes.begin()+cont);
                cont--;
            }
            cont++;
        }
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
        mainSis(0), mainAge("Plutolomeu", &mainSis){}

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;

        try{
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
                    string cid;
                    float value;
                    in >> cid;
                    in >> value;
                    if(mainSis.validaTransacao(cid, value))
                        mainSis.add_tr(cid, value);
                }
            }
            else if(op == "matar"){
                in >> op;
                mainAge.matar(op);
            }
            else if(op == "help"){
                out << "show cli: exibe lista de clientes\n"
                "  show tr: exibe lista de transacoes\n"
                "  show [clienteid]: exibe transacoes do cliente pelo id\n"
                "  init [(int)valor]: inicia um novo sistema\n"
                "  add cli [id][nome]: adiciona um cliente no sistema (id limitado a uma 'palavra')\n"
                "  add tr [clienteid][(int)valor]: registra uma transacao para o cliente\n"
                "  matar [clienteid]: permite eliminar um cliente e seus dados do sistema";
            }
        } catch(char const* e){
            out << e;
        } catch(string e){
            out << e;
        } catch(...){
            out << "fail: ocorreu uma exceção";
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