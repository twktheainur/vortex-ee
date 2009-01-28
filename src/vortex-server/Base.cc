#include "Base.h"
#include <mysql++.h>
using namespace std;


		Base::Base()
				{
		pConnection = mysql_init(NULL);
		if(!pConnection) //error, quit the program
		exit(0);
		printf("Attempting to connect\n");
		if(mysql_real_connect(pConnection,"http://sql.free.fr","vortexdb","6vortex9","vortexdb",0,NULL,0) == NULL) //error, quit the program
		exit(0);
		printf("Connected to the database\n");
		}
		//ouvre la base... peut tre des paramtres  ajouter...
	
		Base::~Base()
		{
			mysql_close(pConnection);
			printf("Connection Terminated\n"); 
		}
		// votre avis?
	
		bool Base::documentExiste(string id)
		{
			 MYSQL_RES *pResult; //pointer to the result set structure
			 
			 bool result;
			 mysql_query(pConnection,"SELECT * FROM Document WHERE idDocument=id"); //query the database

			 pResult = mysql_store_result(pConnection);
			 if (mysql_num_rows(pResult)==1)
			 {
			 	result=true;
			 	mysql_free_result(pResult);
			 }
			 else
			 	result=false;
			 	
			 return result;
		}
		//renvoie vrai si le document existe deja dans la base 
			
		bool Base::utlisateurExiste(string id)
		{
			 MYSQL_RES *pResult; //pointer to the result set structure
			 
			 bool result;
			 mysql_query(pConnection,"SELECT * FROM Utilisateur WHERE identifiant=id"); //query the database

			 pResult = mysql_store_result(pConnection);
			 if (mysql_num_rows(pResult)==1)
			 {
			 	result=true;
			 	mysql_free_result(pResult);
			 }
			 else
			 	result=false;
			 	
			 return result;
		}
		 
 
 
