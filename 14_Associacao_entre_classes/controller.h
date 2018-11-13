#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "medico.h"
#include "paciente.h"
#include "usuario.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
using namespace std;

class Controller{
	map<string, User*> rUser;
public:
	Controller(){}

	string shell(string line){
		stringstream in(line);
		stringstream out;
		string op;
		in >> op;

		try{
			if(op == "addDoc"){ //_doc1 _doc2 _doc3 ...
				string name;
				while(in >> name){
					Doctor* doc = new Doctor(name);
					if(rUser.find(name) == rUser.end())
						rUser[name] = doc;
					else
						delete doc;
				}
			}
			else if(op == "addPat"){ //_pat1 _pat2 _pat3 ...
				string name;
				while(in >> name){
					Patient* pat = new Patient(name);
					if(rUser.find(name) == rUser.end())
						rUser[name] = pat;
					else
						delete pat;
				}
			}
			else if(op == "show"){
				cout << "Medicos" << endl;
				for(auto& par : rUser)
					if(auto doc = dynamic_cast<Doctor*>(par.second))
						cout << "  " << doc << endl;
				cout << "Pacientes" << endl;
				for(auto& par : rUser)
					if(auto pat = dynamic_cast<Patient*>(par.second))
						cout << "  " << pat << endl;
			}
			else if(op == "tie"){ //_doc _pat1 _pat2 ..
				string docName, patName;
				in >> docName;
				auto itDoc = rUser.find(docName);
				if(itDoc == rUser.end())
					throw "usuario " + docName + " nao existe";
				else if(Doctor * doc = dynamic_cast<Doctor*>(itDoc->second))
					while(in >> patName){
						auto itPat = rUser.find(patName);
						if(itPat == rUser.end())
							cout << "  fail: usuario " + patName + " nao existe" + "\n";
						else if(Patient* pat = dynamic_cast<Patient*>(itPat->second))
								doc->addPatient(pat);
						else
							cout << "  fail: paciente " + patName + " nao existe" + "\n";
					}
				else
					throw "medico " + docName + " nao existe";
			}
			else if(op == "untie"){ //_doc _pat1 _pat2 ...
				string docName, patName;
				in >> docName;
				auto itDoc = rUser.find(docName);
				if(itDoc == rUser.end())
					throw "paciente " + docName + "nao existe";
				else if (Doctor * doc = dynamic_cast<Doctor*>(itDoc->second))
					while(in >> patName){
						auto itPat = rUser.find(patName);
						if(itPat == rUser.end())
							throw "paciente " + patName + "nao existe";
						doc->rmPatient(patName);
					}
			}
			else if(op == "rmPat"){ //_pat
				string patName;
				in >> patName;
				auto itPat = rUser.find(patName);
				if(itPat == rUser.end())
					throw "usuario " + patName + "nao existe";
				else if(dynamic_cast<Patient*>(itPat->second)){
					for(auto& pair : rUser)
						if(Doctor * doc = dynamic_cast<Doctor*>(pair.second))
							doc->rmPatient(patName);
				} else
					throw "paciente " + patName + "nao existe";
				delete itPat->second;
				rUser.erase(itPat);
			}
			else if(op == "rmDoc"){ //_doc
				string docName;
				in >> docName;
				auto itDoc = rUser.find(docName);
				if(itDoc == rUser.end())
					throw "usuario " + docName + "nao existe";
				else if(Doctor * doc = dynamic_cast<Doctor*>(itDoc->second))
					doc->selfUntie();
				else
					throw "medico " + docName + " nao existe";
				delete itDoc->second;
				rUser.erase(docName);
			}
			else if(op == "sendMsg"){ //_user _user _title _text
				string send, to, title, text;
				in >> send >> to >> title;
				getline(in, text);
				auto itSend = rUser.find(send);
				if(itSend == rUser.end())
					throw "remetente " + send + " nao existe";
				auto itTo = rUser.find(to);
				if(itTo == rUser.end())
					throw "destinatario " + to + " nao existe";
				if(itSend->second->findReceiver(itTo->second->getName())){
					itTo->second->addMessage(send, title, text);
					out << "mensagem enviada com sucesso!";
				} else
					throw "destinatario nao esta na lista de contatos";
			}
			else if(op == "showInbox"){
				string user;
				in >> user;
				auto itUser = rUser.find(user);
				if(itUser == rUser.end())
					throw "usuario " + user + " nao existe";
				itUser->second->showInbox();
			}
			else
				throw "comando invalido";
		}
		catch(string e){ out << "  fail: " + e; }
		catch(char const* e){ out << "  fail: " << e; }
		catch(...){ out << "  fail: ocorreu uma excecao"; }
		return out.str();
	}

	void coletor(){
		ifstream arquivo ("associacao.txt");
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

#endif