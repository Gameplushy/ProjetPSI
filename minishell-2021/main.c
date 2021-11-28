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

int main(int argc, char* argv[]) {
  char line[MAX_LINE_SIZE];
  char* cmdline[MAX_CMD_SIZE];
  process_t cmds[MAX_CMD_SIZE];
  
  while (1) {
  	check_zero(cmds,sizeof(cmds));
    // (ré-)Initialiser les variables/structures
    int i=0;
    while(check_zero(cmds+i,sizeof(cmds[i]))) memset(&cmds[i],0,sizeof(cmds[i++])); //Réinitialiser toute trace de processus
    i=0;
    while(cmdline+i!=NULL && strlen(cmdline[i])!=0) cmdline[i++]="";
    // Affichage d'une invite de commande
	printf("%s>",getcwd(NULL,0));
    // Lecture d'une ligne de commandes
    	fgets(line,MAX_LINE_SIZE,stdin);
	*(strchr(line,'\n'))='\0';
    // "Nettoyage" de la ligne de commandes
    	trim(line);
	clean(line);
	printf("\n|%s|\n",line);
    // Découpage en "tokens"
    	tokenize(line,cmdline);
    	for(int i=0;cmdline[i]!=NULL;i++)
    		printf("%s %d\n",cmdline[i],is_reserved(cmdline[i]));
    // Parsing de la ligne pour remplir les structures
    // de cmds.
    	int checkerr = parse_cmd(cmdline,cmds);
    	switch(checkerr){
    	 case 1: fprintf(stderr,"No file redirected!\n"); break;
    	 case 2: fprintf(stderr,"Bad file\n"); break;
    	 case 0:{
    	 	process_t* currentProc=&cmds[0];
    	 	while(currentProc!=NULL){
    	 		int returned = launch_cmd(currentProc);
    	 		if(currentProc->next!=NULL) currentProc=currentProc->next;
    	 		else{
    	 			if(returned==0) currentProc=currentProc->next_success;
    	 			else currentProc=currentProc->next_failure;
    	 		}
    	 	}
    	 }
    	 	
    	};
    // Lancement des commandes dans l'ordre attendu,
    // avec les éventuelles redirections et conditionnements
    // d'exécution.
  }
  return -1; // Dead code
}
