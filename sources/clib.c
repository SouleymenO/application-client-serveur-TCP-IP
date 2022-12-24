#include "../headers/clib.h"


// clib.c
// fct qui affiche le menu à l'utilisateur pour qu'il puisse choisir quel type de requete il veut envoyer au serveur
// ne prend rien en paramètre et renvoie un entier correspondant à un type de requte ou 0, pour quitter
// cette fct filtre les entrée et n'admet que des entiers relatifs compris entre [-1, 3]
int menu(void){
	int requete;
	int requeteValide;
	do{
		requeteValide=1;
		printf("__________________________________MENU_________________________________\n");
		printf("0 pour avoir (nom auteur, titre livre, genre, nb_pages) a partir de la reference.\n");
		printf("1 pour avoir (reference, nom auteur, titre livre, genre) a partir des mot-clés.\n");
		printf("2 pour avoir (reference, titre livre) a partir d'un auteur et d'un genre.\n");
		printf("3 pour avoir (reference, titre livre, genre, nb_pages) a partir d'un auteur.\n");
		printf("-1 pour quitter.\n");
		scanf("%d", &requete);
		if(requete < -1 || requete > 3 ){
			requeteValide = 0;
			printf("Saisi incorrecte, saisir un numero de requete entre l'intervale \n");
			printf("---------------------------------------------------------------\n");
			printf("[ -1 , 3 ] comme indique dans le menu ci-dessous\n");
			printf("------------------------------------------------\n");
		}
	}
	while(!requeteValide);
	return requete;
	
}


// clib.h
// fct demande à l'utilisateur une reférence utilisé pour le protocole 0
// filtrage entrée
// renvoie une ref sous forme d'entier
int demanderRefUtilisateur(){
	char refChar[100];
	int ref;
	printf("Quel reference desirez vous chercher ( entier naturel)?\n");
	scanf("%s", refChar);
	ref = atoi(refChar);
	while(ref<0){
		printf("saisi incorrecte entrer un entier naturel:\n");
		scanf("%d", &ref);
	}
	return ref;
}

// clib.h
// 
void afficherRes0(reply0 message){
	printf("RESULTAT DE LA RECHERCHE:\n");
	printf("-------------------------\n");
	printf("Auteur = %s\n", message.auteur);
	printf("Titre = %s\n", message.titre);
	printf("Genre = %s\n", message.genre);
	if(message.nb_page >= 300){
		printf("Plus de 300 pages.\n");
	}
	else{
		printf("Moins de 300 pages.\n");
	}
	
}


// clib.h
// fct demande à l'utilisateur le(s) mot(s) clé(s) recherché(s) dans le titre utilisé pour le protocole 1
// renvoie une string de mots clés (mot_clé0 mot_clé1 mot_clé2 ...)
char *demanderMotsClesUtilisateur() {
	char *mots_cles;
	mots_cles = (char *)malloc(200);

	printf("Veuillez saisir le(s) mot(s) clé(s) recherché(s) dans le titre ?\n");
	printf("(Saisir de la façon suivante : mot_clé1 mot_clé2 mot_clé3 ...)\n");
	scanf(" %[^\n]s", mots_cles);

	return mots_cles;
}


// clib.h
// 
void afficherRes1(int socket, int taille) {
	reply1 message;

	if(taille != 0) {
		printf("RESULTAT DE LA RECHERCHE:\n");
		printf("-------------------------\n");
		for(int i = 0; i < taille; i++) {
			receive(socket, &message, sizeof(reply1));
			printf("Résultat %d :\n", i);
			printf("-------------\n");
			printf("Référence = %d\n", message.reference);
			printf("Auteur = %s\n", message.auteur);
			printf("Titre = %s\n", message.titre);
			printf("Genre = %s\n", message.genre);
			printf("-------------\n");
		}
	}
	else{
		printf("Pas de résultat pour votre recherche.\n");
	}
	
}

// 
void afficherRes3(reply3 message) {
	printf("RESULTAT DE LA RECHERCHE:\n");
	printf("-------------------------\n");
	printf("Résultat :\n");
	printf("-------------\n");
	printf("Référence = %d\n", message.reference);
	printf("Titre = %s\n", message.titre);
	printf("Genre = %s\n", message.genre);
	printf("Nombre de pages = %d\n", message.nb_page);
	printf("Appréciation = %c\n", message.appreciation);
	printf("-------------\n");
}

// traitement de la requete 0
void requete0(int socket) {
	int ref_existe;
	int ref=0; // reference saisi par l'utilisateur
	// demander à l'utilisateur quel ref pour recevoir (nom, titre,genre,nb_pages)
	ref = demanderRefUtilisateur();
	// envoie ref au serveur
	dispatch(socket, &ref, sizeof(int));
	// s'informer du serveur s'il a trouver un livre correspondant à la ref
	receive(socket, &ref_existe, sizeof(int));
	// reception du message reply0 si le livre existe chez la livresBD serveur
	if(ref_existe){
		reply0 message;
		// reception du message
		receive(socket, &message, sizeof(reply0));
		afficherRes0(message);
	}
	else{
		printf("RESULTAT DE LA RECHERCHE:\n");
		printf("-------------------------\n");
		printf("Reference n'existe pas.\n");
	}
}

