#ifndef MEDICO_H
#define MEDICO_H

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include "paciente.h"
#include "usuario.h"

class Doctor : public User{
private:
	std::vector<Patient *> m_patient;

public:
	Doctor(std::string name = "")
		: User(name){}
  	
	~Doctor(){}

	void addPatient(Patient *pat){
		//verifica se nao possui esse pat
		// Our doctor will add this patient
		m_patient.push_back(pat);
		
		// and the patient will also add this doctor
		pat->addDoctor(this);
	}

	void rmPatient(std::string patId){
		for(size_t i = 0; i < m_patient.size(); i++)
			if(m_patient[i]->getName() == patId){
				m_patient[i]->rmDoctor(getName());
				m_patient.erase(m_patient.begin() + i);
				return;
			}
	}

	void selfUntie(){
		for(size_t i = 0; i < m_patient.size(); i++){
			m_patient[i]->rmDoctor(getName());
			return;
		}
	}

	virtual bool findReceiver(string receiv){
		for(size_t i = 0; i < m_patient.size(); i++)
			if(m_patient[i]->getName() == receiv)
				return true;
		return false;
	}

	friend std::ostream& operator<<(std::ostream &out, Doctor * doc);
};

#endif