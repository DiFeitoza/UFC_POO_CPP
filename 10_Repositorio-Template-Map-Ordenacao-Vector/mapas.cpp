// create, read, update, delete

#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct Pessoa{
    string nome;
    int idade;
    Pessoa(string nome = "", int idade = 0):
        nome(nome), idade(idade){
    }
    string toString(){
        return "[" + nome + ":" + to_string(idade) + "]";
    }
};

#include <map>
int main(){

    vector<int> vet = {1, 4, 3, 65, 7, 2, 7, 9};
    vector<int>::iterator it = std::find(vet.begin(), vet.end(), 65);
    
    //incrementar +1 em todos os itens do vetor;
    for(auto it = vet.begin(); it != vet.end(); it++){
        *it += 1;
    }

    //criar um mapeamento em par _chave x _valor;
    map<string, Pessoa> rep;

    //inserir um par de dados;
    rep["maria"] = Pessoa("Maria Lima", 45);
    rep["jose"] = Pessoa("Jose almeida", 70);
    rep["lucas"] = Pessoa("Lucas Pinho", 12);

    //outra forma de inserir;
    rep.insert(make_pair("david", Pessoa("David Sena", 34)));

    //coletando um valor específico do par 'valor';
    cout << rep.at("jose").nome << endl;

    //acessando todos os valores do mapa;
    for(pair<string, Pessoa> par : rep){
        cout << "key: " << par.first << " | valor: " << par.second.toString() << endl;
    }

    //removendo um dado pela chave;
    rep.erase("lucas");

    cout << "Depois de um kick no lucas" << endl;
    
    //acessando todos os valores do mapa;
    for(pair<string, Pessoa> par : rep){
        cout << "key: " << par.first << " | valor: " << par.second.toString() << endl;
    }

    return 0;
}