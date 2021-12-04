/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom : FLORENT HERMAN
  Prénom : Victor Loïck
  Num. étudiant : 21900240 22112293
  Groupe de projet : 15
  Date : 2021-10-11
 
  Parsing de la ligne de commandes utilisateur (implémentation).
 
 */

#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <unistd.h>
#include <fcntl.h>

#include "parser.h"
#include "processus.h"

/*
  Suppression des espaces en début et fin de chaîne.
      Ex : "   chaîne   " => "chaîne"
 
  str : chaîne à modifier
 
  Retourne 0 ou un code d'erreur.
 */
int trim(char* str) {
  assert(str!=NULL);
	char *debut;
	for(size_t i=0;i<strlen(str);i++){
		if(str[i]!=' '){ //Find the first character that isn't a space
			debut = str+i;
			break;
		}
	}
	memmove(str,debut,strlen(debut)+1); //Shove the text at the start of the string (erase starting blanks)

	for(int j=strlen(str)-1;j>=0;j--){
		if(str[j]!=' '){ //Find the last character that isn't a space
			if(j+1<(int)(strlen(str))) str[j+1]='\0'; //Put a \0 right after it (if it isn't there already)
			break;
		}
	}
	return 0;
}

/*
  Suppression des doublons d'espaces.
      Ex : "cmd1   -arg  ;   cmd2  <  input"
        => "cmd1 -arg ; cmd2 < input"
 
  str: chaîne à modifier
 
  Retourne 0 ou un code d'erreur.
 */
int clean(char* str) {
  assert(str!=NULL);
  char *debut, *fin;
  for(size_t i=0;i<strlen(str);i++){
      	if(str[i]==' ') { //Look for a space
			  debut = str+i+1;
			  for(size_t j=i+1;j<strlen(str);j++){
				  if(str[j]!=' '){ //Stop looking when finding a character
				  fin = str+j;
				  break;        
				  }
			  }      
			  if (debut<fin)
				  memmove(debut, fin, strlen(fin)+1); //Move the right part of the string so that there's only 1 space
		}     
  }
  return 0;
}

/*
  Découpage de la chaîne en ses éléments.
    Ex : "ls -l | grep ^a ; cat"
      => {"ls", "-l", "|", "grep", "^a", ";", "cat", NULL}
  str : chaîne à découper (peut être modifiée)
  tokens : tableau des pointeurs sur les éléments de la
           chaîne. Terminé par NULL.
 
  Retourne 0 ou un code d'erreur.
 */
int tokenize(char* str, char* tokens[]) {
  assert(str!=NULL);
  assert(tokens!=NULL);
  int i =0;
  tokens[0] = strtok(str," "); //Put a word in tokens when a space is encountered
  while(tokens[i]!=NULL){
  	tokens[++i]=strtok(NULL," ");
  }
  return 0;
}

/*
  S'agit-il d'un mot réservé du shell ?
  Les mots réservés sont par exemple :
    ";", "&", "<", "2>", "||", ...
 
  tok : la chaîne à tester
 
  Retourne 1 s'il s'agit d'un mot réservé, 0 sinon.
 */
int is_reserved(const char* tok) {
  assert(tok!=NULL);
  char* specialKeys[] = {";","&","<",">","||","&&","|",">>","2>>",">&2","2>&1","2>",NULL};
  for(int i=0;specialKeys[i]!=NULL;i++) //Check the list 1 by 1 for a match
  	if(strcmp(tok,specialKeys[i])==0) return 1;
  return 0;
}

/*
  Remplit le tableau de commandes en fonction du contenu
  de tokens.
    Ex : {"ls", "-l", "|", "grep", "^a", NULL} =>
         {{path = "ls",
           argv = {"ls", "-l", NULL},
           bg = 1,
           ...},
          {path = "grep",
           argv = {"grep", "^a", NULL},
           bg = 0,
           ...}}
  tokens : le tableau des éléments de la ligne de
           commandes (peut être modifié)
  commands : le tableau dans lequel sont stockés les
             différentes structures représentant le
             commandes.
  Retourne 0 ou un code d'erreur.
 */

