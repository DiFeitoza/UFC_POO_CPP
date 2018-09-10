#include <iostream>
#include <vector>
#include <sstream> // fluxo de textos
#include <fstream>
#include <iomanip> // manipulador de 'io' (e/s de dados) setprecision(#) fixed
#include <clocale> // acentuação ptbr

using namespace std;

struct Fone{
    string label;
    string fone;

    Fone(string label = "", string fone = ""):
        label(label), fone(fone)
    {
    }

    string toString(){
        stringstream ss;
        ss << "[" << label << ":" << fone << "]";
        return ss.str();
    }
};

class Contato{
private:
    string nome;
    vector<Fone> fones;

public:
    Contato(string nome = ""):
        nome(nome)
    {
    }

    bool validator(string fone = ""){
        string valido = "0123456789().";
        int cont = 0;
        
        for(char f : fone){
            for(char v : valido){
                if(f == v) break;
                else cont++;
            }
            if(cont == 13) return false;
            else cont = 0;
        }

        return true;
    }

    void add(Fone fone){
        /*for(auto telefone : fones)
            if(telefone.label == fone.label){
                cout << "failure: label duplicado";
                return;
            } */
        fones.push_back(fone);
    }

    bool rm(string foneId){
        for(int i=0; i<(int)fones.size(); i++)
            if(fones[i].label == foneId){
                fones.erase(fones.begin()+i);
                return true;
            }
        return false;
    }

    string toString(){
        stringstream ss;
        ss << nome << " ";
        for(auto fone : fones)
            ss << fone.toString();
        return ss.str();
    }
};

struct Controller{
    Contato cont;

    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;

        if(op == "help"){
            out << "show: exibe os dados do contato atual \n  "
                << "init: _nome \n  "
                << "add: _id _fone[.()0123456789] \n  "
                << "end: sair da execucao atual \n  "
                << "*Dica: para execucao manual basta adicionar 'manual' em uma das linha do contao.txt";
        }
        else if(op == "init"){
            string nome = "vazio";
            in >> nome;
            cont = Contato(nome);
            out << "success";
        }
        else if(op == "update"){
            string nome, label, fone;
            in >> nome;
            cont = Contato(nome);
            while(in >> label && in >> fone){
                if(cont.validator(fone)){
                    cont.add(Fone(label, fone));
                    out << label << ":success ";
                } else
                    out << label << ":invalid ";
            }
        }
        else if(op == "show"){
            out << cont.toString();
        }
        else if(op == "add"){
            string label, fone;

            while(in >> label && in >> fone){
                if(cont.validator(fone)){
                    cont.add(Fone(label, fone));
                    out << label << ":success ";
                } else
                    out << label << ":invalid ";
            }
        }
        else if(op == "rm"){
            string label;
            in >> label;
            if(cont.rm(label))
                out << "sucess";
            else
                out << "fail: label inexistente";
        }

        return out.str();
    }

    void exec(){
        ifstream arquivo ("contato.txt");
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

int main(){
    setlocale(LC_ALL,"Portuguese");
    Controller controller;
    controller.exec();

    return 0;
}