#ifndef UTILISATEUR
#define UTILISATEUR

class Utilisateur
{ // idUser, nom, prenom, pass
    public:
		Utlisateur();
		//constructeur par défaut (quand on a besoin de créer un utilisateur)
	
		Utlisateur(String id);
		//le constructeur d'un Utilisateur déjà présent dans la base
			
		creerUtlisateur(String idUSer, String nom, String prenom, String pass);
		//ajoute l'utilisateur dans la base avec son id, nom, prenom, hash md5 du mot de pass en paramètre
	
		String getPassword();
		//Renvoie le mot de passe de l'utilisateur en clair en récupérant son hash md5 dans la base
				
		String getIdUser();
		//renvoie l'id de l'utilisateur à partir de l'attribut
		
		String getNom();
		//renvoie le nom à partir de la base
		
		String getPrenom();
		//renvoie le prénom à partir de la base
		
		String getMotDePasse();
		//renvoie le pass en clair à partir du hash dans la base
		
		void setIdUser(String idUser);
		//renvoie l'id de l'utilisateur à partir de l'attribut
		
		void setNom(String nom);
		//renvoie le nom à partir de la base
		
		void setPrenom(String prenom);
		//renvoie le prénom à partir de la base
		
		void setMotDePasse(String mdp);
		//renvoie le pass en clair à partir du hash dans la base

    private:
		String identifiant;
};

#endif
