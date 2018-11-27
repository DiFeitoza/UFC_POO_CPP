#ifndef TWEET_H_
#define TWEET_H_

#include <vector>
#include <sstream>
using namespace std;

class Tweet{
    int id;
    string username, text;
    vector<string> v_usrlike;
public:
    Tweet(int id = 0, string username = "", string text = "")
        : id(id), username(username), text(text){}
    ~Tweet(){}
    string getUsername(){
        return username;
    }
    void like(string username){
        auto it = find(v_usrlike.begin(), v_usrlike.end(), username);
        if(it == v_usrlike.end())
            v_usrlike.push_back(username);
        else
            v_usrlike.erase(it);
    }    
    string toString(){
        stringstream ss;
        ss << to_string(id) + " " + username + ": " + text + " {";
        for(auto it = v_usrlike.begin(); it != v_usrlike.end(); it++){
            if(it != v_usrlike.end()-1) ss << *it + " ";
            else ss << *it;
        }
        return ss.str() + "}";
    }
};

#endif
