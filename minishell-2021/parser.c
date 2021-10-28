/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom : FLORENT HERMAN
  Prénom : Victor Loïck
  Num. étudiant : 21900240
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
	for(int i=0;i<strlen(str);i++){
		if(str[i]!=' '){
			debut = str+i;
			break;
		}
	}
	memmove(str,debut,strlen(debut)+1);

	for(int j=strlen(str)-1;j>=0;j--){
		if(str[j]!=' '){
			if(j+1<strlen(str)) str[j+1]='\0';
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
  for(int i=0;i<strlen(str);i++){
      if(str[i]==' ') {
				  debut = str+i+1;
				  for(int j=i+1;j<strlen(str);j++){
				      if(str[j]!=' '){
				          fin = str+j;
				          break;        
				      }
				  }      
				  if (debut<fin)
				      memmove(debut, fin, strlen(fin)+1);
			}     
  }
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
  tokens[0] = strtok(str," ");
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
  char* specialKeys[] = {";","&","<",">","||","&&","|","<<","<<<",">&1",">&2",NULL,"2>"};
  for(int i=0;specialKeys[i]!=NULL;i++)
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
  
  
}
