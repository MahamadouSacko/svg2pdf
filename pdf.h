#ifndef PDF_H
#define PDF_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "xml.h"
#define prl { printf("\n"); }

typedef struct reference_s{
    int id;
    int version;
    char ref;
}reference_t;

typedef struct stream_s{
    struct dictionaires_s * dic;
    void * data;
}stream_t;

typedef struct dictionaires_s{
    char * cles;
    struct object_s * valeur;
    struct dictionaires_s * next;
}dictionaires_t;

typedef struct tableaux_s{
    size_t taille;
    struct object_s  ** tab;
}tableaux_t;

typedef struct object_s{
    union{
        char * null;
        bool boolean;
        int entier;
        float reel;
        reference_t reference;
        char * nom;
        char * chaine;
        stream_t stream;
        tableaux_t tableaux;
        dictionaires_t * dictionaires;
    }object_direct;
    enum{
        null,boolean,entier,reel,chaine,nom,stream,tableaux,dictionaires,reference
    }type;
}pdf_object_t;
pdf_object_t * pdf_null(pdf_object_t* o);
pdf_object_t* pdf_bool(pdf_object_t* o, bool b);
pdf_object_t* pdf_int(pdf_object_t* o, int i);
pdf_object_t* pdf_real(pdf_object_t* o, float f);
pdf_object_t* pdf_reference(pdf_object_t* o, unsigned int id);
pdf_object_t* pdf_name(pdf_object_t* o, const char* n);
pdf_object_t* pdf_string(pdf_object_t* o, const char* s);
pdf_object_t* pdf_stream(pdf_object_t* o, size_t sz, void* data);
pdf_object_t* pdf_array(pdf_object_t* o, size_t n);
pdf_object_t* pdf_dictionary(pdf_object_t* o);
void pdf_object_free(pdf_object_t * o);
pdf_object_t* pdf_stream_from_file(pdf_object_t* o, const char* fname);
pdf_object_t* pdf_stream_from_string(pdf_object_t* o, const char* str);
pdf_object_t* pdf_array_get(pdf_object_t* a, size_t i);
pdf_object_t* pdf_dict_get(pdf_object_t* d, const char* cle);
void pdf_fprint_object(FILE* fd, pdf_object_t* o);

typedef struct pdf_s{
    size_t size;
    pdf_object_t ** PDF;
}pdf_t;

pdf_object_t* pdf_default_catalog(pdf_object_t* c);
pdf_object_t* pdf_default_pages(pdf_object_t* p,unsigned int nb);
pdf_object_t* pdf_default_page(pdf_object_t* p,float w, float h);
pdf_object_t* pdf_default_fonts(pdf_object_t* f);
pdf_t* pdf_create(unsigned int nb, float w, float h);
void pdf_delete(pdf_t* p);
size_t pdf_new_id(pdf_t* p);
void pdf_save(const char* fname, pdf_t* p);
void pdf_set_content(pdf_t* p, unsigned int n, const char* instr);
bool jpeg_info(const void* i, size_t* width, size_t* height, size_t* bpc, const char** cs);
char* pdf_load_image(pdf_t* p, const char* fname);

#endif //PDF_H
