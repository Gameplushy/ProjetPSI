/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom : FLORENT HERMAN
  Prénom : Victor Loïck
  Num. étudiant : 21900240 22112293
  Groupe de projet : 15
  Date : 2021-11-19
 
  Interface du minishell.
 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.h"
#include "processus.h"

int main() {
  char line[MAX_LINE_SIZE];
  char* cmdline[MAX_CMD_SIZE];
  process_t cmds[MAX_CMD_SIZE];
  
  while (1) {
    // (ré-)Initialiser les variables/structures
    CLEAR(cmdline,MAX_CMD_SIZE);
    CLEAR(cmds,MAX_CMD_SIZE);
    for(int i=0;i<MAX_CMD_SIZE;i++) init_process(&cmds[i]);
    do{
    // Affichage d'une invite de commande
		printf("%s>",getcwd(NULL,0));
    // Lecture d'une ligne de commandes
		fgets(line,MAX_LINE_SIZE,stdin);
		*(strchr(line,'\n'))='\0';
	} while(strcmp(line,"")==0);
    // "Nettoyage" de la ligne de commandes
    	trim(line);
		clean(line);
    // Découpage en "tokens"
    	tokenize(line,cmdline);
    // Parsing de la ligne pour remplir les structures
    // de cmds.
    	int checkerr = parse_cmd(cmdline,cmds);
    // Lancement des commandes dans l'ordre attendu,
    // avec les éventuelles redirections et conditionnements
    // d'exécution.
    	switch(checkerr){
			 case 1: fprintf(stderr,"No file redirected!\n"); break;
			 case 2: fprintf(stderr,"Bad file!\n"); break;
			 case 3: fprintf(stderr,"Not enough stuff!\n"); break;
			 case 0:{
			 	process_t* currentProc=&cmds[0];
			 	while(currentProc!=NULL){
			 		set_env(currentProc);
			 		int returned = launch_cmd(currentProc);
			 		if(strcmp(currentProc->path,"exit")==0) return returned;
			 		if(currentProc->next!=NULL) currentProc=currentProc->next; //Pour ; | &
			 		else{
			 			if(returned==0) currentProc=currentProc->next_success; //Pour && !|| si succès
			 			else currentProc=currentProc->next_failure; //|| !&& et fin de commande 
			 		}
			 	}
			 } 	
    	};
  }
  return -1; // Dead code
}
