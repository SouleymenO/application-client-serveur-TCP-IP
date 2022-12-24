#include "../headers/slib.h"


// 
int nb_livre(char *fileName){

   FILE *f = fopen(fileName, "r");
   int nombre = 0;
   char c;
   while((c = fgetc(f)) != EOF){
      if(c == '\n'){
         nombre++;
      }
   }
   fclose(f);
   return nombre;
}


//fonction qui prend un fichier et renvoie un tableau de structure
livre *remplir_tab(char *fileName){
	int tailleMaxLigne = 2*sizeof(int) + MAX_AUTEUR + MAX_TITRE + MAX_GENRE + 1;
	char ligne_lue[tailleMaxLigne + 1];
    	livre *livres;

	char ref[10];
	char page[10];
	char appreciation[2];

	int i = 0;
	livres = (livre *)malloc(nb_livre(fileName) * sizeof(livre));
	FILE *f; 
	// ouverture fichier et traitement cas d'erreur
	if( (f = fopen(fileName, "r")) == NULL){
		perror("fopen");
		exit(-1);
	}
	while(fgets(ligne_lue, tailleMaxLigne, f) != NULL){
		sscanf(ligne_lue, "%[^#]#%[^#]#%[^#]#%[^#]#%[^#]#%[^\n]", ref, livres[i].auteur, livres[i].titre, livres[i].genre, page, appreciation);
		livres[i].reference = atoi(ref);
		livres[i].nb_page = atoi(page);
		livres[i].appreciation = appreciation[0];
		i++;
	}
	
	fclose(f);
	return livres;
}


// fct de recherche d'un livre à partir d'une reference 
// si la ref n'existe pas alors le reply0.auteur = -1
// (utilisé pour le protocole de com 0)
reply0 rechercheRef(int ref, livre *livresBD, int tailleBD){
	reply0 r;
	int i=0;
	
	while(i<tailleBD && (livresBD[i].reference != ref) ){
		i++;
	}
	if(i<tailleBD){ // si trouvé
		strcpy( r.auteur, livresBD[i].auteur);
		strcpy( r.titre, livresBD[i].titre);
		strcpy( r.genre, livresBD[i].genre);
		r.nb_page = livresBD[i].nb_page;
	}else{ // si pas trouvé
		strcpy( r.auteur, "-1");
	}
	
	return r;
}


//slib.c
// fonction qui renvoie le nbre de mot clé trouvé dans le tableau de livre
// si le mot clé n'existe dans aucun titre alors 0 est renvoyé
// (utilisé pour le protocole 1)
int nbreMotsDansTabLivre(char * mot, livre *livresBD, int tailleBD) {
	int compteur = 0;

	for(int i = 0; i < tailleBD; i++) { // compte le nbre de livre trouvé avec le mot clé
		if(strstr(livresBD[i].titre, mot) != NULL) {
			compteur++;
		}
	}

	return compteur;
}

//slib.c
// fonction qui renvoie le nbre de livre de l'auteur trouvé dans le tableau de livre
// si l' auteur n'existe pas dans le tableau alors 0 est renvoyé
// (utilisé pour le protocole 3)
int nbreAuteurDansTabLivre(char *auteur, livre *livresBD, int tailleBD) {
	int compteur = 0;

	for(int i = 0; i < tailleBD; i++) { // compte le nbre de livre trouvé avec l'auteur
		if(strcmp(livresBD[i].auteur, auteur) == 0) {
			compteur++;
		}
	}

	return compteur;
}

//slib.c
// fonction qui renvoie le nouveau tableau de livre selon les mots clés trouvés
// (utilisé pour le protocole 1)
livre *stockeLivreMotCle(char * mot, livre *livresBD, int tailleBD, int nbre_mots_trouves) {
	livre *res;
	int j = 0;

	res = (livre *)malloc(nbre_mots_trouves * sizeof(livre));
	// stocke les livres trouvés dans le nouveau tableau de livre
	for(int i = 0; i < tailleBD; i++) {
		if(strstr(livresBD[i].titre, mot) != NULL) {
			res[j].reference = livresBD[i].reference;
			strcpy(res[j].auteur, livresBD[i].auteur);
			strcpy(res[j].titre, livresBD[i].titre);
			strcpy(res[j].genre, livresBD[i].genre);
			res[j].nb_page = livresBD[i].nb_page;
			res[j].appreciation = livresBD[i].appreciation;
			j++;
		}
	}

	return res;
}


// fonction qui renvoie le nouveau tableau de livre avec les livres de l'auteur
// (utilisé pour le protocole 3)
livre *stockeLivreAuteur(char *auteur, livre *livresBD, int tailleBD, int nbre_auteur_trouve) {
	livre *res;
	int j = 0;

	res = (livre *)malloc(nbre_auteur_trouve * sizeof(livre));
	// stocke les livres trouvés dans le nouveau tableau de livre
	for(int i = 0; i < tailleBD; i++) {
		if(strcmp(livresBD[i].auteur, auteur) == 0) {
			res[j].reference = livresBD[i].reference;
			strcpy(res[j].auteur, livresBD[i].auteur);
			strcpy(res[j].titre, livresBD[i].titre);
			strcpy(res[j].genre, livresBD[i].genre);
			res[j].nb_page = livresBD[i].nb_page;
			res[j].appreciation = livresBD[i].appreciation;
			j++;
		}
	}

	return res;
}

