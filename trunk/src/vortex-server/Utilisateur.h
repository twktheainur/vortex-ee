#ifndef UTILISATEUR
#define UTILISATEUR

class Utilisateur
{ // idUser, nom, prenom, pass
    public:
		Utlisateur();
		//constructeur par d�faut (quand on a besoin de cr�er un utilisateur)
	
		Utlisateur(String id);
		//le constructeur d'un Utilisateur d�j� pr�sent dans la base
			
		creerUtlisateur(String idUSer, String nom, String prenom, String pass);
		//ajoute l'utilisateur dans la base avec son id, nom, prenom, hash md5 du mot de pass en param�tre
	
		String getPassword();
		//Renvoie le mot de passe de l'utilisateur en clair en r�cup�rant son hash md5 dans la base
				
		String getIdUser();
		//renvoie l'id de l'utilisateur � partir de l'attribut
		
		String getNom();
		//renvoie le nom � partir de la base
		
		String getPrenom();
		//renvoie le pr�nom � partir de la base
		
		String getMotDePasse();
		//renvoie le pass en clair � partir du hash dans la base
		
		void setIdUser(String idUser);
		//renvoie l'id de l'utilisateur � partir de l'attribut
		
		void setNom(String nom);
		//renvoie le nom � partir de la base
		
		void setPrenom(String prenom);
		//renvoie le pr�nom � partir de la base
		
		void setMotDePasse(String mdp);
		//renvoie le pass en clair � partir du hash dans la base

    private:
		String identifiant;
};

#endif
