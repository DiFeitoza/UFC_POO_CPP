#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>

#include "tweet.h"
#include "user.h"
#include "repository.h"
#include "tweetgenerator.h"

using namespace std;

class Controller {
    Repository<User>* r_user;
    Repository<Tweet>* r_tweet;
    TweetGenerator* g_tweet;
public:
    Controller(){
        this->r_user = new Repository<User>;
        this->r_tweet = new Repository<Tweet>;
        this->g_tweet = new TweetGenerator(r_tweet);
    }
    string shell(string line){
        stringstream in(line);
        stringstream out;
        string op;
        in >> op;
        try{
            if(op == "addUser"){
                string username;
                in >> username;
                r_user->add(username, User(username));
            }
            else if(op == "users"){
                out << r_user->toString();
            }
            else if(op == "seguir"){
                string seguidor, seguido;
                in >> seguidor >> seguido;
                r_user->getT(seguidor)->seguir(r_user->getT(seguido));
            }
            else if(op == "twittar"){
                string username, text;
                in >> username;
                getline(in, text);
                auto twt = g_tweet->createtwt(username, text);
                r_user->getT(username)->setTimeline(twt);
            }
            else if(op == "seguidores"){
                string user;
                in >> user;
                out << r_user->getT(user)->seguidores();
            }
            else if(op == "seguidos"){
                string user;
                in >> user;
                out << r_user->getT(user)->seguidos();
            }
            else if(op == "timeline"){
                string user;
                in >> user;
                out << r_user->getT(user)->timeline();
            }
            else if(op == "unread"){
                string user;
                in >> user;
                out << r_user->getT(user)->unread();
            }
            else if(op == "myTweets"){
                string user;
                in >> user;
                out << r_user->getT(user)->myTweets();
            }
            else if(op == "like"){
                string username, idtwt;
                in >> username >> idtwt;
                r_tweet->getT(idtwt)->like(username);
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
				cout << line << endl;
				cout << shell(line) << endl;
			}
			arquivo.close();
		} else
			cout << "  fail: impossivel abrir o arquivo \".txt\" de execucao";
	}
};

#endif