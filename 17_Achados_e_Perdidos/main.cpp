#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
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
        throw key + " nao existe";
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
    void addPlace(string id, string pass, string cnpj, string andr, string contc){
        if(current != nullptr)
            throw "efetue logout";        
        User * user = new FisicPlace(id, pass, cnpj, andr, contc);
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
        delete current;
        g_login->logout();
    }        
    void addLost(string id, string cat, string des, string loc){
        if(current == nullptr)
            throw "efetue login";
        Thing * perdido = new BemMaterial(id, cat, loc, des, current);
        r_lost.add(id, perdido);
        current->addLost(perdido);
    }
    void addFound(string id, string cat, string des, string loc){
        if(current == nullptr)
            throw "efetue login";
        Thing * achado = new BemMaterial(id, cat, loc, des, current);
        r_found.add(id, achado);
        current->addFound(achado);
    }
    string showPlaces(string locale){
        stringstream ss;
        int cont = 0;
        for(auto user : r_user.getValues())
            if(FisicPlace * p_place = dynamic_cast<FisicPlace*>(user))
                if(p_place->getAndress().find(locale) != string::npos){
                    ss << p_place->toString() + "\n";
                    cont++;
                }
        if(cont == 0)
            throw "Nao ha Pontos Fisicos cadastratos em: " + locale;                    
        string s = ss.str();
        return s.substr(0, s.length()-1);
    }
    string showAllPlaces(){
        stringstream ss;
        for(auto user : r_user.getValues())
            if(FisicPlace * p_place = dynamic_cast<FisicPlace*>(user))
                ss << p_place->toString() + "\n";
        string s = ss.str();
        return s.substr(0, s.length()-1);
    }    
    int contsubstr(string s_found, string s_lost){
        int cont = 0;
        stringstream ss_found {s_found};
        string word;
        while(ss_found >> word)
            if(s_lost.find(word) != string::npos)
                cont++;
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
        if(current == nullptr)
            throw "efetue login";
        stringstream ss;
        auto v_id_match = current->getFound(id)->getMatches();
        if(v_id_match.empty())
            throw "Ainda não encontramos o possível dono";
        for(auto s_id_match : v_id_match){
            ss << "user: " << r_lost.getT(s_id_match)->getOwner()->getId() << "\n";
            ss << r_lost.getT(s_id_match)->toString();
        }
        return ss.str();
    }
    void giveBack(string id, string username){
        if(current == nullptr)
            throw "efetue login";
        stringstream ss;
        auto v_id_match = current->getFound(id)->getMatches();
        if(v_id_match.empty())
            throw "Ainda não encontramos o possível dono";
        for(auto s_id_match : v_id_match)
            if(username == r_lost.getT(s_id_match)->getOwner()->getId()){
                r_lost.getT(s_id_match)->setStatus(current->getId() + " encontrou e quer devolver este item");
                r_found.getT(id)->setStatus("Aguardando confirmacao de devolucao");
                r_lost.getT(s_id_match)->setTwin(id);
                return;
            }
        throw "username inserido nao corresponde a um match valido";
    }
    void checkGiveBack(string id){
        if(current == nullptr)
            throw "efetue login";
        stringstream ss;
        auto lost = current->getLost(id);
        auto twin_found = r_found.getT(lost->getTwin());
        lost->setStatus("Perdeu, Achou! Estamos felizes por ter encontrado!");
        twin_found->setStatus("Perdeu, Achou! Obrigado, sua atitude faz a diferenca!");
    }    

    string showItens(){
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
};

class Controller {
    Sistema sis;
    int cont = 0;
public:
    string shell(string line){
        stringstream in(line);
        stringstream out {""};
        string op;
        in >> op;
        in.get(); //long pos = in.tellg(); in.seekg(pos+1);
        try{
            if(op == "addUser"){
				string id, password;
				in >> id >> password;
				sis.addUser(id, password);
                out << "Usuario cadastrado com sucesso!";
            }
            else if(op == "addPlace"){
                string id, pass, cnpj, andress, contact;
                getline(in, id, '|');
                getline(in, pass, '|');
                getline(in, cnpj, '|');
                getline(in, andress, '|');
                getline(in, contact, '|');
                sis.addPlace(id, pass, cnpj, andress, contact);
                out << "Ponto Fisico cadastrado com sucesso!";
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
            else if(op == "addLost" || op == "addFound"){
                string category, description, lost_location;
                /* cout << "Qual a categoria do bem?"; */
                getline(in, category, '|');
                /* cout << "Descreva onde pode tê-lo perdido com palavras-chave"
                << "\nex: cidade, bairro, rua, trajeto, instituição"; */
                getline(in, lost_location, '|');
                /* cout << "Como é este bem? tem algo que o torna único?"
                << "\nex: cor, tamanho, marca, modelo, raça, avaria"; */
                getline(in, description);
                if(op == "addLost")
                    sis.addLost(to_string(cont), category, description, lost_location);
                else
                    sis.addFound(to_string(cont), category, description, lost_location);
                cont++;
            }
            else if(op == "match"){
                sis.match();
            }
            else if(op == "matchFound"){
                string id;
                in >> id;
                out << sis.matchFound(id);
            }           
            else if(op == "giveBack"){
                string id, username;
                in >> id >> username;
                sis.giveBack(id, username);
                out << "Aguardando confirmacao de devolucao!";
            }
            else if(op == "checkGiveBack"){
                string id;
                in >> id;
                sis.checkGiveBack(id);
                out << "Recebimento confirmado!";
            }
            else if(op == "showItens"){
                out << sis.showItens();
            }
            else if(op == "showLost"){
                out << sis.showLost();
            }
            else if(op == "showFound"){
                out << sis.showFound();
            }
            else if(op == "showPlaces"){
                string locale;
                getline(in, locale);
                out << sis.showPlaces(locale);
            }
            else if(op == "showAllPlaces"){
                out << sis.showAllPlaces();
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
                line = shell(line);
                if(line == "") cout << "\n";
                else cout << line + "\n\n";
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
    FALTA FAZER:
    1. Implementar data de encontro/perda do bem e de registro.
    com isso verificar a compatibilidade pela data primeiro.
    depois pelo tipo, pelo local, e por fim por caracteristicas.
    Achar a melhor forma de comparar e fazer o match.
    2. Quem cadastra perdido diz intervalo de perda, maximo quinze dias atras.
    3. Quem registra achado, automaticamente registra o dia do achado.
    4. Outras categorias, como os Pets (possuem nome e raça).
    6. Iniciar chat.
    7. Reconhecimento por imagem.

    URGÊNCIAS:
    2. Usuário pode excluir achado/perdido.

    5. Validar o match.

    3. Usuário pode solicitar transferência.
    4. Usuário pode aceitar a transferência.

    8. Usuário pode solicitar entrega.
    9. Usuário pode confirmar entrega.

    10. Status da coisa.

    IMPORTANTE:
    1. Registrar data e hora de registro da coisa.    
    5. Usuário pode solicitar adicionar contato.
    6. Usuário pode aceitar solicitação.
    7. Usuário pode enviar mensagens.    

    DÚVIDAS
    1. quem deve guardar a compatibilidade? um map ou o próprio item? (criar uma classe MatchCompare).
*/