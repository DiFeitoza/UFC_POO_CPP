#include <iostream>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>

#include "user.h"
#include "thing.h"

using namespace std;

template<typename T>
class Repository{
    map<string, T> data;
public:
    bool exists(string key){
        return data.find(key) != data.end();
    }
    bool add(string key, T elem){
        if(!exists(key)){
            data[key] = elem;
            return true;
        } return false;
    }
    void rem(string key){
        if(!exists(key))
            throw "entrada nao existe";
        data.erase(key);
    }
    T getT(string key, string str = ""){
        auto item = data.find(key);
        if(item != data.end())
            return item->second;
        throw "" + str + " " + key + " nao existe";
    }
    vector<T> getValues(){
        vector<T> vecT;
        for(auto &par : data)
            vecT.push_back(par.second);
        return vecT;
    }
    vector<string> getKeys(){
        vector<string> vecK;
        for(auto& par : data)
            vecK.push_back(par.first);
        return vecK;
    }
    string toString(){
        stringstream ss;
        for(auto key : this->getKeys())
            ss << key + " ";
        ss << "\n";
        return ss.str();
    }
};

class GerLogin{
    User ** current = nullptr;
    Repository<User*> * r_user;
public:
    GerLogin(User ** current, Repository<User*> * r_user)
        : current(current), r_user(r_user){}

    void login(string id, string pass){
        if(*current != nullptr)
            throw "ja existe alguem logado";
        User * user = r_user->getT(id);
        if(!user->verifyPass(pass))
            throw "senha invalida";
        *current = user;
    }
    void logout(){
        if(*current == nullptr)
            throw "ninguem logado";
        *current = nullptr;
        return;
    }
    string toString(){
        stringstream ss;
        for(auto key : r_user->getKeys())
            ss << key;
        return ss.str();
    }
};

class Sistema{
    User * current = nullptr;
    Repository<User*> r_user;
	Repository<Thing*> r_lost;
    Repository<Thing*> r_found;
    GerLogin * g_login = new GerLogin { &current, &r_user };
public:
    void login(string id, string pass){
        if(current != nullptr)
            throw "efetue logout";
        g_login->login(id, pass);
    }
    void logout(){
        if(current == nullptr)
            throw "efetue login";
        g_login->logout();
    }
    string logged(){
        if(current == nullptr)
            throw "efetue login";
        return "Hi " + current->getId() + "!";
    }
    void changePass(string old, string newp){
        if(current == nullptr)
            throw "efetue login";
        current->changePass(old, newp);
    }    
    void addUser(string id, string pass){
        if(current != nullptr)
            throw "efetue logout";
        User * user = new User(id, pass);
        r_user.add(id, user);
    } 
    void rmUser(string pass){
        if(current == nullptr)
            throw "efetue login";
        if(!current->verifyPass(pass))
            throw "senha incorreta";
        for(auto lost : current->getVecLost())
            r_lost.rem(lost->getId());
        for(auto found : current->getVecFound())
            r_found.rem(found->getId());
        r_user.rem(current->getId());
        g_login->logout();
    }        
    void addPerdido(string id, string cat, string des, string loc){
        if(current == nullptr)
            throw "efetue login";        
        Thing * perdido = new BemMaterial(id, cat, loc, des);
        r_lost.add(id, perdido);
        current->addLost(perdido);
    }
    void addAchado(string id, string cat, string des, string loc){
        if(current == nullptr)
            throw "efetue login";        
        Thing * achado = new BemMaterial(id, cat, loc, des);
        r_found.add(id, achado);
        current->addFound(achado);
    }    
    string showAll(){
        if(current == nullptr)
            throw "efetue login";
        return current->toString();
    }
    string showLost(){
        if(current == nullptr)
            throw "efetue login";
        return current->showLost();
    }
    string showFound(){
        if(current == nullptr)
            throw "efetue login";
        return current->showFound();
    }

    int contsubstr(string s_lost, string s_found){
        int cont = 0;
        stringstream ss_lost {s_lost};
        string word;
        while(ss_lost >> word)
            if(s_found.find(word) != string::npos){
                cont++;
                cout << "achei\n";
            }
        return cont;
    }

