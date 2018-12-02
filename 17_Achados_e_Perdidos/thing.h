#ifndef THING_H_
#define THING_H_

#include <iostream>

class User;

using namespace std;

class Thing{
protected:
	string id, category, where_found, description;
	vector<string> v_matches;
	User ** owner = nullptr;
public:
	Thing(string id, string cat, string where, string desc, User ** own)
		: id(id), category(cat), where_found(where), description(desc), owner(own){}

	virtual string getId() = 0;
	virtual string getCategory() = 0;
	virtual string getWhereFound() = 0;
	virtual string getDescription() = 0;
	virtual vector<string> getMatches() = 0;
	virtual User * getOwner() = 0;
	virtual void setMatch(string match_id) = 0;
	virtual string toString() = 0;
};

class BemMaterial : public Thing{
public:
	BemMaterial(string id, string cat, string where, string desc, User** own)
		: Thing(id, cat, where, desc, own){}

	virtual string getId(){	return id; }
	virtual string getCategory(){ return category; }
	virtual string getWhereFound(){ return where_found; }
	virtual string getDescription(){ return description; }
	virtual vector<string> getMatches(){ return v_matches; }
	virtual User * getOwner(){ return *owner; }

	virtual void setMatch(string match_id){
		v_matches.push_back(match_id);
	}

	virtual string toString(){
		stringstream ss;
		ss << "id: " + id + "\n"
		+ "Categoria: " + category + "\n"
		+ "Descrição: " + description + "\n"
		+ "Localizado: " + where_found;
		return ss.str();
	}
};

/* class Pet : public Thing{
	string raca, nome;
public:
	Pet(string id = "", string cat = "", string where = "", string desc = "", string raca = "", string nome = ""):
		Thing(id, cat, where, desc), raca(raca), nome(nome){}

	virtual string getId(){	return id; }
	virtual string getCategory(){ return category; }
	virtual string getWhereFound(){ return where_found; }
	virtual string getDescription(){ return description; }

	virtual string toString(){
		stringstream ss;
		ss << id + ":" + description;
		return ss.str();
	}
}; */


#endif