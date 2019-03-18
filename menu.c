#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

menu_t* createMenu(const char* text){
	int i;
	menu_t* m = malloc(sizeof(menu_t));
	m->descriptif = malloc(strlen(text)+1);
	strcpy(m->descriptif, text);
	m->parent = NULL;
	for(i=0; i<9; i++){
		m->items[i].etiquette = Vide;
		m->items[i].type.sousMenu = NULL;
		m->items[i].type.action = NULL;
	}
	
	return m;
	

}

void addMenuAction(menu_t* m, const char* text, void(*f)()){
	int i = 0;
	while(m->items[i].etiquette != Vide && i<7)i++;
	
	m->items[i].etiquette = action;
	
	m->items[i].type.action = malloc(sizeof(action_t));
	m->items[i].type.action->descriptif = malloc(strlen(text)+1);
	strcpy(m->items[i].type.action->descriptif, text);
	m->items[i].type.action->action = f;
	
}

void addSubMenu(menu_t* m, menu_t* sm){
	int i = 0;
	if(sm->parent == NULL){
		while(m->items[i].etiquette != Vide && i<9)i++;
		m->items[i].etiquette = sousMenu;
		m->items[i].type.sousMenu = malloc(sizeof(menu_t));
		m->items[i].type.sousMenu = sm;
		sm->parent = m;
	}
}

void deleteMenu(menu_t* m){
	if(m->parent == NULL){
		free(m);
	}
}

void viderBuffer(){
  int c = 0;
  while(c != '\n' && c != EOF) c = getchar();
}

int readChoice(){
	char* choice = malloc(sizeof(char));
	printf("Veuillez saisir votre choix:\n");
	scanf("%c", choice);
	if(*choice=='p')choice="112";
    int n = atoi(choice);
    viderBuffer();
    return n;
}


void launchMenu(menu_t* m){
	int i;

	// affichage du menu
	printf("%s\n", m->descriptif);
	for(i = 0; i<8; i++){
		if(m->items[i].etiquette == action){
			printf("%d - %s\n",(i+1), m->items[i].type.action->descriptif);
		}else if(m->items[i].etiquette == sousMenu){
			printf("%d - %s\n", (i+1),m->items[i].type.sousMenu->descriptif);
		}
	}
	
	int choix = readChoice();
	if(choix == 'p'&& m->parent != NULL){
		launchMenu(m->parent);
	}else if(choix == 0 || choix > 9 || m->items[choix-1].etiquette == Vide){
		printf("Option invalide!\n\n");
	}else if(choix == 'p'-'0' && m->parent == NULL){
		printf("Option invalide!\n\n");
	}else if(choix >= 1 && choix <= 9){
		if(m->items[choix-1].etiquette == action)m->items[choix-1].type.action->action();
		if(m->items[choix-1].etiquette == sousMenu)launchMenu(m->items[choix-1].type.sousMenu);

	}
}