//slib.c
// fonction qui traite la requete 1 avec tous les mots clés
// renvoie le tableau de livre final
// (utilisé pour le protocole 1)
livre *rechercheMotsCles(livre *livresBD, int tailleBD, char **mots) {
	int i = 0;
	int taille = tailleBD;
	int nbr_mot;
	livre *res = livresBD;

	//compte le nbre de livre avec le mot cle
	nbr_mot = nbreMotsDansTabLivre(mots[i], res, taille);
	if(nbr_mot != 0) {
		res = stockeLivreMotCle(mots[i], res, taille, nbr_mot);
		taille = nbr_mot;
	}

	return res;
}

// slib.c
// fonction qui va transformer une struct livre en struct reply1
// (utilisé pour le protocole 1)
reply1 *livreEnReply1(livre *livres, int taille) {
	reply1 *tab;
	tab = (reply1 *)malloc(taille * sizeof(reply1));

	for(int i = 0; i < taille; i++) {
		tab[i].reference = livres[i].reference;
		strcpy(tab[i].auteur, livres[i].auteur);
		strcpy(tab[i].titre, livres[i].titre);
		strcpy(tab[i].genre, livres[i].genre);
	}

	return tab;
}

// fonction qui va transformer une struct livre en struct reply3
// (utilisé pour le protocole 3)
reply3 *tabLivreEnTabReply3(livre *livres, int taille) {
	reply3 *tab;
	tab = (reply3 *)malloc(taille * sizeof(reply3));

	for(int i = 0; i < taille; i++) {
		tab[i].reference = livres[i].reference;
		strcpy(tab[i].titre, livres[i].titre);
		strcpy(tab[i].genre, livres[i].genre);
		tab[i].nb_page = livres[i].nb_page;
		tab[i].appreciation = livres[i].appreciation;
	}

	return tab;
}

// transforme un élément d'un tableau de livre en un reply3 et le retourne
reply3 tabLivreEnReply3(livre *livre, int position_livre) {
	reply3 res;

	res.reference = livre[position_livre].reference;
	strcpy(res.titre, livre[position_livre].titre);
	strcpy(res.genre, livre[position_livre].genre);
	res.nb_page = livre[position_livre].nb_page;
	res.appreciation = livre[position_livre].appreciation;

	return res;
}

// renvoie la position du livre qui est selectionné selon le critère de recherche de la requete3
int traitementCritereRequete3(int critere, livre *livres, int nbr_auteur) {
	int position;

	if(critere == 1) { // critere nbre page minimum
		int nb_page = livres[0].nb_page;
		for(int i = 1; i < nbr_auteur; i++) {
			if(livres[i].nb_page < nb_page) {
				nb_page = livres[i].nb_page;
				position = i;
			}
		}
	} else if(critere == 2) { // critère meilleure appréciation
		char appreciation = livres[0].appreciation;
		for(int i = 0; i < nbr_auteur; i++) {
			if(livres[i].appreciation < appreciation) { // compare le code ASCII de l'appréciation et garde le premier du poids le plus faible (donc meilleure appréciation)
				appreciation = livres[i].appreciation;
				position = i;
			}
		}
	}

	return position;
}

// traitement de la requete 0
void requete0(int socket, livre *livresBD, int tailleBD) {
	int ref=0; // reference livre
	int ref_existe=0; // bool ref existe ou pas au debut on suppose qu'elle n'existe pas
	reply0 message;
	// reception de la reference
	receive(socket, &ref, sizeof(int));
	// recherche de la ref et preparation message
	message = rechercheRef(ref, livresBD, tailleBD);
	// test si aucune correspondance avec la ref (oui || non)
	
	if( strcmp(message.auteur, "-1") != 0 ){  // ref trouvé
		ref_existe = 1;
	}
	// notifier le client si correspondence de ref existe dans livresBD
	dispatch(socket, &ref_existe, sizeof(int));
	// envoie du message au client si la ref existe
	if(ref_existe){
		dispatch(socket, &message, sizeof(reply0));
	}
}

