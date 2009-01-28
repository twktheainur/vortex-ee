#ifndef COMMENTAIRE_H_
#define COMMENTAIRE_H_

#include "Utilisateur.h"
#include "Document.h"
#include "Base.h"
#include <mysql++.h>
#include <string>
using namespace std;

class Commentaire
{ // idUser, idDoc, numChrono, texte
    public:
		Commentaire(string user, string doc);
		//Constructeur pour crer un nouveau commentaire
		
		void setTexte();
	
    private:
		string user;
		string doc;
		string texte;
		int numCom;
		MYSQL *pConnection;
};

#endif
