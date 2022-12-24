#ifndef CLIB_H
#define CLIB_H

#include <netdb.h>
#include "cslib.h"

// fct qui affiche le menu à l'utilisateur pour qu'il puisse choisir quel type de requete il veut envoyer au serveur
// ne prend rien en paramètre et renvoie un entier correspondant à un type de requte ou 0, pour quitter
// cette fct filtre les entrée et n'admet que des entiers relatifs compris entre [-1, 3]
int menu(void);


// fct demande à l'utilisateur une reférence utilisé pour le protocole 0
// filtrage entrée
// renvoie une ref sous forme d'entier
int demanderRefUtilisateur();


// 
void afficherRes0(reply0 message);

// fct demande à l'utilisateur le(s) mot(s) clé(s) recherché(s) dans le titre utilisé pour le protocole 1
// renvoie une string de mots clés (mot_clé0 mot_clé1 mot_clé2 ...)
char *demanderMotsClesUtilisateur();

// affiche les résultats du protocole 1
void afficherRes1(int socket, int taille);

// 
void afficherRes3(reply3 message);

// traitement de la requete 0
void requete0(int socket);

// traitement requete1
void requete1(int socket);

// traitement requete2
void requete2(int socket);

// traitement requete3
void requete3(int socket);


// fct demande à l'utilisateur un auteur et un genre utilisé pour le protocole 2
// filtrage entrée
// renvoie une chaine de caractère qui concatennent l'auteur et le genre avec un espace entre les deux
char *demanderAuteur();

char *demanderGenre();

void afficherRes2(reply2 message2, int i);


#endif
