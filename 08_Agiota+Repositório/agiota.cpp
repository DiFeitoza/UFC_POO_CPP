#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<map>

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

    Transacao(int idt = 0, string idc = "", float val = 0.0):
        id_tr(idt), id_cli(idc), value(val){}
};

template<typename T>
class Repositorio{
    map<string, T> data;
public:
    bool exists(string key){
        return data.find(key) != data.end();
    }
    bool add(string key, T elem){
        if(!exists(key)){
            data[key] = elem;
            return true;
        }
        return false;
    }
    bool rem(string key){
        if(exists(key)){
            data.erase(key);
            return true;
        }
        return false;
    }
    T& getT(string key, string str = ""){
        auto item = data.find(key);
        if(item != data.end())
            return item->second;
        throw "" + str + " " + key + " nao existe";
    }
    vector<T> getValues(){
        vector<T> vecT;
        for(auto& par : data)
            vecT.push_back(par.second);
        return vecT;
    }
    vector<string> getKeys(){
        vector<string> vecK;
        for(auto& par : data)
            vecK.push_back(par.first);
        return vecK;
    }
    static void toString(T t){
        stringstream ss;
        ss << t.id << ":";
        if(t.curso){
            ss << t.curso << ":";
        }
        ss << t.nome;
        cout << ss.str();
    }
};

class Sistema{
public:
    float value;
    Repositorio<Cliente> repCli;
    Repositorio<Transacao> repTra;
    int cont_id_tr = 0;

    Sistema(float value):
        value(value){}

    Sistema init(float value){
        return Sistema(value);
    }

    bool validaTransacao(string cid, float value){
        if(this->value + value <= 0)
            throw "fail: fundos insuficientes\n";
        if(repCli.getT(cid).saldo + value > 0)
            throw "fail: valor maior que a divida\n";
        return true;
    }
    void add_cli(string cid, string nome){
        if(repCli.exists(cid))
            throw "fail: cliente " + cid + " ja existe\n";
        repCli.add(cid, Cliente(cid, nome));
    }
    void add_tr(string cid, float value){            
        if(repCli.exists(cid)){
            repCli.getT(cid).saldo += value;
            this->value += value;
            repTra.add(to_string(cont_id_tr), Transacao(cont_id_tr, cid, value));
            cont_id_tr++;
        } else
            throw "fail: cliente " + cid + " nao existe\n";
    }
    void show_cli(){
        for(Cliente cli : repCli.getValues()){
            cout << "  [cid:" + cli.id + "] " + cli.nome + ":"
            + to_string((int)cli.saldo) + "\n";
        }         
    }
    void show_tr(){
        for(Transacao tr : repTra.getValues()){
            cout << "  [id:" + to_string(tr.id_tr) + "]" + " "
            + tr.id_cli + ":" + to_string((int)tr.value) + "\n";
        }        
    }
    void show_id(string cid){
        for(Transacao tr : repTra.getValues()){
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

    Agiota(string nome, Sistema* psis):
        nome(nome), psis(psis){}
    
    void matar(string cid){
        psis->repCli.rem(cid);
        for(auto &tr: psis->repTra.getValues())
            if(tr.id_cli == cid)
                psis->repTra.rem(to_string(tr.id_tr));
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