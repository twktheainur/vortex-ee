#ifndef DOCUMENT_H_
#define DOCUMENT_H_

#include <string>
#include "Base.h"
#include <mysql++.h>
#include <stdio.h>
using namespace std;

class Document
{
    public:
    
		Document();
		//constructeur par dfaut (quand on a besoin de crer un Document vide)
	
		Document(string id);
		//le constructeur d'un Document dj prsent dans la base
		
		string getIdDoc();
		//renvoie l'id du doc  partir de l'attribut
		
		string getAuteur();
		//renvoie l'auteur du doc  partir de la base
		
		string getType();
		//renvoie le type du doc  partir de la base
		
		void setIdDoc(string idDoc);
		//affecte un id au doc (attribut de la classe)
		
		void setAuteur(string idAuteur);
		//affecte un idUser au doc dans la base
		
		void setCommentaire(string com);
		//affecte lecommentaire explicatif du document laiss par l'auteur au doc dans la base
		
		void setType(int type);
		//affecte le type au doc {video,image,son} identifis par un chiffre dans la base

    private:
		string idDoc; //nom physique du document
		string auteur;
		MYSQL *pConnection;
		enum type { VIDEO, SON, IMAGE };
		
};

#endif
