#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Controlador{
public:
    string interpretador(string line){
        stringstream in(line);
        stringstream out;
        string op;

        in >> op;
    
        try{
            if(op == "addAlu"){
            } else if(op == "addPro"){
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
        ifstream arquivo ("command.txt");
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