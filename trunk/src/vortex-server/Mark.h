#ifndef NOTE
#define NOTE

class Note
{ // idUser, idDoc, note
    public:
		Note();
		//Constructeur pour cr�er une nouvelle note

		Note(String idUser, String idDoc)
		{
          this->idDoc=idDoc;
          this->idUser=idUser;
        }
		//Constructeur pour r�cup�rer la note de User pour Doc

    private:
      string idDoc;
      string idUser;
};

#endif
