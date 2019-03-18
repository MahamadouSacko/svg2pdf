#include "xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void * mymalloc(size_t sz){
    void * p=malloc(sz);
    if(p==NULL){
        char *s="l'allocation a echoue";
        fwrite(s,sizeof(char),strlen(s)+1,stderr);
        printf("\n");
        exit(EXIT_FAILURE);
        return NULL;
    }else{
        return p;
    }
} 

FILE* myfopen(const char* fname, const char* mode){
    FILE * fout=fopen(fname,mode);
    if(fout==NULL){
        char * s="impossible d'auvrir le fichier";
        fwrite(s,sizeof(char),strlen(s),stderr);
        printf("\n");
        exit(EXIT_FAILURE);
        
    }else{
        
        return fout;
    }
}

xelement_t* create_xelement(const char* name){
    xelement_t *e=(xelement_t*)mymalloc(sizeof( xelement_t));
    e->nom_element=mymalloc(strlen(name)+1);
    strcpy(e->nom_element,name);
    e->AT=NULL;
    e->pere=NULL;
    e->frere=NULL;
    e->contenu.etiquete=vide;
    e->contenu.type.fils=NULL;
    e->contenu.type.raw=NULL;
    return e;
}

xattribute_t * add_xattribute(xelement_t* e, const char* name, const char* value){
    bool verif=false;
    xattribute_t * a=(xattribute_t *)mymalloc(sizeof(xattribute_t));
    if(e->AT==NULL){
        a->nom_attribut=mymalloc(strlen(name)+1);
        strcpy(a->nom_attribut,name);
        a->valeur_attribut=mymalloc(strlen(value)+1);
        strcpy(a->valeur_attribut,value);
        a->next=NULL;
        e->AT=a;
    }else{
        xattribute_t * b=EMPTY_AT;
        xattribute_t * prec=EMPTY_AT;
        b=e->AT;
        while(b!=NULL){
            if (strcmp(b->nom_attribut,name)==0){
                free(b->valeur_attribut);
                b->valeur_attribut=mymalloc(strlen(value)+1);
                strcpy(b->valeur_attribut,value);
                verif=true;
            }
            prec=b;
            b=b->next;
        }
        if(verif==false){
            a->nom_attribut=mymalloc(strlen(name)+1);
            strcpy(a->nom_attribut,name);
            a->valeur_attribut=mymalloc(strlen(value)+1);
            strcpy(a->valeur_attribut,value);
            prec->next=a;
            a->next=NULL;
        }
    }
    return e->AT;
    
}

void add_sub_xelement(xelement_t* e, xelement_t* s){
    if(s->pere==NULL&&(e->contenu.etiquete==vide)){
        s->pere=e;
        e->contenu.etiquete=fils;
        e->contenu.type.fils=s;
        s->frere=NULL;
    }else if(s->pere==NULL&&e->contenu.etiquete==fils){
        s->pere=e;
        xelement_t *b=e->contenu.type.fils;
        xelement_t *prec=EMPTY_AE;
        while(b!=NULL){
            prec=b;
            b=b->frere;
        }
        prec->frere=s;
        s->frere=NULL;
    }else{
        fprintf(stderr, "faillure add_sub_xelement\n");
        exit(EXIT_FAILURE); 
    }
}

void add_raw(xelement_t* e, const char* r){
    if(e->contenu.etiquete==raw){
        e->contenu.etiquete=raw;
        free(e->contenu.type.raw);
        e->contenu.type.raw=mymalloc(strlen(r)+1);
        strcpy(e->contenu.type.raw,r);
    }else if(e->contenu.etiquete==vide){
        e->contenu.etiquete=raw;
        e->contenu.type.raw=mymalloc(strlen(r)+1);
        strcpy(e->contenu.type.raw,r);
    }else{
        fprintf(stderr, "add_raw\n");
        exit(EXIT_FAILURE); 
    }
}
void delete_xelement(xelement_t* e){
    if(e->pere==NULL){
        free(e);
        e=NULL;
    }
}

void save_xelement(FILE* fd, xelement_t* e){
    char * auvrante="<";
    char * ferment=">";
    char * auvrante2="</";
    char * ferment2=">";
    char * ferment3="/>";
    char * egal="=";
    char * epsace=" ";
    char * ligne="\n";
    char * guillemets="\"";
    fprintf(fd, "%s",auvrante);
    fprintf(fd, "%s",e->nom_element);
    liste_attribute l=e->AT;
    while(l!=NULL){
        fprintf(fd, "%s",epsace);
        fprintf(fd, "%s",l->nom_attribut);
        fprintf(fd, "%s",egal);
        fprintf(fd, "%s",guillemets);
        fprintf(fd, "%s",l->valeur_attribut);
        fprintf(fd, "%s",guillemets);
        l=l->next;
    }
    
    if(e->contenu.etiquete==vide){
        fprintf(fd, "%s",ferment3);
    }else{
        fprintf(fd, "%s",ferment);
        if(e->contenu.etiquete==fils){
            xelement_t * temp=e->contenu.type.fils;
            while(temp!=NULL){
                fprintf(fd, "%s",ligne);
                save_xelement(fd,temp);
                temp=temp->frere;
            }
        }else if(e->contenu.etiquete==raw){
            fprintf(fd, "%s",e->contenu.type.raw);
        }else{
            fprintf(stderr, "faillure save_xelement");
            exit(EXIT_FAILURE);
        }
        if(e->pere==NULL||e->pere->pere==NULL)
            fprintf(fd, "%s",ligne);
        fprintf(fd, "%s",auvrante2);
        fprintf(fd, "%s",e->nom_element);
        fprintf(fd, "%s",ferment2);
    }
}

