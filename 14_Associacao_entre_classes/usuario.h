#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

class Message{
    string send, title, text;
public:
    Message(string send, string title, string text)
        : send(send), title(title), text(text){}
    
    ~Message(){};
    
    string getSend(){ return send; };
    string getTitle(){ return title; };
    string getText(){ return text; };
};

class User{   
    string name;
    vector<Message> inbox;
public:
    virtual bool findReceiver(string recev) = 0;
    
    User(string name)
        : name(name){}
    virtual ~User(){};

    virtual string getName(){ return name; }

    void addMessage(string send, string title, string msg){
        if(findReceiver(send))
            inbox.push_back(Message(send, title, msg));
        else
            throw "destinatario invalido";
    }

    void showInbox(){
        stringstream ss;
        if(inbox.empty())
            throw "caixa de entrada vazia";
        else
            for(int i = inbox.size() - 1; i>=0; i--){
                ss << "  [" + inbox[i].getSend() + ":" +
                inbox[i].getTitle() + "]" + inbox[i].getText() + "\n";
                inbox.pop_back();
            }
        cout << ss.str();
    }

    friend class Doctor;
    friend class Patient;
};

#endif