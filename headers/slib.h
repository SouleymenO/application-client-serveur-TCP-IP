#ifndef SLIB_H
#define SLIB_H

#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#include "cslib.h"

// structure d'une ligne contenant toutes les infos sur le livre
typedef struct {
      int reference;
      char auteur[MAX_AUTEUR];
      char titre[MAX_TITRE];
      char genre[MAX_GENRE];
      int nb_page;
      char appreciation;
} livre;


//
int nb_livre(char *fileName);


//fonction qui prend un fichier et renvoie un tableau de structure
livre *remplir_tab(char *fileName);


// fct de recherche d'un livre à partir d'une reference 
// si la ref n'existe pas alors le reply0.auteur = -1
// (utilisé pour le protocole de com 0)
reply0 rechercheRef(int ref, livre *livresBD, int tailleBD);

// fonction qui renvoie le nbre de mot clé trouvé dans le tableau de livre
// si le mot clé n'existe dans aucun titre alors 0 est renvoyé
// (utilisé pour le protocole 1)
int nbreMotsDansTabLivre(char * mot, livre *livresBD, int tailleBD);

// fonction qui renvoie le nbre de livre de l'auteur trouvé dans le tableau de livre
// si l' auteur n'existe pas dans le tableau alors 0 est renvoyé
// (utilisé pour le protocole 3)
int nbreAuteurDansTabLivre(char *auteur, livre *livresBD, int tailleBD);

// fonction qui renvoie le nouveau tableau de livre selon les mots clés trouvés
// (utilisé pour le protocole 1)
livre *stockeLivreMotCle(char * mot, livre *livresBD, int tailleBD, int nbre_mots_trouves);

// fonction qui renvoie le nouveau tableau de livre avec les livres de l'auteur
// (utilisé pour le protocole 3)
livre *stockeLivreAuteur(char * auteur, livre *livresBD, int tailleBD, int nbre_auteur_trouve);

// fonction qui traite la requete 1 avec tous les mots clés
// renvoie le tableau de livre final
// (utilisé pour le protocole 1)
livre *rechercheMotsCles(livre *livresBD, int tailleBD, char **mots);

// fonction qui va transformer une struct livre en struct reply1
// (utilisé pour le protocole 1)
reply1 *livreEnReply1(livre *livres, int taille);

// fonction qui va transformer une struct livre en struct reply3
// (utilisé pour le protocole 3)
reply3 *tabLivreEnTabReply3(livre *livres, int taille);

// transforme un élément d'un tableau de livre en un reply3 et le retourne
reply3 tabLivreEnReply3(livre *livre, int position_livre);

// renvoie la position du livre qui est selectionné selon le critère de recherche de la requete3
int traitementCritereRequete3(int critere, livre *livres, int nbr_auteur);

// traitement de la requete 0
void requete0(int socket, livre *livresBD, int tailleBD);

// traite la requete 1
void requete1(int socket, livre *livresBD, int tailleBD);

// traite la requete 2
void requete2(int socket, livre *livresBD, int tailleBD);

// traite la requete 3
void requete3(int socket, livre * livresBD, int tailleBD);




reply2 *rechercheAuteurEtGenre(char *auteur, char *genre, livre *livresBD, int tailleBD);

int taille_tab_protocole2(char *auteur, char *genre, livre *livresBD, int tailleBD);

#endif
