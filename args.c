#include "args.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>



option_t* opt_void(option_t* l, const char* kw, void (*f)()){
	option_t* new = malloc(sizeof(struct option_s));
	new->keyword = kw;
	new->spec = OptVoid;
	new->fct.opt_void = f;
	new->next = l;
	return new;
}

option_t* opt_int(option_t* l, const char* kw, void (*f)(int)){
	option_t* new = malloc(sizeof(struct option_s));
	new->keyword = kw;
	new->spec = OptInt;
	new->fct.opt_int = f;
	new->next = l;
	return new;
}

option_t* opt_float(option_t* l, const char* kw, void (*f)(float)){
	option_t* new = malloc(sizeof(struct option_s));
	new->keyword = kw;
	new->spec = OptFloat;
	new->fct.opt_float = f;
	new->next = l;
	return new;
}

option_t* opt_string(option_t* l, const char* kw, void (*f)(const char*)){
	option_t* new = malloc(sizeof(struct option_s));
	new->keyword = kw;
	new->spec = OptString;
	new->fct.opt_str = f;
	new->next = l;
	return new;
}

void opt_delete(option_t* l){
	free(l);
}


void process_arguments(option_t* l, int argc, char* *argv){
	int i;
	option_t* parcours = l;
	i = 1;
	while(i < argc){
		parcours = l;
		while(parcours != NULL && i < argc){
			if((strcmp(argv[i],parcours->keyword)) == 0){
				if(parcours->spec == OptVoid){
					parcours->fct.opt_void();
					i = i + 1;
				}else if(parcours->spec == OptInt){	
					int entier = atoi(argv[i+1]);				
					parcours->fct.opt_int(entier);
					i = i + 2;
				}else if(parcours->spec == OptString){					
					parcours->fct.opt_str(argv[i + 1]);
					i = i + 2;
				}else if(parcours->spec == OptFloat){
					float decimal = atof(argv[i+1]);
					parcours->fct.opt_float(decimal);
					i = i + 2;
				}			
			}else{
				parcours = parcours->next;
				
			}
		}
	}	
}


