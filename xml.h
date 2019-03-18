#ifndef XML_H
#define XML_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define size_word 20
#define size_string 20
#define size_raw 256

typedef struct xattribute_s{
    char * nom_attribut;
    char * valeur_attribut;
    struct  xattribute_s * next;
}xattribute_t;

typedef  xattribute_t * liste_attribute;

#define EMPTY_AT ((liste_attribute)NULL)

typedef struct contenu_s{
    union{
        char *raw ;
        struct xelement_s * fils;
    }type;
    enum{
        raw,fils,vide
    }etiquete;
}contenu_t;

#define EMPTY_AT ((liste_attribute)NULL)

typedef struct xelement_s{
    char * nom_element;
    liste_attribute AT;
    struct xelement_s * pere;
    struct xelement_s * frere;
    contenu_t contenu;
}xelement_t;

typedef xelement_t *liste_element;

#define EMPTY_AE ((liste_element)NULL)

void * mymalloc(size_t sz);
FILE* myfopen(const char* fname, const char* mode);
xelement_t* create_xelement(const char* name);
xattribute_t * add_xattribute(xelement_t* e, const char* name, const char* value);
void add_sub_xelement(xelement_t* e, xelement_t* s);
void add_raw(xelement_t* e, const char* r);
void delete_xelement(xelement_t* e);
void save_xelement(FILE* fd, xelement_t* e);
void save_xml(const char* fname, xelement_t* e);
void print_xelement(xelement_t* e);
char next_char(FILE* fd);
void check_next_char(FILE* fd, char c);
char* next_word(FILE* fd);
void check_next_word(FILE* fd, const char* w);
char* next_string(FILE* fd);
char* next_raw(FILE* fd);
xelement_t* load_xelement(FILE* fd, const char* end_tag);
void load_xelement_raw(FILE* fd, xelement_t* e);
void load_xelement_sub(FILE* fd, xelement_t* e);
void load_xelement_content(FILE* fd, xelement_t* e);
xelement_t* load_xml(const char* fname);

#endif //XML_H
