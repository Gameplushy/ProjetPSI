/*
  Projet minishell - Licence 3 Info - PSI 2021
 
  Nom : FLORENT HERMAN
  Prénom : Victor Loïck
  Num. étudiant : 21900240 22112293
  Groupe de projet : 15
  Date : 2021-11-19
 
  Parsing de la ligne de commandes utilisateur (headers).
 
 */

#ifndef PARSER_H
#define PARSER_H

#define MAX_LINE_SIZE 1024
#define MAX_CMD_SIZE 256

#include <string.h>

#include "processus.h"

// Effacer un tableau (tout mettre à zéro)
#define CLEAR(ptr, size) memset((ptr), 0, (size)*sizeof(*ptr))

int trim(char* str);
int clean(char* str);
int tokenize(char* str, char* tokens[]);

int is_reserved(const char* tok);
int parse_cmd(char* tokens[], process_t* commands);
#endif
