#include "Document.h"
#include <mysql++.h>


		Document::Document()
		{
			mysql_query(pConnection,"INSERT INTO Document(idDoc, auteur, type) VALUES(null,null,null)");
		}
		//constructeur par dfaut (quand on a besoin de crer un Document vide)
	
		Document::Document(string id)
		{
			 MYSQL_RES *pResult; //pointer to the result set structure
			 MYSQL_ROW Row;
			 mysql_query(pConnection,"SELECT idDoc FROM Document WHERE idDoc='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 idDoc=Row[0];
			 
			 mysql_query(pConnection,"SELECT auteur FROM Document WHERE idDoc='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 auteur=Row[0];
			 
		}
		//le constructeur d'un Document dj prsent dans la base
		
		string Document::getIdDoc()
		{
			return idDoc;
		}
		//renvoie l'id du doc  partir de l'attribut
		
		string Document::getAuteur()
		{
			return auteur;
		}
		//renvoie l'auteur du doc  partir de la base
		
		string Document::getType()
		{
			string tp;
			MYSQL_RES *pResult; //pointer to the result set structure
			MYSQL_ROW Row;
			mysql_query(pConnection,"SELECT type FROM Document WHERE idDoc='idDoc'");
			pResult = mysql_use_result(pConnection);
			Row = mysql_fetch_row(pResult);
			tp = Row[0];
			return tp;
		}
		//renvoie le type du doc  partir de la base
		
		void Document::setIdDoc(string doc)
		{
			mysql_query(pConnection,"UPDATE Document SET idDoc='doc' WHERE idDoc='null'");
		}
		//affecte un id au doc (attribut de la classe)
		
		void Document::setAuteur(string idAuteur)
		{
			mysql_query(pConnection,"UPDATE Document SET auteur='idAuteur' WHERE idDoc='idDoc'");
		}
		//affecte un idUser au doc dans la base
		
		
		void Document::setType(int tipe)
		{
			mysql_query(pConnection,"UPDATE Document SET type='tipe' WHERE idDoc='idDoc'");
		}			
		//affecte le type au doc {video,image,son} identifis par un chiffre dans la base
