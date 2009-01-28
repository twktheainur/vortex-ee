#ifndef BASE_H_
#define BASE_H_
#include <string>
#include <mysql++.h>
#include <stdio.h>
#include "Document.h"
#include "Commentaire.h"
#include "Utilisateur.h"
using namespace std;

class Base
{
    public:
		Base();
		//ouvre la base... peut tre des paramtres  ajouter...
	
		~Base();
		// votre avis?
	
		bool documentExiste(string id);
		//renvoie vrai si le document existe deja dans la base
			
		bool utlisateurExiste(string id);
		//renvoie vrai si l'utilisateur existe deja dans la base
	
    private:

	MYSQL *pConnection;
	
};

#endif 
