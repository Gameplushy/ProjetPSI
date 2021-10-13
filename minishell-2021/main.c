/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Interface du minishell.
 
 */


#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "processus.h"

int main(int argc, char* argv[]) {
  char line[MAX_LINE_SIZE];
  char* cmdline[MAX_CMD_SIZE];
  process_t cmds[MAX_CMD_SIZE];
  
  while (1) {
    // (ré-)Initialiser les variables/structures
    
    // Affichage d'une invite de commande

    // Lecture d'une ligne de commandes
    fgets(line,MAX_LINE_SIZE,stdin);
	*(strchr(line,'\n'))='\0';
    // "Nettoyage" de la ligne de commandes
    trim(line);
	clean(line);
	printf("\n|%s|\n",line);
    // Découpage en "tokens"
    tokenize(line,cmdline);
    // Parsing de la ligne pour remplir les structures
    // de cmds.
    
    // Lancement des commandes dans l'ordre attendu,
    // avec les éventuelles redirections et conditionnements
    // d'exécution.
  }
  return -1; // Dead code
}
