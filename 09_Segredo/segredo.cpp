#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Segredo{
    int nivel;
    string texto;
public:
    Segredo(int nivel = 0, string descricao = ""){
        setNivel(nivel);
        this->texto = descricao;
    }
  
    void setNivel(int value){
        if((value < 0) || (value > 10))
            throw "fail: nivel invalido";
        this->nivel = value;
    }

    void setSegredo(string newSec){
        this->texto = newSec;
    }

    string toString(){
        return "[" + texto + "] " + to_string(nivel); 
    }
};

class User{
    string id;
    string pass;
    Segredo seg;
public:
    User(string id = "", string pass = ""){
        this->id = id;
        this->pass = pass;
    }
    string getId(){
        return id;
    }
    string toString(){
        return id + seg.toString(); 
    }
    bool verifyPass(string pass){
        return this->pass == pass;
    }
    void changePass(string oldPass, string newPass){
        if(this->pass == oldPass)
            this->pass = newPass;
        else
            throw "fail: senha invalida";
    }
};

class Sistema{
    vector<User> usuarios;
    string adminName;
public:
    Sistema(){
        adminName = "admin";
        usuarios.push_back(User("admin", "admin"));
    }
    
    User& getUser(string id){
        for(auto& user : usuarios)
          if(user.getId() == id)
                return user;
        throw "fail: usuario nao existe";
    }
  
    void addUser(User user){
        for(auto& other : usuarios)
          if(user.getId() == other.getId())
                throw "fail: id ja existe";
        usuarios.push_back(user);
    }
};

class GerLogin{
    User * current;
    Sistema * sistema;
public:
    GerLogin(Sistema * sistema){
        this->sistema = sistema;     
    }
  
    User& getCurrent(){
        if(current == nullptr)
            throw "fail: ninguem logado";
        return *current;
    }
  
    void login(string login, string pass){
        if(current != nullptr)
            throw "fail: ja existe alguem logado";
        User& user = sistema->getUser(login);
        if(!user.verifyPass(pass))
            throw "fail: senha invalida";
        current = &user;
    }
  
    void logout(){
        if(current == nullptr)
            throw "fail: ninguem logado";
        current = nullptr;
    }
};

class Controller{
    Sistema sist;
    GerLogin gerLog;
    User * current;
public:
    Controller():
      gerLog(&sist)
    {
        current = nullptr;
    }
  
    void shell(string line){
        stringstream in(line);
        string op;
        in >> op;
        if(op == "addUser"){//_nome _senha
            string nome, pass;
            in >> nome >> pass;
            sist.addUser(User(nome, pass));
        }else if(op == "login"){//_id _pass
            string nome, pass;
            in >> nome >> pass;
            gerLog.login(nome, pass);
        }else if(op == "logout"){
            gerLog.logout();
        }else if(op == "show"){
            cout << gerLog.getCurrent().toString() << endl;
        }else if(op == "changePass"){//_old _new
            string old, newPass;
            in >> old >> newPass;
            gerLog.getCurrent().changePass(old, newPass);
        }else if(op == "segredo"){
            int level;
            string newSec;
            in >> level;
            getline(in, newSec);
            gerLog.getCurrent().setSegredo(newSec);
        }
    };
    
    void exec(){
        string line;
        
        while(true){
            getline(cin, line);
            cout << "$" << line << endl;
            if(line == "end")
                return;
            try{
                shell(line);
            }catch(const char * e){
                cout << e << endl;
            }
        }
    }
};

int main(){
    Controller c;
    c.exec();
    return 0;
}