void save_xml(const char* fname, xelement_t* e){
    FILE *fd=myfopen(fname,"w");
    save_xelement(fd,e);
}

void print_xelement(xelement_t* e) {
    save_xelement(stdout, e);
}

char next_char(FILE* fd){
    int c=fgetc(fd);
    if(c==EOF){
        fprintf(stderr, "faillure next_char");
        exit(EXIT_FAILURE);
    }else if(c==' '||c=='\n'||c=='\r'||c=='\t'){
        return next_char(fd);
    }
    return c;
}

void check_next_char(FILE* fd, char c){
    int n=next_char(fd);
    if(n!=c){    
        fprintf(stderr, "faillure check_next_char ");
        exit(EXIT_FAILURE);
    }
}

bool is_next_char(FILE* fd, char c, bool cons){
    int n=next_char(fd);
    if(n!=c){
        ungetc(n,fd);
        return false;
    }else if(n==c&&cons==false){
        ungetc(n,fd);
        return true;
    }else {
        return true;
    }
}


char* next_word(FILE* fd){
    int size=size_word;
    int cpt=0;
    char * ch=NULL;
    ch=mymalloc(size);
    char c;
    c=next_char(fd);
    while((c!='<')&& (c!='>')&&(c!='/')&&(c!='=')&&(c!=' ')&&(c!='"')&&(c!='\n')&&(c!='\t')&&(c!='\r')&&(c!='\b')&&(c!='\v')){
        if(cpt>size){
            ch=realloc(ch,2*size);
            size *=2;
        }
        ch[cpt]=c;
        c=fgetc(fd);
        cpt++;
        if(c==EOF){
            free(ch);
            fprintf(stderr, "faillure next_word");
            exit(EXIT_FAILURE);
        }
    }
    if(cpt==0){
        if(c=='>'||c=='/'){
            ungetc(c,fd);
        }
        return NULL;
    }
    ungetc(c,fd);
    ch[cpt]='\0';
    return ch;
}


void check_next_word(FILE* fd, const char* w){
    char *ch=next_word(fd);
    if (strcmp(ch,w)!=0){
        fprintf(stderr, "faillure check_next_word");
        exit(1);
    }
}

char* next_string(FILE* fd){
    int size=size_string;
    int cpt=0;
    char * ch=NULL;
    ch=mymalloc(size);
    char c;
    is_next_char(fd,'"',true);
    c=next_char(fd);
    while((c!='"')){
        if(cpt>size){
            ch=realloc(ch,2*size);
            size *=2;
        }
        ch[cpt]=c;
        c=fgetc(fd);
        cpt++;
        if(c==EOF){
            free(ch);
            fprintf(stderr, "faillure next_string");
            exit(EXIT_FAILURE);
        }
    }
    ch[cpt]='\0';
    return ch;
}

char* next_raw(FILE* fd){
    int size=size_raw;
    int cpt=0;
    char * ch=NULL;
    ch=mymalloc(size);
    char c;
    c=next_char(fd);
    while((c!='<')){
        if(cpt>size){
            ch=realloc(ch,2*size);
            size *=2;
        }
        ch[cpt]=c;
        c=fgetc(fd);
        cpt++;
        if(c==EOF){
            free(ch);
            fprintf(stderr, "faillure next_raw");
            exit(EXIT_FAILURE);
        }
    }
    ungetc(c,fd);
    ch[cpt]='\0';
    return ch;
}

xelement_t* load_xelement(FILE* fd, const char* end_tag);

void load_xelement_raw(FILE* fd, xelement_t* e){
    char* w = next_raw(fd);
    check_next_char(fd, '<');
    check_next_char(fd, '/');
    check_next_word(fd, e->nom_element);
    check_next_char(fd, '>');
    add_raw(e,w);
    free(w);
}

void load_xelement_sub(FILE* fd, xelement_t* e) {
    xelement_t* f = load_xelement(fd, e->nom_element);
    if (f != NULL) {
        add_sub_xelement(e,f);
        load_xelement_sub(fd, e);
    }
}

void load_xelement_content(FILE* fd, xelement_t* e) {
    if (is_next_char(fd,'<',false))
        load_xelement_sub(fd, e);
    else
        load_xelement_raw(fd, e);
}

xelement_t* load_xelement(FILE* fd, const char* end_tag) {
    xelement_t* e = NULL;
    char c;

    check_next_char(fd,'<');
    
    if ((end_tag) && (is_next_char(fd,'/',true))) {
        check_next_word(fd,end_tag);
        check_next_char(fd,'>');
        return NULL;
    }
    
    char* name = next_word(fd);
    if (name == NULL) {
        fprintf(stderr, "load_xelement: tag name expected\n");
        exit(EXIT_FAILURE);
    }
    e = create_xelement(name);
    free(name);
    
    while((name = next_word(fd)) != NULL) {
        check_next_char(fd,'=');
        char* value = next_string(fd);
        add_xattribute(e,name,value);
    }
    c = next_char(fd);
    
    if (c == '/') {
        check_next_char(fd,'>');
        return e;
    }
    
    if (c == '>') {
        load_xelement_content(fd, e);
        return e;
    }
    
    fprintf(stderr, "load_xelement: end of markup expected ('>' or '/>'), but got %c\n", c);
    exit(EXIT_FAILURE);
}

xelement_t* load_xml(const char* fname) {
    FILE* fd = myfopen(fname, "r");
    xelement_t* e = load_xelement(fd,NULL);
    fclose(fd);
    return e;
}
