#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Entry {
    bool favorited = false;
public:
    Entry(){};
    virtual ~Entry(){};
    virtual string getId() = 0;
    void setFavorited(bool value){ this->favorited = value; }    
    bool isFavorited(){ return favorited; }
    virtual string toString() = 0;
};

class Note : public Entry { 
    string id;
    vector<string> itens;
public:
    Note(string id = ""):id(id){}
    
    string getId(){ return id; }

    void addItem(string item){
        itens.push_back(item);
    }
    void rmItem(size_t indice){
        if(indice >= itens.size() || indice < 0)
            throw "indice" + to_string(indice) + "nao existe";
        itens.erase(itens.begin() + indice);
    }
    virtual string toString(){
        stringstream ss;
        int ind = 0;
        if(this->isFavorited()) ss << "@ ";
        else ss << "- ";
        ss << this->id << " N ";
        for(auto item : itens)
            ss << "[" + to_string(ind++) + ":" + item + "]";
        return ss.str();
    }
};

class Fone {
    string id;
    string number;
public:
    Fone(string id = "", string number = ""){
        this->id = id;
        this->number = number;
    }

    string getId(){ return id; }
    string getNumber(){ return number; }

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
public:
    Contato(string name = ""):
        name(name){}

    string getId(){ return name; }

    void addFone(Fone fone){
        if(!Fone::validate(fone.getNumber()))
            throw "fone " + fone.getNumber() + " invalido";
        fones.push_back(fone);
    }
    void rmFone(size_t indice){
        if(indice < 0 || indice >= fones.size())
            throw "indice " + to_string(indice) + " nao existe";
        fones.erase(fones.begin() + indice);
    }
    vector<Fone> getFones(){
        vector<Fone> resp;
        for(auto fone : fones)
            resp.push_back(fone);
        return resp;
    }
    virtual string toString(){
        stringstream ss;
        if(this->isFavorited())
            ss << "@ ";
        else
            ss << "- ";
        ss << this->name << " C ";
        int i = 0;
        for(auto fone : getFones())
            ss << "[" + to_string(i++) + ":" + fone.getId() + ":" + fone.getNumber() + "]";
        return ss.str();
    }
};

class Agenda {
    map<string, Entry*> m_entries;
    map<string, Entry*> m_favorites;
public:
    void addEntry(Entry * entry){
        string id = entry->getId();
        m_entries[id] = entry;
    }
    void rmEntry(string id){
        m_entries.erase(id);
        m_favorites.erase(id);
    }
    void favorite(string id){
        Entry * entry = getEntry(id);
        entry->setFavorited(true);
        m_favorites[id] = entry;
    }
    void unfavorite(string id){
        getEntry(id)->setFavorited(false);
        m_favorites.erase(id);
    }
    bool exists(string id){
        auto it = m_entries.find(id);
        if(it != m_entries.end())
            return true;
        return false;
    }
    Entry * getEntry(string id){
        auto it = m_entries.find(id);
        if(it != m_entries.end())
            return it->second;
        throw "entrada " + id + " nao existe";
    }
    vector<Entry*> getEntries(){
        vector<Entry*> v_entries;
        for(auto pair : m_entries)
            v_entries.push_back(pair.second);
        return v_entries;
    }
    vector<Entry*> getFavorited(){
        vector<Entry*> v_favorites;
        for(auto pair : m_favorites)
            v_favorites.push_back(pair.second);
        return v_favorites;
    }
    vector<Entry*> search(string pattern){
        vector<Entry*> resp;
        for(auto& par : m_entries)
            if(par.second->toString().find(pattern) != string::npos)
                resp.push_back(par.second);
        return resp;
    }
    template<typename T>
    bool casttest(Entry* e, T* t){
        if(dynamic_cast<T*>(e))
            return true;
        return false;
    }
    
    string toString(){
        stringstream ss;
        for(auto entry : getEntries())
            ss << entry->toString() << "\n";
        return ss.str();
    }
};

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

                if(agenda.exists(name))
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
                if(Contato* cont = dynamic_cast<Contato*>(entry))
                    cont->rmFone(indice);
            }
            else if(op == "addNota"){
                string title, item;
                bool exist = false;
                ss >> title;
                Note * nota = new Note(title);

                if(agenda.exists(title))
                    if(nota  = dynamic_cast<Note*>(agenda.getEntry(title)))
                        exist = true;
                while(ss >> item)
                    nota->addItem(item);
                if(!exist)
                    agenda.addEntry(nota);
            }
            else if(op == "rmItem"){
                string name;
                int indice;
                ss >> name >> indice;
                Entry * entry = agenda.getEntry(name);
                if(Note * note = dynamic_cast<Note*>(entry))
                    note->rmItem(indice);
            }            
            else if(op == "rmEntry"){
                string name;
                ss >> name;
                Entry* entry = agenda.getEntry(name);
                agenda.rmEntry(name);
                delete entry;
            }
            else if(op == "agenda"){
                out << agenda.toString();
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
                auto v_favs = agenda.getFavorited();
                for(auto entry : v_favs)
                    out << entry->toString() + "\n";
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
		ifstream arquivo ("agenda.txt");
		string line;

		if(arquivo.is_open()){
			while(!arquivo.eof()){
				getline(arquivo, line);
				if(line == "manual"){
					while(line != "end"){
						getline(cin, line);
						cout << shell(line) << endl;
					}
				}
				else if(line == "end"){
					break;
				}
				cout << line << endl;
				cout << shell(line) << endl;
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