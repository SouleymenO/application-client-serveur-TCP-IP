# application-client-serveur-TCP-IP
***
Projet réalise en équipe de 3 pendant une semaine (28/11/22 - 02/12/22)
***
## Table des matières
1. [Infos générales]
2. [Technologies]
3. [Installation]
### Infos générales
***
Ce projet consiste en la réalisation en C d'une application client/serveur TCP/IP simplifiée. Il s'agira de réaliser, d'une part, un serveur qui interrogera un fond documentaire (fichier texte qui simule une base de données) contenant des informations sur des livres sous le format (référence#auteur#titre#genre#nombre_de_pages#appréciation), d'autre part, un client qui pourra échanger avec l'utilisateur pour récupérer ses requêtes et ensuite échanger avec le serveur via le protocole TCP/IP pour récupérer les données nécessaires et ensuite les afficher à l'utilisateur.

Pour plus d'informations, veuillez vous référer au rapport (doc/Rapport_SR.pdf).
### Technologies
***
Ce projet a été intégralement réalisé en **C**.
### Installation
***
Lors de cette explication, toutes les commandes sont saisies depuis le dossier projet c'est-à-dire au même endroit que le makefile.

**Commandes de compilation :**
  - création de l’exécutable server : <br>
      $ make bin/server
  - création de l’exécutable client : <br>
      $ make bin/client
  - création des 2 exécutables en même temps : <br>
      $ make
  - suppression des exécutables (les fichiers .o sont effacés directement après la création des exécutables) : <br>
      $ make clean

**Commandes d’exécution :**
  - exécution server : <br>
      $ bin/server num_port filename.txt
  - exécution client : <br>
      $ bin/client server_name num_port
      
Veuillez bien mettre le même numéro de port pour le client et le serveur !
