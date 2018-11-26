#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Tweet{
    int id;
    string username, text;
public:
    Tweet(int id = 0, string username = "", string text = "")
        : id(id), username(username), text(text){}

    string toString(){
        return "[" + to_string(id) + ":" + username + ":" + text + "]";
    }
};

class User{
    string username;
    int queue;
    vector<Tweet*> v_timeline;
    vector<User*> v_seguidos;
    vector<User*> v_seguidores;
public:
    User(string username = ""):
        username(username){}
    ~User(){}
    void seguir(User * seguido){
        this->v_seguidos.push_back(seguido);
        seguido->v_seguidores.push_back(this);
    }
    string seguidos(){
        stringstream ss;
        for(auto sigo : v_seguidos)
            ss << sigo->toString() + " ";
        return ss.str();
    }
    string seguidores(){
        stringstream ss;
        for(auto seguidor : v_seguidores)
            ss << seguidor->toString() + " ";
        return ss.str();
    }
    void setTimeline(Tweet * twt){
        this->v_timeline.push_back(twt);
        this->queue++;
        for(auto seguidor : v_seguidores){
            seguidor->v_timeline.push_back(twt);
            seguidor->queue++;
        }
    }
    string unread(){
        stringstream ss;
        for(size_t i = v_timeline.size()-1; i > v_timeline.size() - queue; i--){
            ss << this->v_timeline[i]->toString() + "\n";
        }
        this->queue = 0;
        return ss.str();
    }
    string toString(){
        return username + "\n";
    }
};

template<typename T>
class Repository{
    map<string, T> data; 
public:
    Repository(){}

    void add(string k, T v){
        data[k] = v;
    }
    void rem(string k){
        auto it  = data.find(k);
        if(it == data.end())
            throw "nao existe";
        data.erase(it);
        delete it;
    }
    bool exists(string k){
        auto it  = data.find(k);
        if(it == data.end())
            return false;
        return true;
    }
    T* getT(string k){
        auto it = data.find(k);
        if(it == data.end())
            throw "nao existe";
        return &it->second;
    }
    vector<T*> getValues(){
        vector<T*> v_data;
        for(auto pair : data)
            v_data.push_back(pair.second);
        return v_data;
    }
    string toString(){
        stringstream ss;
        for(auto pair : data)
            ss << pair.second.toString();
        return ss.str();
    }
};

class TweetGenerator{
    int nextId = {0};
    Repository<Tweet> * p_r_tweet;
public:    
    TweetGenerator(Repository<Tweet>* r_tweet){
        this->p_r_tweet = r_tweet;
    }
    Tweet* createtwt(string username, string text){
        p_r_tweet->add(to_string(nextId), Tweet(nextId, username, text));
        nextId++;
        return p_r_tweet->getT(username);
    }
};

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
            }
            else if(op == "unread"){
                string user;
                in >> user;
                out << r_user->getT(user)->unread();
            }
            else if(op == "myTweets"){
            }
            else if(op == "like"){
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