// traitement requete1
void requete1(int socket) {
	char *mots; // string de mots clés pour protocole1
	int size; // taille du string mots_cles
	int nbr_livres_trouves;

	// demander à l'utilisateur quel(s) est/sont le(s) mot(s) clé(s) recherché(s) dans le titre
	mots = demanderMotsClesUtilisateur();
	// calcul de la taille du string mots_cles saisi par l'utilisateur
	size = strlen(mots) + 1; // +1 pour compter le caractere de fin de chaine
	// envoie de la taille du string mots_cles au serveur
	dispatch(socket, &size, sizeof(int));
	//envoie du string mots_cles au serveur
	dispatch(socket, mots, size);
	//reception du nbre de livres trouvés avec les mots clés saisis au client
	receive(socket, &nbr_livres_trouves, sizeof(int));
	// receptionn des reply1 envoyé par le serveur et affichage à l'utilisateur des résultats
	afficherRes1(socket, nbr_livres_trouves);


	//liberation mémoire
	//free(mots);
}

// traitement requete2
void requete2(int socket) {
	char *auteur; // auteur saisi par utilisateur
	char *genre; // genre saisi par utilisateur
	reply2 message2;
	int taille_tab;


	// demander à l'utilisateur l'auteur et le genre
	auteur = demanderAuteur();
	genre = demanderGenre();
	
	// envoyer auteur saisi par l'utilisateur
	dispatch(socket, auteur, MAX_AUTEUR);
	// envoyer auteur saisi par l'utilisateur
	dispatch(socket, genre, MAX_GENRE);
	// recevoir le nbre de resultats obtenus
	receive(socket, &taille_tab, sizeof(int));

	if(taille_tab > 0) { // si résultats
		printf("RESULTAT DE LA RECHERCHE:\n");
		printf("-------------------------\n");
		for(int i = 0; i < taille_tab; i++) { // recevoir un part un les résultats et les afficher
			receive(socket, &message2, sizeof(reply2));
			afficherRes2(message2, i);
		}
	}else{ // si pas de résultats
		printf("RESULTAT DE LA RECHERCHE:\n");
		printf("-------------------------\n");
		printf("Aucun livre ne correspond à vos choix.\n");
	}
}

// traitement requete3
void requete3(int socket) {
	char auteur[100];
	int nbr_res;
	//char crit[50];
	int critere;
	reply3 res;
	// demander à l'utilisateur un auteur
	printf("Veuillez saisir un nom d'auteur :\n");
	printf("(en respectant la casse cad la majuscule au début)\n");
	scanf(" %[^\n]s", auteur);
	// envoyer l'auteur saisie par l'utilisateur au serveur
	dispatch(socket, auteur, 100);
	// recevoir le nbre de résultats trouvés
	receive(socket, &nbr_res, sizeof(int));
	// traitement des résultats
	if(nbr_res > 1) { // cas où plusieurs résultats
		// demande à l'utlisateur le critère de recherche
		printf("Plusieurs résultats trouvés !\n");
		printf("Veuiller choisir le critère de recherche parmis les suivants :\n");
		printf("1 - le nombre de pages minimum\n");
		printf("2 - la meilleure appréciation des lecteurs\n");
		scanf("%d", &critere);
		// envoie le num du critère selectionné
		dispatch(socket, &critere, sizeof(int));
		// reçoit le res et l'affiche
		receive(socket, &res, sizeof(reply3));
		afficherRes3(res);
	} else if (nbr_res == 1) { // cas où 1 résultats
		// reçoit le résultat en reply3
		receive(socket, &res, sizeof(reply3));
		// affiche résultat
		afficherRes3(res);
	} else { // cas où 0 résultat
		printf("Aucun résultat trouvé.\n");
	}
}






// fct demande à l'utilisateur un auteur et un genre utilisé pour le protocole 2
// filtrage entrée
// renvoie une chaine de caractère qui concatennent l'auteur et le genre avec un espace entre les deux
char *demanderAuteur(){
	char *auteur;
	auteur = (char *)malloc(MAX_AUTEUR);
	printf("Quel nom d'auteur voulez-vous chercher ?\n");
	scanf(" %[^\n]s", auteur);

	return auteur;
}
	
char *demanderGenre(){
	char *genre;
	genre = (char *)malloc(MAX_GENRE);
	printf("Quel genre voulez-vous chercher pour cet auteur ?\n");
	scanf(" %[^\n]s", genre);

	return genre;
}

void afficherRes2(reply2 message2, int i){
	printf("Resultat %d :\n", i);
	printf("-------------\n");
	printf("Reference = %d\n", message2.reference);
	printf("Titre = %s\n", message2.titre);
	printf("-------------\n");
}