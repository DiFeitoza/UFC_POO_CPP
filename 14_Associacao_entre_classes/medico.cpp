#include "medico.h"

std::ostream& operator<<(std::ostream &out, Doctor * doc){
    size_t length = doc->m_patient.size();
    out << doc->getName() << "[ ";
    for (size_t count = 0; count < length; ++count)
            out << doc->m_patient[count]->getName() << ' ';
    out << "]";
    return out;
}