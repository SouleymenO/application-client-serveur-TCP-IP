#ifndef CSLIB_H
#define CSLIB_H

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h> 
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>


#define MAX_AUTEUR 50
#define MAX_TITRE 100
#define MAX_GENRE 20


// structure reponse protocole de communication 0
typedef struct {
      char auteur[MAX_AUTEUR];
      char titre[MAX_TITRE];
      char genre[MAX_GENRE];
      int nb_page;
} reply0;

// structure reponse protocole de communication 1
typedef struct {
      int reference;
      char auteur[MAX_AUTEUR];
      char titre[MAX_TITRE];
      char genre[MAX_GENRE];
} reply1;

// structure reponse protocole de communication 2
typedef struct {
      int reference;
      char titre[MAX_TITRE];
} reply2;

// structure reponse protocole de communication 3
typedef struct {
      int reference;
      char titre[MAX_TITRE];
      char genre[MAX_GENRE];
      int nb_page;
      char appreciation;
} reply3;






// procedure qui ecrit dans un descripteur p le buffer 
// cette procedure verifie si tout a été envoyé et qu'il n'y a pas d'erreur
void dispatch(int p, void *buffer, int taille);


// procedure qui lit dans un descripteur p le buffer 
// cette procedure verifie si tout a été lu et qu'il n'y a pas d'erreur
void receive(int p, void *buffer, int taille);

// fct qui va renvoyer le nbre de mots dans un string
// caractere de séparation " "
int nbreMotsString(char *string, int taille);

// fct qui va renvoyer un tableau de mots avec les mots du string
// caractere de séparation " "
char **decouperString(char *string, int nbre_mots);

#endif
