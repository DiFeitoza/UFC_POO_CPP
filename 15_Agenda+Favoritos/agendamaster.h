#ifndef AGENDAMASTER_H
#define AGENDAMASTER_H

#include "note.h"
#include "contato.h"
#include "agenda.h"

using namespace std;

class AgendaMaster : public Agenda{
public:
    Contato * getContato(string id){
        if(Contato * cont = dynamic_cast<Contato*>(getEntry(id)))
            return cont;
        throw "entrada " + id + " nao e um contato";
    }
    Note * getNote(string id){
        if(Note * note = dynamic_cast<Note*>(getEntry(id)))
            return note;
        throw "entrada " + id + " nao e uma nota";
    }
};

#endif