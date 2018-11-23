#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Entry {
    bool favorited;
public:
    Entry();
    virtual ~Entry();
    virtual string getId() = 0;
    virtual void setFavorited(bool value) = 0;
    virtual bool isFavorited() = 0;
    virtual string toString() = 0;
};

class Fone {
public:
    string id;
    string number;

    Fone(string id = "", string number = ""){
        this->id = id;
        this->number = number;
    }

    static bool validate(string number) {
        string data = "1234567890()- ";
        for(auto c : number)
            if(data.find(c) == string::npos)
                return false;
        return true;
    }
};

class Contato : public Entry {
    string name;
    vector<Fone> fones;
    bool favorited;
public:
    Contato(string name = ""):
        name(name), favorited(false){}

    string getId(){
        return name;
    }
    void setFavorited(bool value){
        this->isFavorited = value;
    }    
    bool isFavorited(){
        return isFavorited;
    }
    void addFone(Fone fone){
        if(!Fone::validate(fone.number))
            throw string("fone " + fone.number + " invalido");
        fones.push_back(fone);
    }
    void rmFone(size_t indice){
        if(indice < 0 || indice >= fones.size())
            throw string("indice " + to_string(indice) + " nao existe");
        fones.erase(fones.begin() + indice);
    }
    vector<Fone> getFones(){
        vector<Fone> resp;
        for(auto fone: fones)
            resp.push_back(fone);
        return resp;
    }
    string toString(){
        stringstream ss;
        if(this->isFavorited) ss << "@ ";
        else ss << "- ";
        ss << this->name + " C ";
        int i = 0;
        for(auto fone : getFones())
            ss << "[" + to_string(i++) + ":" + fone.id + ":" + fone.number + "]";
        return ss.str();
    }
};

class Agenda {
    map<string, Entry*> m_entries;
    map<string, Entry*> m_favorites;
public:
    void addEntry(Entry * entry){
        string id = entry->getId;
        m_entries[id] = entry;
    }
    void rmEntry(string id){
        m_entries.erase(id);
        m_favorites.erase(id);
        //delete;
    }
    void favorite(string id){
        getEntry(id)->setFavorited(true);
    }
    void unfavorite(string id){
        getEntry(id)->setFavorited(false);
    }
    bool exists(string id){
        auto it = m_entries.find(id);
        if(it != m_entries.end())
            return true;
        throw "fail: entrada" + id + "nao existe";
    }
    Entry * getEntry(string id){
        auto it = m_entries.find(id);
        if(it != m_entries.end())
            return it->second;
        throw "fail: entrada" + id + "nao existe";        
    }
    vector<Entry*> getEntries(){
        vector<Entry*> v_entries;
        for(auto pair : m_entries)
            v_entries.push_back(pair.second);
    }
    vector<Entry*> getFavorited(){
        vector<Entry*> v_favorites;
        for(auto pair : m_favorites)
            v_favorites.push_back(pair.second);
    }
    vector<Entry*> search(string pattern){
        vector<Entry*> resp;
        for(auto& par : m_entries)
            if(par.second->toString().find(pattern) != string::npos)
                resp.push_back(par.second);
        return resp;
    }
    string toString(){
        string saida = "";
        for(auto entry : getEntries())
            saida += entry->toString() + "\n";
        return saida;
    }
};
/*
class Agenda {
    map<string, Contato> contatos;
    map<string, Contato*> favoritos;

public:
    void addContato(Contato cont){
        string name = cont.getName();
        contatos[name] = cont;
    }
    void rmContato(string name) {
        contatos.erase(name);
        favoritos.erase(name);
    }

    bool exists(string name){
        if(contatos.count(name) == 1)
           return true;
        return false;
    }

    void favoritar(string nome){
        Contato* pContat = getContato(nome);
        pContat->setFavorite(true);
        favoritos[nome] = pContat;
    }

    vector<Contato*> getFavoritos(){
        vector<Contato*> resp;
        for(auto contato : favoritos)
            if(contato.second->getFavorite())
                resp.push_back(contato.second);
        return resp;
    }

    void desfavoritar(string nome){
        getContato(nome)->setFavorite(false);
    }

    Contato * getContato(string name){
        if(contatos.count(name))
            return &contatos[name];
        throw string("fail: contato " + name + " nao existe");
    }

    vector<Contato> getContatos(){
        vector<Contato> resp;
        for(auto& par : contatos)
            resp.push_back(par.second);
        return resp;
    }

    vector<Contato> search(string pattern){
        vector<Contato> resp;
        for(auto& par : contatos)
            if(par.second.toString().find(pattern) != string::npos)
                resp.push_back(par.second);
        return resp;
    }

    string toString(){
        string saida = "";
        for(auto contato : getContatos())
            saida += contato.toString() + "\n";
        return saida;
    }
};
*/

class Controller {
    Agenda agenda;
public:
    string shell(string line){
        stringstream ss(line);
        stringstream out;
        string op;
        ss >> op;

        try{
            if(op == "addContato"){
                string name, id_number;
                bool exist = false;
                ss >> name;
                Contato* cont = new Contato(name);
                if(cont = dynamic_cast<Contato*>(agenda.getEntry(name)))
                    exist = true;
                while(ss >> id_number){
                    string id, fone;
                    stringstream ssfone(id_number);
                    getline(ssfone, id, ':');
                    ssfone >> fone;
                    cont->addFone(Fone(id, fone));
                }
                if(!exist)
                    agenda.addEntry(cont);
            }
            else if(op == "rmFone"){
                string name;
                int indice;
                ss >> name >> indice;
                Entry * entry = agenda.getEntry(name);
                if(Contato * cont = dynamic_cast<Contato*>(entry))
                    cont->rmFone(indice);
            }
            else if(op == "rmContato"){
                string name;
                ss >> name;
                agenda.rmEntry(name);
            }
            else if(op == "agenda"){
                cout << agenda.toString();
            }
            else if(op == "search"){
                string pattern;
                ss >> pattern;
                auto resp = agenda.search(pattern);
                for(auto entry : resp)
                    if(dynamic_cast<Contato*>(entry))
                        cout << entry->toString() << endl;
            }
            else if(op == "fav"){
                string name;
                ss >> name;
                agenda.favorite(name);
            }
            else if(op == "unfav"){
                string name;
                ss >> name;
                agenda.unfavorite(name);
            }
            else if(op == "favorited"){
                auto favs = agenda.getFavorited();
                for(auto contato : favs)
                    cout << contato->toString() + "\n";
            }
            else{
                cout << "comando invalido" << endl;
            }
        }
        catch(string e){ out << "  fail: " + e; }
	    catch(char const* e){ out << "  fail: " << e; }
	    catch(...){ out << "  fail: ocorreu uma excecao"; }
	    return out.str();
	} 

	void exec(){
		ifstream arquivo ("agenda.txt");
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
				else if(line == "end"){
					break;
				}
				cout << line << endl;
				cout << "  " << shell(line) << endl;
			}
			arquivo.close();
		} else
			cout << "  fail: impossivel abrir o arquivo \".txt\" de execucao";
	}
};

int main()
{
    Controller controller;
    controller.exec();
    return 0;
}