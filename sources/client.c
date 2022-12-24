#include "../headers/clib.h"


int main(int argc, char **argv){
	// Arguments commande terminal
	if (argc != 3) { // erreur si la commande n'est pas saisie entièrement
		fprintf(stderr, "Usage: ./client server_name num_port\n");
		exit(-1);
	}

	int port;
	if(atoi(argv[2]) < 49152 || atoi(argv[2]) > 65535) { // si port non valide
		fprintf(stderr, "Entrer un numéro de port libre entre 49152 et 65535\n");
		exit(-1);
	}
	port = atoi(argv[2]); // le numéro de port reçoit celui entré en argument dans la ligne de commande

	///////////////////////////////////////////////////////////////////////////////////


	int p;
	struct hostent *h;  // information concernant le serveur
	struct sockaddr_in s;
	
	// creation de la socket
	if( (p = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(-1);
	}
	
	// appel au serveur DNS
	if( (h=gethostbyname(argv[1])) == NULL ){
		fprintf(stderr, "error in fct gethostbyname\n");
		exit(-1);
	}
	
	// remplir la structure struct sockaddr_in s
	s.sin_family = AF_INET;
	memcpy(&s.sin_addr.s_addr, h->h_addr, sizeof(u_long)); // sin_addr.s_addr reçoit la valeur h_addr
	s.sin_port = htons(port);
	
	// connexion
	if(connect(p, (struct sockaddr *)&s, sizeof(struct sockaddr)) == -1){
		perror("connect");
		exit(-1);
	}
	
	
	////////////////////////////// DIALOGUE //////////////////////////////////////////////////////////
	
	int requete;
	int terminer = 0;
	
	// on affiche le menu, et tant que le USER ne choisi pas de quitter (-1), on fait:
	while( !terminer){ 
	
		// affichage menu
		requete = menu();
		
		// envoie requete au serveur
		dispatch(p, &requete, sizeof(int));
		// switch sur different protocole d'echange selon le choix de l'utilisateur
		switch(requete){
		   case 0:{
		   	requete0(p);
		   	break;
		   }
		   case 1:{
			requete1(p);
			break;
		   }
		   case 2:{
		   	requete2(p);
		   	break;
		   }
		   case 3:{
			requete3(p);
		   	break;
		   }
		   case -1:
		   	terminer = 1;
		   	printf("AU REVOIR.\n");
		   	break;
		   default:
		   	// default pour s'assurer du bon filtrage fait precedement si filtrage mal 
		   	// fait interruption programme et renvoie erreur
		   	fprintf(stderr,"error in main\n");
		   	exit(-1);
		}
	
	}
	
	///////////////////////////////// FIN DIALOGUE ///////////////////////////////////////////////////
	close(p);
	return 0;
}
