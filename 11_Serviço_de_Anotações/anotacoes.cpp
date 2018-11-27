#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class Nota{
    string title, text;
public:
    Nota(string title = "", string text = ""):
        title(title), text(text){}

    string toString(){
        return "  [" + title + ";" + text + "]";
    }
};

class Usuario{
    string username, password;
    map<string, Nota> notas;
public:
    Usuario(string username = "", string password = ""):
        username(username), password(password){}

    bool changePass(string oldPass, string newPass){
        if(password == oldPass){
            password = newPass;
            cout << "  Senha alterada com sucesso!";
            return true;
        } throw "Fail: a senha digitada nao confere com a atual.";
    }
    void addNote(string title, string text){
        if(!existsNote(title))
            notas[title] = Nota(title, text);
        else
            throw "Fail: ja existe nota com este titulo.";
    }
    bool existsNote(string title){
        auto it = notas.find(title);
        if(it != notas.end())
            return true;
        return false;
    }
    bool rmNote(string title){
        auto it = notas.find(title);
        if(it != notas.end()){
            notas.erase(it);
            return true;
        } throw "Fail: nota ha nota com este titulo.";
    }
    void showNotes(){
        cout << "  " + username << endl;
        for(auto& it : notas)
            cout << "  " + it.second.toString() << endl;
    }
    void toString(){
        cout << "  " + username + ":" + password;
    }
    // friend Usuario Sistema::login(string uid, string pass);
    friend class Sistema;
};

class Sistema{
    Usuario* current = nullptr;
    map<string, Usuario> usuarios;
public:
    bool addUser(string userId, string password){
        if(!exists(userId)){
            usuarios[userId] = Usuario(userId, password);
            return true;
        }
        throw "Fail: usuario ja cadastrado.";
    }
    bool exists(string userId){
        auto it = usuarios.find(userId);
        if(it != usuarios.end())
            return true;
        return false;
    }
    Usuario* login(string uid, string pass){
        current = getUser(uid);
        if(current->username != uid)
            throw "Fail: username nao cadastrado.";
        else if(current->password != pass)
            throw "Fail: senha incorreta.";
        return current;
    }
    Usuario* getUser(string uid){
        auto it = usuarios.find(uid);
        if(usuarios.find(uid) != usuarios.end());
            return &it->second;
        throw "Fail: usuario nao cadastrado.";
    }
    void toString(){
        cout << "  Usuarios: [ ";
        for(auto pair: usuarios)
            cout << pair.second.username + " ";
        cout << "]";
    }
};

class Controlador{
    Sistema mySis;
    Usuario* currentUser = nullptr;
public:
    string interpretador(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;
    
        try{
            if(currentUser == nullptr && op != "login" && op != "addUser" && op != "showUsers" && op != "help"){
                throw "Fail: efetue login ou crie uma conta para acessar o sistema.";
            }
            else if(op == "help"){
                cout
                << "  login _username _password \n"
                << "  addUser _username _password \n"
                << "  showUsers \n"
                << "  logout \n"
                << "  whois \n"
                << "  changePass _oldPass _newPass \n"
                << "  addNote _title _text \n"
                << "  rmNote _title \n"
                << "  showNotes";
            }
            else if(op == "login"){
                string userId, password;
                in >> userId;
                in >> password;
                currentUser = mySis.login(userId, password);
                out << "Seja bem-vindo " + userId + "!";
            }
            else if(op == "logout"){
                currentUser = nullptr;
                out << "Deslogado com sucesso!";
            }
            else if(op == "whois"){
                currentUser->toString();
            }                    
            else if(op == "addUser"){
                string userId, password;
                in >> userId;
                in >> password;
                mySis.addUser(userId, password);
                out << "Usuario cadastrado com sucesso!";
            }
            else if(op == "changePass"){
                string oldPass, newPass;
                in >> oldPass >> newPass;
                currentUser->changePass(oldPass, newPass);
            }
            else if(op == "addNote"){
                string title, text;
                in >> title;
                getline(in, text);
                currentUser->addNote(title, text);
                out << "Nota adicionada com sucesso!";
            }
            else if(op == "rmNote"){
                string title;
                in >> title;
                currentUser->rmNote(title);
                out << "Nota removida com sucesso!";
            }
            else if(op == "showNotes"){
                currentUser->showNotes();
            }
            else if(op == "showUsers"){
                mySis.toString();
            }
            else
                throw "Fail: comando invalido, \"help\" para ajuda!";
        } catch(char const* e){
            out << e;
        } catch(string e){
            out << e;
        } catch(...){
            out << "Fail: ocorreu uma exceção.";
        }    

        return out.str();
    }

    void coletor(){
        ifstream arquivo ("anotacoes.txt");
        string line;

        if(arquivo.is_open()){
            while(!arquivo.eof()){
                getline(arquivo, line);
                if(line == "manual"){
                    while(line != "end"){
                        getline(cin, line);
                        cout << "  " << interpretador(line) << endl;
                    }
                }
                else if(line == "end")
                    break;
                cout << line << endl;
                cout << "  " << interpretador(line) << endl;
            }
            arquivo.close();
        } else
            cout << "impossivel abrir o arquivo!";
    }
};

int main(){
    Controlador c;
    c.coletor();
    return 0;
}