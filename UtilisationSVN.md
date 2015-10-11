# Introduction #

Cette page explique en détail comment utiliser svn pour récupérer le code de Vortex, et le synchroniser avec le dépôt Google Code.


# Détails #

## Etape préliminaire ##

svn checkout https://vortex-ee.googlecode.com/svn/trunk/ vortex-ee --username _votre.nick.gmail_

Quand on demande un mot de passe, donner celui qui vous est donné sur cette page:

http://code.google.com/hosting/settings

## A faire à chaque fois avant de commencer à travailler sur le code ##

(dans vortex-ee) svn update

## A faire à chaque fois que vous avez fini de travailler sur le code ##
### Ajout/Supression de fichiers/Repertoires: ###
#### Si vous avez créé de nouveaux fichiers ####
svn add _nom.fichier/repertoire_
#### Si vous avez supprime des fichiers ou voulez les exclure: ####
svn del _nom.fichier/repertoire_
### Sync avec le server SVN(commit) ###
svn commit (a faire dans le repertoire vortex-ee)