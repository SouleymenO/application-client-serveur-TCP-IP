#include "../headers/cslib.h"


// procedure qui ecrit dans un descripteur p le buffer 
// cette procedure verifie si tout a été envoyé et qu'il n'y a pas d'erreur
void dispatch(int p, void *buffer, int taille){
	int n;
	if( (n = write(p, buffer, taille)) == -1 || n != taille ){
		fprintf(stderr, "error in fct write in fct dispatch.\n");
		exit(-1);
	}
}


// procedure qui lit dans un descripteur p le buffer 
// cette procedure verifie si tout a été lu et qu'il n'y a pas d'erreur
void receive(int p, void *buffer, int taille){
	int n;
	if( (n = read(p, buffer, (size_t)taille)) == -1 || n != taille ){
		fprintf(stderr, "error in fct read in fct receive.\n");
		exit(-1);
	}
}


//clib.c
// fct qui va renvoyer le nbre de mots dans un string
// caractere de séparation " "
int nbreMotsString(char *string, int taille) {
	char copy[taille]; // la copie de la string
	int nbre_mots = 0;
	char *mot;

	strcpy(copy, string);
	mot = strtok(copy, " ");

	while(mot != NULL) { // compter le nbre de mots
		mot = strtok(NULL, " ");
		nbre_mots++;
	}

	return nbre_mots;
}


//clib.c
// fct qui va renvoyer un tableau de mots avec les mots du string
// caractere de séparation " "
char **decouperString(char *string, int nbre_mots) {
	char **mots;
	int i = 0;

	//instanciation du tableau de chaine de caractere mots
	mots = (char **)malloc(nbre_mots * sizeof(char *)); // taille du tableau
	for(int j = 0; j < nbre_mots; j++) {
		mots[j] = (char *)malloc(50); // taille d'une ligne du tableau
	}

	char *mot = strtok(string, " "); // découpe du string avec comme caractère de séparation " "

	while(mot != NULL) {
		//printf("mot %d : %s\n", i, mot);
		strcpy(mots[i], mot); // ajout des mots dans le tableau
		//printf("MOT %d : %s", i, mots[i]);
		mot = strtok(NULL, " ");
		i++;
	}

	return mots;
}