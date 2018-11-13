#include <iostream>
#include <string>
#include <vector>
 
// Since these classes have a circular dependency, we're going to forward declare Doctor
class Doctor;
 
class Patient
{
private:
	std::string m_name;
	std::vector<Doctor *> m_doctor; // so that we can use it here
 
	// We're going to make addDoctor private because we don't want the public to use it.
	// They should use Doctor::addPatient() instead, which is publicly exposed
	// We'll define this function after we define what a Doctor is
	// Since we need Doctor to be defined in order to actually use anything from it
	void addDoctor(Doctor *doc);
 
public:
	Patient(std::string name):
		m_name(name)
	{
	}
 
	// We'll implement this function below Doctor since we need Doctor to be defined at that point
	friend std::ostream& operator<<(std::ostream &out, const Patient &pat);
 
	std::string getName() const { return m_name; }
 
	// We're friending Doctor so that class can access the private addDoctor() function
	// (Note: in normal circumstances, we'd just friend that one function, but we can't
	// because Doctor is forward declared)
	friend class Doctor;
};
 
class Doctor
{
private:
	std::string m_name;
	std::vector<Patient *> m_patient;
 
public:
	Doctor(std::string name):
		m_name(name)
	{
	}
 
	void addPatient(Patient *pat)
	{
		// Our doctor will add this patient
		m_patient.push_back(pat);
		
		// and the patient will also add this doctor
		pat->addDoctor(this);
	}
 
 
	friend std::ostream& operator<<(std::ostream &out, const Doctor &doc)
	{
		unsigned int length = doc.m_patient.size();
		if (length == 0)
		{
			out << doc.m_name << " has no patients right now";
			return out;
		}
 
		out << doc.m_name << " is seeing patients: ";
		for (unsigned int count = 0; count < length; ++count)
			 out << doc.m_patient[count]->getName() << ' ';
 
		return out;
	}
 
	std::string getName() const { return m_name; }
};
 
void Patient::addDoctor(Doctor *doc)
{
	m_doctor.push_back(doc);
}
 
std::ostream& operator<<(std::ostream &out, const Patient &pat)
{
	unsigned int length = pat.m_doctor.size();
	if (length == 0)
	{
		out << pat.getName() << " has no doctors right now";
		return out;
	}
 
	out << pat.m_name << " is seeing doctors: ";
	for (unsigned int count = 0; count < length; ++count)
		out << pat.m_doctor[count]->getName() << ' ';
 
	return out;
}

int main()
{
	// Create a Patient outside the scope of the Doctor
	Patient *p1 = new Patient("Dave");
	Patient *p2 = new Patient("Frank");
	Patient *p3 = new Patient("Betsy");
 
	Doctor *d1 = new Doctor("James");
	Doctor *d2 = new Doctor("Scott");
 
	d1->addPatient(p1);
 
	d2->addPatient(p1);
	d2->addPatient(p3);
 
	std::cout << *d1 << '\n';
	std::cout << *d2 << '\n';
	std::cout << *p1 << '\n';
	std::cout << *p2 << '\n';
	std::cout << *p3 << '\n';
 
	delete p1;
	delete p2;
	delete p3;
	
	delete d1;
	delete d2;
 
	return 0;
}