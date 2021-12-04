/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom : FLORENT HERMAN
  Prénom : Victor Loïck
  Num. étudiant : 21900240 22112293
  Groupe de projet : 15
  Date : 2021-11-19
 
  Gestion des processus (implémentation).
 
 */

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/wait.h>

#include "processus.h"
#include "builtin.h"

#include <stdio.h>

#ifndef NDEBUG
int check_zero(void* ptr, size_t size) {
  int result=0;
  for (size_t i=0; i<size; ++i) {
    result+=*((char*) ptr++);
  }
  return result;
}
#endif

/*
  Initialiser une structure process_t avec les
  valeurs par défaut.
 
  proc : un pointeur sur la structure à initialiser
 
  Retourne 0 ou un code d'erreur.
 */
int init_process(process_t* proc) {
  assert(proc!=NULL);
  assert(check_zero(proc, sizeof(*proc))==0);
  process_t newProc;
  *proc=newProc;
  proc->pid=0;
  proc->path="";
  proc->next=NULL;
  proc->argv=(char**)malloc(sizeof(char*)*20); //Arbitrary number
  proc->stdin=0;
  proc->stdout=1;
  proc->stderr=2;
  proc->bg=0;
  proc->next=NULL;
  proc->next_success=NULL;
  proc->next_failure=NULL;
  proc->fdclose[0]=-1; proc->fdclose[1]=-1;
  return 0;
}

/*
  Remplacer les noms de variables d'environnement
  par leurs valeurs.
 
  proc : un pointeur sur la structure à modifier.
 
  Retourne 0 ou un code d'erreur.
 */
int set_env(process_t* proc) {
  assert(proc!=NULL);
  int i=1;
  while(proc->argv[i]!=NULL){
  	if(proc->argv[i][0]=='$') //Check if it's a variable (don't do anything if it's not)
  		proc->argv[i]=getenv(proc->argv[i]+1);
  	i++;
  }
  return 0;
}

/*
  Lancer la commande en fonction des attributs de
  la structure et initialiser les champs manquants.
 
  On crée un nouveau processus, on détourne
  éventuellement les entrée/sorties.
  On conserve le PID dans la structure du processus
  appelant (le minishell).
  On attend si la commande est lancée en "avant-plan"
  et on initialise le code de retour.
  On rend la main immédiatement sinon.
 
  proc : un pointeur sur la structure contenant les
         informations pour l'exécution de la commande.
 
  Retourne 0 ou un code d'erreur.
  
 */
int launch_cmd(process_t* proc) {
  assert(proc!=NULL);
  proc->pid=fork();
  if((proc->pid)!=0){ //Proc minishell
  	if(!proc->bg) waitpid(proc->pid,&(proc->status),0);
  	return WEXITSTATUS(proc->status);
  }
  else{ //Proc commande
  	if(proc->stdin!=0){
  		dup2(proc->stdin,0);
  		close(proc->stdin);
  	}
  	if(proc->stdout!=1){
  		dup2(proc->stdout,1);
  		close(proc->stdout);
  	}
  	if(proc->stdout!=2){
  		dup2(proc->stderr,2);
  		close(proc->stderr);
  	}
  	if(proc->fdclose[0]!=-1) close(proc->fdclose[0]);
  	if(proc->fdclose[1]!=-1) close(proc->fdclose[1]);
  	if(is_builtin(proc->path)) return builtin(proc);
  	else return execvp(proc->path,proc->argv);
  	return -1;
  }
  
}