int parse_cmd(char* tokens[], process_t* commands) {
  assert(tokens!=NULL);
  assert(commands!=NULL);
  
  int i=0; //Iterator for tokens
  int nbProc = 0; //Iterator for commands
  int negator; //For !
  while(tokens[i]!=NULL){
  	if(strcmp(tokens[i],"!")==0){
  	 negator=1;//NOT
  	 i++;
  	 if(tokens[i]==NULL) return 3;
  	}
  	else negator=0;
  	commands[nbProc].path=tokens[i]; //First word should be the command name
  	commands[nbProc].argv[0]=tokens[i];
 	int argn=1;
  	while(tokens[++i]!=NULL && !is_reserved(tokens[i])){ //Keep looking for arguments until it's a reserved keyword
  		commands[nbProc].argv[argn++]=tokens[i];
  	}
  	commands[nbProc].argv[argn]=NULL; //Put a NULL at the end of argv
	if(tokens[i]!=NULL){
		if(strcmp(tokens[i],"&")==0) {
			commands[nbProc].bg=1;
			if(tokens[i+1]!=NULL) commands[nbProc].next=&commands[nbProc+1];
		}
		else if((strcmp(tokens[i],"&&")==0 && negator==0) || (strcmp(tokens[i],"||")==0 && negator==1)){ //AND|NOT OR
		 	commands[nbProc].next_success=&commands[nbProc+1];
		 	if(tokens[i+1]==NULL) return 3;
		 }
		else if((strcmp(tokens[i],"||")==0 && negator==0) || (strcmp(tokens[i],"&&")==0 && negator==1)) { //OR|NOT AND
			commands[nbProc].next_failure=&commands[nbProc+1];
			if(tokens[i+1]==NULL) return 3;
		}
		else if(strcmp(tokens[i],"|")==0) { //Pipeline
			if(tokens[i+1]==NULL) return 3;
	  		int tube[2];
	  		pipe(tube);
	  		commands[nbProc].stdout=tube[1];
	  		commands[nbProc].fdclose[0]=tube[0];
	  		commands[nbProc+1].stdin=tube[0];
	  		commands[nbProc+1].fdclose[1]=tube[1];
	  		commands[nbProc].next=&commands[nbProc+1];
		}
		else{ //redirecttions
			if(strcmp(tokens[i],">")==0 || strcmp(tokens[i],"2>")==0){ //Sorties avec écrasement
				if(tokens[i+1]==NULL) return 1;
				int newFile = open(tokens[i+1],O_WRONLY|O_TRUNC|O_CREAT,0744);
				if(newFile==-1) return 2;
				if(strcmp(tokens[i],">")==0) commands[nbProc].stdout=newFile;
				else if(strcmp(tokens[i],"2>")==0) commands[nbProc].stderr=newFile;
				++i;
			}
			else if (strcmp(tokens[i],">>")==0 || strcmp(tokens[i],"2>>")==0){ //Sorties sans écrasement
				if(tokens[i+1]==NULL) return 1;
				int newFile = open(tokens[i+1],O_WRONLY|O_APPEND|O_CREAT,0744);
				if(newFile==-1) return 2;
				if(strcmp(tokens[i],">>")==0) commands[nbProc].stdout=newFile;
				else if(strcmp(tokens[i],"2>>")==0) commands[nbProc].stderr=newFile;
				++i;
			}
			else if(strcmp(tokens[i],"<")==0){
				if(tokens[i+1]==NULL) return 1;
				int newFile = open(tokens[i+1],O_RDONLY|O_CREAT,0744);
				if(newFile==-1) return 2;
				commands[nbProc].stdin=newFile;
				++i;
			}
			else if(strcmp(tokens[i],"2>&1")==0) commands[nbProc].stderr=commands[nbProc].stdout;
			else if(strcmp(tokens[i],">&2")==0) commands[nbProc].stdout=commands[nbProc].stderr;
			else commands[nbProc].next=&commands[nbProc+1]; //Sinon c'est ; ou rien du tout, donc aucun interférence spécifique avec le proc suivant.
		}
		nbProc++;	
  		i++;	
	}
  }
  return 0;
}
