#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "entry.h"
#include "note.h"
#include "fone.h"
#include "contato.h"
#include "agenda.h"
#include "agendamaster.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class Controller {
    AgendaMaster agenda;
public:
    string shell(string line){
        stringstream ss(line);
        stringstream out;
        string op;
        ss >> op;

        try{
            if(op == "addContato"){
                string name, id_number;
                ss >> name;
                
                if(!agenda.exists(name)){
                    Contato* cont = new Contato(name);
                    while(ss >> id_number){
                        string id, fone;
                        stringstream ssfone(id_number);
                        getline(ssfone, id, ':');
                        ssfone >> fone;
                        cont->addFone(Fone(id, fone));
                    }
                    agenda.addEntry(cont);
                }
                else{
                    Contato * cont = agenda.getContato(name);
                    while(ss >> id_number){
                        string id, fone;
                        stringstream ssfone(id_number);
                        getline(ssfone, id, ':');
                        ssfone >> fone;
                        cont->addFone(Fone(id, fone));
                    }
                }
            }
            else if(op == "rmFone"){
                string name;
                int indice;
                ss >> name >> indice;
                Contato * cont = agenda.getContato(name);
                cont->rmFone(indice);
            }
            else if(op == "addNota"){
                string title, item;
                ss >> title;

                if(!agenda.exists(title)){
                    Note * note = new Note(title);
                    while(ss >> item)
                        note->addItem(item);
                    agenda.addEntry(note);
                }
                else{
                    Note * note = agenda.getNote(title);
                    while(ss >> item)
                        note->addItem(item);
                }
            }
            else if(op == "rmItem"){
                string name;
                int indice;
                ss >> name >> indice;
                Note * note = agenda.getNote(name);
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
                    out << entry->toString() << endl;
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
		ifstream arquivo ("input.txt");
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

#endif