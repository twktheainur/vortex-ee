#include "Utilisateur.h"
#include <mysql++.h>
using namespace std;


		Utilisateur::Utilisateur()
		{
			
		}
		//constructeur par dfaut (quand on a besoin de crer un utilisateur)
	
		Utilisateur::Utilisateur(string id)
		{
			 MYSQL_RES *pResult; //pointer to the result set structure
			 MYSQL_ROW Row;
			 mysql_query(pConnection,"SELECT identifiant FROM Utilisateur WHERE identifiant='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 identifiant=Row[0];
			 
			 mysql_query(pConnection,"SELECT nom FROM Utilisateur WHERE identifiant='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 nom=Row[0];
			 
			 mysql_query(pConnection,"SELECT prenom FROM Utilisateur WHERE identifiant='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 prenom=Row[0];

			 mysql_query(pConnection,"SELECT password FROM Utilisateur WHERE identifiant='id'"); //query the database

			 pResult = mysql_use_result(pConnection);
			 Row = mysql_fetch_row(pResult);
			 password=Row[0];
		}
		//le constructeur d'un Utilisateur dj prsent dans la base
	
		string Utilisateur::getPassword()
		{
			return password;
		}
		//Renvoie le mot de passe de l'utilisateur en clair en rcuprant son hash md5 dans la base
				
		string Utilisateur::getIdUser()
		{
			return identifiant;
		}
		//renvoie l'id de l'utilisateur  partir de l'attribut
		
		string Utilisateur::getNom()
		{
			return nom;
		}
		//renvoie le nom  partir de la base
		
		string Utilisateur::getPrenom()
		{
			return prenom;
		}
		//renvoie le prnom  partir de la base

