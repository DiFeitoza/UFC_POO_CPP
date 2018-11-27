#ifndef USER_H_
#define USER_H_

#include <vector>
#include <sstream>
#include "tweet.h"

class User{
    string username;
    int queue = 0;
    vector<Tweet*> v_timeline;
    vector<Tweet*> v_myTweets;
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
        for(auto it = v_seguidos.begin(); it != v_seguidos.end(); it++){
            if(it != v_seguidos.end()-1) ss << (*it)->username + " ";
            else ss << (*it)->username;
        }
        return ss.str();
    }
    string seguidores(){
        stringstream ss;
        for(auto it = v_seguidores.begin(); it != v_seguidores.end(); it++){
            if(it != v_seguidores.end()-1) ss << (*it)->username + " ";
            else ss << (*it)->username;
        }
        return ss.str();
    }
    void setTimeline(Tweet * twt){
        this->v_myTweets.push_back(twt);
        for(auto seguidor : v_seguidores){
            seguidor->v_timeline.push_back(twt);
            seguidor->queue++;
        }
    }
    string unread(){
        stringstream ss;
        if(queue == 0)
            return "  Nao ha novos tweets";
        for(int i = 1; i <= queue; i++){
            ss << v_timeline[v_timeline.size()-i]->toString() + "\n";
        }
        this->queue = 0;
        return ss.str();
    }
    string myTweets(){
        stringstream ss;
        if(v_myTweets.size() == 0)
            return "  Que tal publicar um tweet?";
        for(auto it = v_myTweets.end()-1; it >= v_myTweets.begin(); it--)
            ss << (*it)->toString() + "\n";
        return ss.str();
    }
    string timeline(){
        stringstream ss;
        if(v_timeline.size() == 0)
            return "  Nao ha tweets aqui :(";
        for(auto it = v_timeline.end()-1; it >= v_timeline.begin(); it--)
            ss << (*it)->toString() + "\n";
        this->queue = 0;
        return ss.str();
    }
    string toString(){
        stringstream ss;
        ss << "  " + this->username + "\n";
        ss << "    seguidos\t[";
        ss << this->seguidos() + "]\n";
        ss << "    seguidores\t[";
        ss << this->seguidores();
        return ss.str() + "]\n";
    }
};

#endif