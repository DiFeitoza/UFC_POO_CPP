#include "paciente.h"
#include "medico.h"

#include <iostream>

void Patient::addDoctor(Doctor *doc){
	for(auto * doctor : m_doctor)
		if(doctor->getName() == doc->getName())
			return;
	m_doctor.push_back(doc);
}

void Patient::rmDoctor(std::string docId){
	for(size_t i = 0; i < m_doctor.size(); i++){
		if(m_doctor[i]->getName() == docId){
			m_doctor.erase(m_doctor.begin() + i);
			return;
		}
	}
}

bool Patient::findReceiver(std::string receiv){
	for(size_t i = 0; i < m_doctor.size(); i++)
		if(m_doctor[i]->getName() == receiv)
			return true;
	return false;
}
 
std::ostream& operator<<(std::ostream &out, Patient *pat){
	size_t length = pat->m_doctor.size();
	out << pat->getName() << "[ ";
	for (size_t count = 0; count < length; ++count)
		out << pat->m_doctor[count]->getName() << ' ';
	out << "]";
	return out;
}