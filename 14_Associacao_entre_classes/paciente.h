#ifndef PACIENTE_H
#define PACIENTE_H

#include <iostream>
#include <string>
#include <vector>
#include "usuario.h"
 
// Since these classes have a circular dependency, we're going to forward declare Doctor
class Doctor;
 
class Patient : public User{
private:
	std::vector<Doctor *> m_doctor; // so that we can use it here
 
	// We're going to make addDoctor private because we don't want the public to use it.
	// They should use Doctor::addPatient() instead, which is publicly exposed
	// We'll define this function after we define what a Doctor is
	// Since we need Doctor to be defined in order to actually use anything from it
	void addDoctor(Doctor *doc);
	void rmDoctor(std::string docId);
	virtual bool findReceiver(std::string recev);
 
public:
	Patient(std::string name = "")
		: User(name){}

	~Patient(){}

	// We'll implement this function below Doctor since we need Doctor to be defined at that point
	friend std::ostream& operator<<(std::ostream &out, Patient * pat);

	// We're friending Doctor so that class can access the private addDoctor() function
	// (Note: in normal circumstances, we'd just friend that one function, but we can't
	// because Doctor is forward declared)
	friend class Doctor;
};

#endif