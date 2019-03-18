#ifndef MENU_H
#define MENU_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct item_u{
	union {
		struct menu_u* sousMenu;
		struct action_u* action;
	}type;
	enum{
		sousMenu, action, Vide
	}etiquette;
}item_t;

typedef struct action_u{
 char* descriptif;
 void (*action)();
}action_t;

typedef struct menu_u{
 	struct menu_u* parent;
 	char* descriptif;
 	item_t items [9];
}menu_t;

menu_t* createMenu(const char* text);
void addMenuAction(menu_t* m, const char* text, void(*f)());
void addSubMenu(menu_t* m, menu_t* sm);
void deleteMenu(menu_t* m);
int readChoice();
void launchMenu(menu_t* m);
void viderBuffer();

#endif
