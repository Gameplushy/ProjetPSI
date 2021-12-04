/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom : FLORENT HERMAN
  Prénom : Victor Loïck
  Num. étudiant : 21900240 22112293
  Groupe de projet : 15
  Date : 2021-11-19
 
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
    char* specialCmds[] = {"exit","cd","export","unset",NULL};
  for(int i=0;specialCmds[i]!=NULL;i++){//Cycle the special command list until match or NULL reached
  	if(strcmp(cmd,specialCmds[i])==0) return 1;
  }
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
	if(strcmp(proc->path,"cd")==0) {
		if(proc->argv[1]==NULL||proc->argv[2]!=NULL) return -1; //Make sure there's only one destination
		return cd(proc->argv[1],proc->stderr);
	}
	if(strcmp(proc->path,"export")==0){
		if(proc->argv[1]==NULL||proc->argv[2]!=NULL) return -1; //Make sure there's only one destination
		int c = 0;
		while(proc->argv[1][c]!='\0' && proc->argv[1][c++]!='='); //
		if(proc->argv[1][--c]=='='){ //Check for a = in order to seperate variable from value
			proc->argv[1][c]='\0';
			return export(proc->argv[1],proc->argv[1]+c+1,proc->stderr);
		}
		else return -1;
	}
	if(strcmp(proc->path,"exit")==0) return exit_shell(atoi(proc->argv[1]),proc->stdout);
	if(strcmp(proc->path,"unset")==0) return unsetVar(proc->argv[1],proc->stderr);
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
  //assert(path!=NULL);
  if(path==NULL){
  	dprintf(fderr,"Vous n'avez pas donné de répertoire!");
  	return 1;
  } 
  int retvalue = chdir(path);
  if(retvalue==-1) dprintf(fderr,"Le répertoire de travail n'a pas changé vers %s.\n",path);
  return retvalue;
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
  int retvalue = setenv(var,value,1);  
  if(retvalue==-1) dprintf(fderr,"Export de la variable %s a retourné la valeur %d\n",var,retvalue);
  return retvalue;
}

/*
	Suppression d'une variable d'environnement
	
	var : nom de la variable
	fderr
*/

int unsetVar(const char* var, int fderr) {
	assert(var!=NULL);
	int retvalue = unsetenv(var);
	if(retvalue==-1) dprintf(fderr,"Destruction de la variable %s a retourné la valeur %d\n",var,retvalue);
	return retvalue;
}

/*
  Quitter le minishell
 
  ret : code de retour du minishell
  fdout : descripteur de la sortie standard pour
          l'affichage éventuel d'un message de sortie
 
  Retourne un code d'erreur en cas d'échec.
 */

int exit_shell(int ret, int fdout) {
	dprintf(fdout,"Fermeture avec code de retour %d\n",ret);  
	return ret;
}
