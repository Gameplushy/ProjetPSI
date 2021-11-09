/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom :
  Prénom :
  Num. étudiant :
  Groupe de projet :
  Date :
 
  Gestion des commandes internes du minishell (implémentation).
 
 */


#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#include "builtin.h"
#include "processus.h"

/*
 La chaîne passée représente-t-elle une commande
 interne ?
 
 cmd : chaîne de caractères.
 
 Retourne 1 si la chaîne passée désigne une
 commande interne (cd, exit, ...)
 */
int is_builtin(const char* cmd) {
  assert(cmd!=NULL);
    char* specialCmds[] = {"exit","cd","pwd","exit","export","unset",NULL};
  for(int i=0;specialCmds[i]!=NULL;i++)
  	if(strcmp(cmd,specialCmds[i])==0) return 1;
  return 0;
}

/*
  Exécute la commande interne
 
  proc : process_t dont le contenu a été initialisé
         au moment du parsing
 
  Retourne 0 si tout s'est bien passé, la valeur de
  retour de la commande ou -1 si la commande n'est
  pas reconnue.
 */

int builtin(process_t* proc) {
  assert(proc!=NULL);
  /*switch(proc->path){
  	default:
   		return -1;
  }*/
  return -1;
}

/*
  Change directory : change le répertoire de travail
  courant du minishell.

  path : le chemin vers lequel déplacer le CWD
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel (erreur du cd)
  
  Retourne le code de retour de l'appel système.
 */

int cd(const char* path, int fderr) {
  assert(path!=NULL);
  //return chdir(path);
}

/*
  Ajout/modification d'une variable d'environnement.
 
  var : nom de la variable
  value : valeur à lui donner
  fderr : le descripteur de la sortie d'erreur pour
          affichage éventuel
 
  Retourne le code de retour de l'appel système.
 */

int export(const char* var, const char* value, int fderr) {
  assert(var!=NULL);
  assert(value!=NULL);
  setenv(var,value,1);  
}

/*
  Quitter le minishell
 
  ret : code de retour du minishell
  fdout : descripteur de la sortie standard pour
          l'affichage éventuel d'un message de sortie
 
  Retourne un code d'erreur en cas d'échec.
 */

int exit_shell(int ret, int fdout) {
  
}