// traite la requete 1
void requete1(int socket, livre *livresBD, int tailleBD) {
	int size_mots_cles; // taile du string mots_cles que le client envoie
	int nbre_mots; // nbre de mots dans le string mots_cles

	// pour boucle
	char **mots; // tableau des mots
	int taille = tailleBD;
	int nbr_mot; // nbre de titres avec le mot clé recherché
	livre *ancienTab = livresBD;
	livre *res; // livres contenant le mot clé recherché

	reply1 *tab_final;

	// lecture de la taile du string mots_cles envoyée par le client
	receive(socket, &size_mots_cles, sizeof(int));
	//instanciation du char* mots_cles à partir de la taille reçue
	char mots_cles[size_mots_cles];
	// lecture du string mots_cles envoyée par le client
	receive(socket, &mots_cles, size_mots_cles);
	// calcul du nbre de mots dans mots_cles
	nbre_mots = nbreMotsString(mots_cles, size_mots_cles);
	//découpage du string mots_cles en mots
	mots = decouperString(mots_cles, nbre_mots);
	// creation du nouveau tableau de livre
	for(int j = 0; j < nbre_mots; j++) {
		nbr_mot = nbreMotsDansTabLivre(mots[j], ancienTab, taille);
		if(nbr_mot != 0) {
			res = stockeLivreMotCle(mots[j], ancienTab, taille, nbr_mot);
			taille = nbr_mot;
			ancienTab = res;
		}
	}

	//envoie du nbre de livres trouvés avec les mots clés saisis au client
	dispatch(socket, &nbr_mot, sizeof(int));
	// envoie des livres un part un sous forme de structure reply1 au client
	if(nbr_mot != 0) {
		tab_final = livreEnReply1(res, taille); // transformer notre tableau de livres (structure livre*) res en struct reply1*

		for(int i = 0; i < taille; i++) { // envoie
			dispatch(socket, &tab_final[i], sizeof(reply1));
		}
	}

	//libération mémoire
	/*free(mots);
	free(ancienTab);
	free(res);
	free(tab_final);*/
}

// traite la requete 2
void requete2(int socket, livre *livresBD, int tailleBD) {
	reply2 *tab;
	int taille;
	char auteur[MAX_AUTEUR];
	char genre[MAX_GENRE];
	reply2 livre;

	// recevoir l'auteur saisi par le client
	receive(socket, auteur, MAX_AUTEUR);
	// recevoir l'auteur saisi par le client
	receive(socket, genre, MAX_GENRE);
	// calcul la taille des résultats trouvés
	taille = taille_tab_protocole2(auteur, genre, livresBD, tailleBD);
	// creer le nouveau tableau de reply2 avec les résultats obtenus
	tab = rechercheAuteurEtGenre(auteur, genre, livresBD, tailleBD);
	// envoie le nbre de résultats obtenus
	dispatch(socket, &taille, sizeof(int));
	
	if(taille != 0) { // si résultats
		for(int i = 0; i < taille; i++) { // envoie un aprt un les résultats en reply2
			livre = tab[i];
			dispatch(socket, &livre, sizeof(reply2));
		}
	}
}

// traite la requete 3
void requete3(int socket, livre * livresBD, int tailleBD) {
	char auteur[100];
	int nbr_auteur;
	livre *res;
	int critere;
	reply3 *reply;
	reply3 message;

	// recevoir l'auteur saisi par l'utlisateur
	receive(socket, auteur, 100);
	// compte le nbre de livres de l'auteur reçu
	nbr_auteur = nbreAuteurDansTabLivre(auteur, livresBD,tailleBD);
	// recherche des livres de l'auteur reçu
	res = stockeLivreAuteur(auteur, livresBD, tailleBD, nbr_auteur);
	// envoie le nbre de résultats trouvés
	dispatch(socket, &nbr_auteur, sizeof(int));
	// traitement des résultats
	if(nbr_auteur > 1) { // cas où plusieurs résultats
		int position;
		// reçoit le critere saisi par l'utilisateur
		receive(socket, &critere, sizeof(int));
		// traitement critere
		position = traitementCritereRequete3(critere, res, nbr_auteur);
		// création du reply3 à envoyer au client
		message = tabLivreEnReply3(res, position);
	} else if (nbr_auteur == 1) { // cas où 1 résultats
		// transforme le resultat en reply3
		reply = tabLivreEnTabReply3(res, nbr_auteur);
		message = reply[0];
	}
	// envoie du reply3 au client
	dispatch(socket, &message, sizeof(reply3));

	// libération mémoire
	/*free(res);
	free(reply);*/
}





int taille_tab_protocole2(char *auteur, char *genre, livre *livresBD, int tailleBD){
	int c = 0;
	for(int i = 0; i < tailleBD; i++){
		if(strcmp(livresBD[i].auteur, auteur) == 0 && strcmp(livresBD[i].genre, genre) == 0){
			c++;
		}
	}
	return c;
}

reply2 *rechercheAuteurEtGenre(char *auteur, char *genre, livre *livresBD, int tailleBD){
	reply2 *tab;
	int j = 0;
	int taille = taille_tab_protocole2(auteur, genre, livresBD, tailleBD);
	tab = (reply2 *)malloc(taille * sizeof(reply2));
	for(int i = 0; i < tailleBD; i++){
		if(strcmp(livresBD[i].auteur, auteur) == 0 && strcmp(livresBD[i].genre, genre) == 0){
			tab[j].reference = livresBD[i].reference;
			strcpy(tab[j].titre, livresBD[i].titre);
			j++;
		}
	}
	return tab;
}