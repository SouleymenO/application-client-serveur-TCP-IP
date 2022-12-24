#include "../headers/slib.h"

#define MAX_CONNECTIONS_ATTENTE 10


// handler pour liberer les resources des fils lors de leur mort
void handler(int signum){
	printf("SIGCHILD intercepted, child server processus resources's freed\n");
	wait(NULL);
}


int main(int argc, char **argv){
	// Arguments commande terminal
	if (argc != 3) { // erreur si la commande n'est pas saisie entièrement
		fprintf(stderr, "Usage: ./server num_port filename\n");
		exit(-1);
	}

	int port;
	if(atoi(argv[1]) < 49152 || atoi(argv[1]) > 65535) { // si port non valide
		fprintf(stderr, "Entrer un numéro de port libre entre 49152 et 65535\n");
		exit(-1);
	}
	port = atoi(argv[1]); // le numéro de port reçoit celui entré en argument dans la ligne de commande

	// vérification de l'existence et la lecture du fichier .txt dans doc/
	if(!access(argv[2], F_OK )) { // fichier existe
        printf("The file %s was found\n", argv[2]);
		if(!access(argv[2], R_OK )){ // fichier peut être lu
        	printf("The file %s can be read\n",argv[2]);
    	}else{ // fichier ne peut pas être lu
        	fprintf(stderr, "The file %s cannot be read\n",argv[2]);
			exit(-1);
    	}
    } else { // fichier non trouvé
        fprintf(stderr, "The file %s not found\n",argv[2]);
		exit(-1);
    }
	
	/////////////////////////////////////////////////////////////////////////////////////


	int p;   // socket d'écoute
	struct sockaddr_in s; // structure contenant l'addresse
	struct sockaddr_in s2; // informations sur le client
	int p2; // socket de service
	
	// deroutement du comportement lors de la reception d'un SIGCHLD
	struct sigaction ac;
	ac.sa_handler = handler;
	ac.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &ac, NULL);
	

	// creation de la socket d'écoute
	if( (p = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		perror("socket");
		exit(-1);
	}
	printf("Listenning socket created, file descriptor n:%d.\n", p);
	
	// config structure correspondante à l'adresse
	s.sin_family = AF_INET;
	s.sin_port = htons(port);
	s.sin_addr.s_addr = htonl(INADDR_ANY);
	
	
	// attachement de la socket d'écoute p en utilisant la structure définie dans s (bind)
	if(bind(p, (struct sockaddr *)&s, sizeof(struct sockaddr)) == -1){
		perror("bind");
		exit(-1);
	}
	printf("binding socket %d with (ip=%u, port=%d).\n ", p, s.sin_addr.s_addr, port);
	
	// ouverture de service sur la socket d'écoute p (listen)
	if(listen(p, MAX_CONNECTIONS_ATTENTE) == -1){
		perror("listen");
		exit(-1);
	}
	printf("Listen on socket n %d.\n", p);
	
	while(1) { // boucle infini acceptation des connexion
	
		// accept connection
		socklen_t len = sizeof(struct sockaddr);
		if((p2 = accept(p, (struct sockaddr *)&s2, &len)) == -1){
			perror("accept");
			exit(-1);
		}
		printf("Connection accepted client with @IP: %u\n", s2.sin_addr.s_addr);
		
		switch (fork()){
		   case -1:
		   	perror("fork");
		   	exit(-1);
		   case 0: // dialogue du fils avec le client en utilisant la socket p2
			close(p); // fermeture socket d'écoute pour le fils
			//////// DIALOGUE AVEC LE CLIENT ////////////////////////////////////////////////////////
			int requete; // code de la requete
			int terminer = 0;
			// collecter les informations du fichier simulant la base de donnée
			livre *livresBD = remplir_tab(argv[2]);
			// calcul taille de la BD càd nombre de livres
			int tailleBD = nb_livre(argv[2]);
			
			while(!terminer){ // tant que l'échange avec une et une seul machine n'a pas terminer faire:
			
				//reception du code requete demandé par le client
				receive(p2, &requete, sizeof(int));
				// selon la requete demandée on execute un protocole different de communication
				switch(requete){
				   case 0:{
					// traite la requete 0
					requete0(p2, livresBD, tailleBD);
				   	break;
				   }
				   case 1:{
					// traite la requete 1
					requete1(p2, livresBD, tailleBD);
				   	break;
				   }
				   case 2:{
					//traite la requete 2
				   	requete2(p2, livresBD, tailleBD);
				   	break;
				   }
				   case 3:{
					// traite la requete 3
					requete3(p2, livresBD, tailleBD);
				   	break;
				   }
				   case -1:
				   	terminer = 1;
				   	break;
				   default:
				   	// default si le serveur recoi une requete pas valide il concidere 
				   	fprintf(stderr,"error in main\n");
				   	exit(-1);
				}
				
			}
			
			
			
			
			
			
			// liberation mémoire
			free(livresBD);
			
			
			
			//////// FIN DIALOGUE  ///////////////////////////////////////////////////////////////
			close(p2);
			return 0;
		   	
		   	
		   default:{ // comportement du père
			// fermeture socket de service pour le pere, le fils crée s'ocupe de la partie service
		   	close(p2);
		   	
		   	
		   }
		}
		
	}
	close(p);
}