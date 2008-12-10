#ifndef Document
#define Document

class Document
{ // idDoc, idAuteur, Commentaire, type
    public:
		Document();
		//constructeur par défaut (quand on a besoin de créer un Document vide)
	
		Document(String id);
		//le constructeur d'un Document déjà présent dans la base
		
		String getIdDoc();
		//renvoie l'id du doc à partir de l'attribut
		
		String getAuteur();
		//renvoie l'auteur du doc à partir de la base
		
		String getCommentaire();
		//renvoie le commentaire explicatif du document laissé par l'auteur à partir de la base
		
		int getType();
		//renvoie le type du doc à partir de la base
		
		void setIdDoc(String idDoc);
		//affecte un id au doc (attribut de la classe)
		
		void setAuteur(String idAuteur);
		//affecte un idUser au doc dans la base
		
		void setCommentaire(String com);
		//affecte lecommentaire explicatif du document laissé par l'auteur au doc dans la base
		
		void setType(int type);
		//affecte le type au doc {video,image,son} identifiés par un chiffre dans la base

    private:
		String idDoc; //nom physique du document
};

#endif
