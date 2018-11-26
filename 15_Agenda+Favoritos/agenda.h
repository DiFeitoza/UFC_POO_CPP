#ifndef AGENDA_H
#define AGENDA_H

#include "entry.h"
#include <map>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

class Agenda {
    map<string, Entry*> m_entries;
    map<string, Entry*> m_favorites;
public:
    void addEntry(Entry * entry){
        string id = entry->getId();
        m_entries[id] = entry;
    }
    void rmEntry(string id){
        m_entries.erase(id);
        m_favorites.erase(id);
    }
    void favorite(string id){
        Entry * entry = getEntry(id);
        entry->setFavorited(true);
        m_favorites[id] = entry;
    }
    void unfavorite(string id){
        getEntry(id)->setFavorited(false);
        m_favorites.erase(id);
    }
    bool exists(string id){
        auto it = m_entries.find(id);
        if(it != m_entries.end())
            return true;
        return false;
    }
    Entry * getEntry(string id){
        auto it = m_entries.find(id);
        if(it != m_entries.end())
            return it->second;
        throw "entrada " + id + " nao existe";
    }
    vector<Entry*> getEntries(){
        vector<Entry*> v_entries;
        for(auto pair : m_entries)
            v_entries.push_back(pair.second);
        return v_entries;
    }
    vector<Entry*> getFavorited(){
        vector<Entry*> v_favorites;
        for(auto pair : m_favorites)
            v_favorites.push_back(pair.second);
        return v_favorites;
    }
    vector<Entry*> search(string pattern){
        vector<Entry*> resp;
        for(auto& par : m_entries)
            if(par.second->toString().find(pattern) != string::npos)
                resp.push_back(par.second);
        return resp;
    }
    string toString(){
        stringstream ss;
        for(auto entry : getEntries())
            ss << entry->toString() << "\n";
        return ss.str();
    }
};

#endif