#ifndef DATABASE_H_
#define DATABASE_H_

class Base
{
    public:
		Base();
		//ouvre la base... peut �tre des param�tres � ajouter...

		~Base();
		//� votre avis?

		bool documentExiste(String id);
		//renvoie vrai si le document existe deja dans la base

		bool utlisateurExiste(String id);
		//renvoie vrai si l'utilisateur existe deja dans la base

    private:

};

#endif
