#ifndef UTILISATEUR_H_
#define UTILISATEUR_H_

#include <string>
#include "Document.h"
#include "Base.h"
#include "Commentaire.h"
using namespace std;

class Utilisateur
{ // idUser, nom, prenom, pass
    public:
		Utilisateur();
		//constructeur par dfaut (quand on a besoin de crer un utilisateur)
	
		Utilisateur(string id);
		//le constructeur d'un Utilisateur dj prsent dans la base
	
		string getPassword();
		//Renvoie le mot de passe de l'utilisateur en clair en rcuprant son hash md5 dans la base
				
		string getIdUser();
		//renvoie l'id de l'utilisateur  partir de l'attribut
		
		string getNom();
		//renvoie le nom  partir de la base
		
		string getPrenom();
		//renvoie le prnom  partir de la base
		
		string getMotDePasse();
		//renvoie le pass en clair  partir du hash dans la base


    private:
		string identifiant;
		string nom;
		string prenom;
		string password;
		MYSQL *pConnection;
};

#endif
