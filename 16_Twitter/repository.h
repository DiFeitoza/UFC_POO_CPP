#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include <map>

template<typename T>
class Repository{
    map<string, T> data;
public:
    void add(string k, T v){
        data[k] = v;
    }
    void rem(string k){
        auto it  = data.find(k);
        if(it == data.end())
            throw "entrada " + k + " nao existe";
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
            throw "entrada " + k + " nao existe";
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

// class RepoMaster : public Repository{};

#endif