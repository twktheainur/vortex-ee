#include "Commentaire.h"
#include <mysql++.h>
using namespace std;

		Commentaire::Commentaire(string util, string docx) 
		{
			 MYSQL_RES *pResult; //pointer to the result set structure
			 
			 mysql_query(pConnection,"SELECT * FROM Commentaire"); //query the database

			 pResult = mysql_store_result(pConnection);
			 numCom = mysql_num_rows(pResult)+1;
			 
			 mysql_query(pConnection,"INSERT INTO Commentaire(idDoc, idUser, texte, numCom) VALUES('util','docx','null','numCom')");

			 user=util;
			 doc=docx;
		}
		//Constructeur pour crer un nouveau commentaire
		
		/*Commentaire::Commentaire(string user, string doc)
		{
			 MYSQL_RES *pResult; //pointer to the result set structure
			 MYSQL_ROW Row;
			 mysql_query(pConnection,"SELECT * FROM Commentaire WHERE idDoc='doc' and identifiant='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 doc=Row[0];
			 user=Row[1];
			 texte=Row[2];
		}*/
		
		void Commentaire::setTexte() 
		{
			
		}
