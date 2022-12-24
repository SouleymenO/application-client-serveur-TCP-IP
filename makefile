#############################################################################
# Fichier Makefile 
# UE SYS - IM2AG - UGA - 2022/2023
#############################################################################
# utilisation des variables internes $@ $< $^ $*
# $@ : correspond au nom de la cible
# $< : correspond au nom de la premiere dependance
# $^ : correspond à toutes les dépendances
# $* : correspond au nom du fichier sans extension 
#       (dans les regles generiques uniquement)
#############################################################################
# information sur la regle executee avec la commande @echo
# (une commande commancant par @ n'est pas affichee a l'ecran)
#############################################################################


#############################################################################
# definition des variables locales
#############################################################################

# compilateur C
CC = gcc

# options de compilation
COMPILOPTS = -Wall

# liste des executables
EXECUTABLES = $(B)client $(B)server

B = bin/
S = sources/
H = headers/

#############################################################################
# definition des regles
#############################################################################

########################################################
# la règle par défaut
all : $(EXECUTABLES)

########################################################
# regles explicites de creation des executables


clib.o : $(S)clib.c
	$(CC) $^ -c -I $(H)

slib.o : $(S)slib.c
	$(CC) $^ -c -I $(H)

cslib.o : $(S)cslib.c
	$(CC) $^ -c -I $(H)

client.o : $(S)client.c
	$(CC) $^ -c -I $(H)

server.o : $(S)server.c
	$(CC) $^ -c -I $(H)


$(B)client : cslib.o clib.o client.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@" dans le fichier bin"
	@echo "---------------------------------------------"
	$(CC) $^ -o $@
	rm -f client.o clib.o

$(B)server : cslib.o slib.o server.o
	@echo ""
	@echo "---------------------------------------------"
	@echo "Creation de l'executable "$@" dans le fichier bin"
	@echo "---------------------------------------------"
	$(CC) $^ -o $@
	rm -f server.o slib.o cslib.o


# regle pour "nettoyer" le répertoire
clean:
	rm -f $(EXECUTABLES)