    void match(){
        vector<Thing*> v_found = r_found.getValues();        
        vector<Thing*> v_lost = r_lost.getValues();

        for(auto &found : v_found){
            int bighest = 0;
            string match_id = "";

            for(auto &lost : v_lost){
                int cont = 0;
                cont += contsubstr(found->getCategory(), lost->getCategory());
                cont += contsubstr(found->getWhereFound(), lost->getWhereFound());
                cont += contsubstr(found->getDescription(), lost->getDescription());
                if(cont > bighest){
                    bighest = cont;
                    match_id = lost->getId();
                }
            }
            found->setMatch(match_id);
        }
    }

    string matchFound(string id){
        stringstream ss;
        for(auto s_id_match : current->getFound(id)->getMatches()){
            ss << r_lost.getT(s_id_match)->toString();
            ss << "\n";
        }
        if(current->getFound(id)->getMatches().empty())
            ss << "vazio\n";
        return ss.str();
    }
};

class Controller {
    Sistema sis;
    int cont = 0;
public:
    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;
        try{
            if(op == "addUser"){
				string id, password;
				in >> id >> password;
				sis.addUser(id, password);
                out << "Usuario cadastrado com sucesso!";
            }
            else if(op == "rmUser"){ //_pass
				string password;
				in >> password;
				sis.rmUser(password);
                out << "Removido com sucesso e deslogado!";
            }
            else if(op == "login"){
				string name, password;
				in >> name >> password;
				sis.login(name, password);
                out << "Seja bem-vindo " + name + "!";
            }
            else if(op == "logout"){
                sis.logout();
                out << "Deslogado com sucesso!";
            }            
            else if(op == "logged"){
                out << sis.logged();
            }
            else if(op == "changePass"){
                string old, newPass;
                in >> old >> newPass;
                sis.changePass(old, newPass);
                out << "Senha alterada com sucesso!";
            }
            else if(op == "addPerdido" || op == "addAchado"){
                string category, description, lost_location;
                /* cout << "Qual a categoria do bem?"; */
                getline(in, category, ':');
                /* cout << "Descreva onde pode tê-lo perdido com palavras-chave"
                << "\nex: cidade, bairro, rua, trajeto, instituição"; */
                getline(in, lost_location, ':');
                /* cout << "Como é este bem? tem algo que o torna único?"
                << "\nex: cor, tamanho, marca, modelo, raça, avaria"; */
                getline(in, description);
                if(op == "addPerdido")
                    sis.addPerdido(to_string(cont), category, description, lost_location);
                else
                    sis.addAchado(to_string(cont), category, description, lost_location);
                cont++;
            }
            else if(op == "showAll"){
                out << sis.showAll();
            }
            else if(op == "showLost"){
                out << sis.showLost();
            }
            else if(op == "showFound"){
                out << sis.showFound();
            }
            else if(op == "match"){
                sis.match();
            }
            else if(op == "matchFound"){ //_idItem
                string id;
                in >> id;
                out << sis.matchFound(id);
            }
            else
                cout << "comando invalido" << endl;
        }
        catch(string e){ out << "  fail: " + e; }
	    catch(char const* e){ out << "  fail: " << e; }
	    catch(...){ out << "  fail: ocorreu uma excecao"; }
	    return out.str();
	} 

	void exec(){
		ifstream arquivo ("input.txt");
		string line;
		if(arquivo.is_open()){
			while(!arquivo.eof()){
				getline(arquivo, line);
				if(line == "manual")
					while(line != "end"){
						getline(cin, line);
						cout << shell(line) << endl;
					}
				else if(line == "end")
					break;
				cout << "$" << line << endl;
				cout << shell(line) + "\n" << endl;
			}
			arquivo.close();
		} else
			cout << "  fail: impossivel abrir o arquivo \".txt\" de execucao";
	}
};

main(){
    Controller control;
    control.exec();
    system("pause");
    return 0;
}

/*
    FALTA FAZER
    1. Implementar data de encontro/perda do bem e de registro
    com isso verificar a compatibilidade pela data primeiro
    depois pelo tipo, pelo local, e por fim por caracteristicas.
    Achar a melhor forma de comparar e fazer o match;

    2. Outras categorias, como os Pets (possuem nome e raça)..

    DÚVIDAS
    1. quem deve guardar a compatibilidade? um map ou o próprio item? (criar uma classe MatchCompare)
*/