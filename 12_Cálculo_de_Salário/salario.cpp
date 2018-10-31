#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

class{};
class{};
class{};

class Controlador{

public:
    string interpretador(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;
    
        try{
            if(op == ""){
            }
            else if(op == ""){
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

    void coletor(){
        ifstream arquivo ("salario.txt");
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