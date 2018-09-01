#include <iostream>
#include <sstream> /*fluxo de texto, trabalhar com */
#include <iomanip> /* manipulador de 'io' entrada e saída de dados setprecision(#) fixed*/
#include <cstdio>

using namespace std;

main(){
    string line;
    getline(cin, line);

    stringstream ss(line);
    string palavra;
    ss >> palavra;

    /*
    while(ss >> palavra){
        for(char letra : palavra){
            if( letra == ':'){
                ss >> palavra;
                cout << palavra;
            }
        }
    }
    */

   /*
    while(ss >> palavra){
        stringstream ss(palavra);
        string nome, fone;
        getline(palavra, nome);
        getline(palavra, fone);

    }*/

    cout << fixed << setprecision(5) << 2334.4645547;


    return 0;
}