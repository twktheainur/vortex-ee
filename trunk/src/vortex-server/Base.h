#ifndef BASE
#define BASE

class Base
{
    public:
		Base();
		//ouvre la base... peut être des paramètres à ajouter...
	
		~Base();
		//à votre avis?
	
		bool documentExiste(String id);
		//renvoie vrai si le document existe deja dans la base
			
		bool utlisateurExiste(String id);
		//renvoie vrai si l'utilisateur existe deja dans la base
	
    private:

};

#endif
