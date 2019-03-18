#include "pdf.h"
#include "xml.h"
#define _GNU_SOURCE
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

pdf_object_t * pdf_null(pdf_object_t* o){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->object_direct.null=mymalloc(strlen("null")+1);
        strcpy(o->object_direct.null,"null");
        o->type=null;
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->object_direct.null=mymalloc(strlen("null")+1);
        strcpy(o->object_direct.null,"null");
        o->type=null;
        return o;
    }else{
        fprintf(stderr, "faillure pdf_null\n");
        exit(EXIT_FAILURE); 
    }
    
}
pdf_object_t* pdf_bool(pdf_object_t* o, bool b){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=boolean;
        o->object_direct.boolean=b;
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=boolean;
        o->object_direct.boolean=b;
        return o;
    }else{
        fprintf(stderr, "faillure pdf_bool\n");
        exit(EXIT_FAILURE); 
    }
}

pdf_object_t* pdf_int(pdf_object_t* o, int i){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=entier;
        o->object_direct.entier=i;
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=entier;
        o->object_direct.entier=i;
        return o;
    }else{
        fprintf(stderr, "faillure pdf_int\n");
        exit(EXIT_FAILURE); 
    }
}
pdf_object_t* pdf_real(pdf_object_t* o, float f){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=reel;
        o->object_direct.reel=f;
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=reel;
        o->object_direct.reel=f;
        return o;
    }else{
        fprintf(stderr, "faillure pdf_real\n");
        exit(EXIT_FAILURE); 
    }
}

pdf_object_t* pdf_reference(pdf_object_t* o, unsigned int id){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=reference;
        o->object_direct.reference.id=id+1;
        o->object_direct.reference.version=0;
        o->object_direct.reference.ref='R';
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=reference;
        o->object_direct.reference.id=id+1;
        o->object_direct.reference.version=0;
        o->object_direct.reference.ref='R';
        return o;
    }else{
        fprintf(stderr, "faillure pdf_reference\n");
        exit(EXIT_FAILURE); 
    }
}
pdf_object_t* pdf_name(pdf_object_t* o, const char* n){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=nom;
        o->object_direct.nom=mymalloc(strlen(n)+1);
        strcpy(o->object_direct.nom,n);
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=nom;
        o->object_direct.nom=mymalloc(strlen(n)+1);
        strcpy(o->object_direct.nom,n);
        return o;
    }else{
        fprintf(stderr, "faillure pdf_name\n");
        exit(EXIT_FAILURE); 
    }
    
}
pdf_object_t* pdf_string(pdf_object_t* o, const char* s){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=chaine;
        o->object_direct.chaine=mymalloc(strlen(s)+1);
        strcpy(o->object_direct.chaine,s);
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=chaine;
        o->object_direct.chaine=mymalloc(strlen(s)+1);
        strcpy(o->object_direct.chaine,s);
        return o;
    }else{
        fprintf(stderr, "faillure pdf_string\n");
        exit(EXIT_FAILURE); 
    }
    
}

pdf_object_t* pdf_stream(pdf_object_t* o, size_t sz, void* data){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=stream;
        dictionaires_t * d=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        d->cles=mymalloc(strlen("Length")+1);
        strcpy(d->cles,"Length");
        d->valeur=pdf_int(d->valeur,sz);
        d->next=NULL;
        o->object_direct.stream.dic=d;
        o->object_direct.stream.data=data;
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=stream;
        dictionaires_t * d=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        d->cles=mymalloc(strlen("Length")+1);
        strcpy(d->cles,"Length");
        pdf_object_t * i=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        i->type=entier;
        i->object_direct.entier=sz;
        d->valeur=i;
        d->next=NULL;
        o->object_direct.stream.dic=d;
        o->object_direct.stream.data=data;
        return o;
    }else{
        fprintf(stderr, "faillure pdf_stream\n");
        exit(EXIT_FAILURE); 
    }
    
}
pdf_object_t* pdf_array(pdf_object_t* o, size_t n){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=tableaux;
        o->object_direct.tableaux.tab=mymalloc(n*(sizeof(pdf_object_t)));
        o->object_direct.tableaux.taille=n;
        size_t i=0;
        for (i = 0; i<n ; i++){
            o->object_direct.tableaux.tab[i]=pdf_null(o->object_direct.tableaux.tab[i]);
            
        }
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=tableaux;
        o->object_direct.tableaux.tab=mymalloc(n*(sizeof(pdf_object_t)));
        o->object_direct.tableaux.taille=n;
        size_t i=0;
        for (i = 0; i <n ; i++){
            o->object_direct.tableaux.tab[i]=pdf_null(o->object_direct.tableaux.tab[i]);
            
        }
        return o;
    }else{
        fprintf(stderr, "faillure pdf_array\n");
        exit(EXIT_FAILURE); 
    }
    
}

pdf_object_t* pdf_dictionary(pdf_object_t* o){
    if(o==NULL){
        o=(pdf_object_t *)mymalloc(sizeof(pdf_object_t));
        o->type=dictionaires;
        o->object_direct.dictionaires=NULL;
        return o;
    }else if(o!=NULL){
        pdf_object_free(o);
        o->type=dictionaires;
        o->object_direct.dictionaires=NULL;
        return o;
    }else{
        fprintf(stderr, "faillure pdf_array\n");
        exit(EXIT_FAILURE); 
    }
}
void pdf_object_free(pdf_object_t * o){
    if(o==NULL){
    }else if(o->type==null){
        free(o->object_direct.null);
        o->object_direct.null=NULL;
    }else if(o->type==boolean){
        
    }else if(o->type==entier){
        
    }else if(o->type==reel){
        
    }else if(o->type==chaine){
        free(o->object_direct.chaine);
        o->object_direct.chaine=NULL;
    }else if(o->type==nom){
        free(o->object_direct.nom);
        o->object_direct.nom=NULL;
    }else if(o->type==stream){
        dictionaires_t * cur =o->object_direct.stream.dic;
        while(cur!=NULL){
            free(cur->cles);
            cur->cles=NULL;
            pdf_object_free(cur->valeur);
            cur->valeur=NULL;
            cur=cur->next;
        }
        free(o->object_direct.stream.data);
        o->object_direct.stream.data=NULL;
    }else if(o->type==tableaux){
        free(o->object_direct.tableaux.tab);
        o->object_direct.tableaux.tab=NULL;
    }else if(o->type==dictionaires){
        dictionaires_t * cur =o->object_direct.dictionaires;
        while(cur!=NULL){
            free(cur->cles);
            cur->cles=NULL;
            pdf_object_free(cur->valeur);
            cur->valeur=NULL;
            cur=cur->next;
        }
    }else if(o->type==reference){
        
    }else{
        fprintf(stderr, "faillure pdf_object_free\n");
        exit(EXIT_FAILURE); 
    }
}

pdf_object_t* pdf_stream_from_file(pdf_object_t* o, const char* fname){
    FILE *fd=myfopen(fname,"r");
    fseek(fd,0,SEEK_END);
    size_t taille=ftell(fd);
    fseek(fd,0,SEEK_SET);
    char *ch=(char *)mymalloc(taille);
    fread(ch,sizeof(char),taille,fd);
    pdf_stream(o,taille,ch);
    return o;
}
pdf_object_t* pdf_stream_from_string(pdf_object_t* o, const char* str){
    size_t taille=strlen(str);
    char * ch=mymalloc(taille+1);
    strcpy(ch,str);
    pdf_stream(o,taille,ch);
    return o; 
}

pdf_object_t* pdf_array_get(pdf_object_t* a, size_t i){
    if(a->type==tableaux){
        if(a!=NULL){
            return a->object_direct.tableaux.tab[i];
        }else{
            return NULL;
            
        }
    }else{
        return NULL;
    }
}

pdf_object_t* pdf_dict_get(pdf_object_t* d, const char* cle){
    if(d->type==dictionaires){
        if(d->object_direct.dictionaires==NULL){
            dictionaires_t * o=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
            o->cles=mymalloc(strlen(cle)+1);
            strcpy(o->cles,cle);
            o->valeur=NULL;
            o->valeur=pdf_null(o->valeur);
            o->next=NULL;
            d->object_direct.dictionaires=o;
            return o->valeur;	
        }else{
            dictionaires_t * cur =d->object_direct.dictionaires;
            dictionaires_t * prec=d->object_direct.dictionaires;
            while(cur!=NULL){
                if (strcmp(cur->cles,cle)==0){
                    return cur->valeur;
                }
                prec=cur;
                cur=cur->next;
            }
            dictionaires_t * o=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
            o->cles=mymalloc(strlen(cle)+1);
            strcpy(o->cles,cle);
            o->valeur=NULL;
            o->valeur=pdf_null(o->valeur);
            o->next=NULL;
            prec->next=o;
            return o->valeur;
        }
    }
    return NULL;
}

void pdf_fprint_object(FILE* fd, pdf_object_t* o){
    if(o==NULL){
        
    }else if(o->type==null){
        fprintf(fd, "%s",o->object_direct.null);
    }else if(o->type==boolean){
        if(o->object_direct.boolean==1){
            fprintf(fd, "%s","true");
        }else{
            fprintf(fd, "%s","false");
        }
    }else if(o->type==entier){
        fprintf(fd, "%d",o->object_direct.entier);
    }else if(o->type==reel){
        fprintf(fd, "%g",o->object_direct.reel);
    }else if(o->type==chaine){
        fprintf(fd, "(%s)",o->object_direct.chaine);
    }else if(o->type==nom){
        fprintf(fd, "/%s",o->object_direct.nom);
    }else if(o->type==stream){
        dictionaires_t * d =o->object_direct.stream.dic;
        size_t ta=d->valeur->object_direct.entier;
        fprintf(fd, "<<");
        fprintf(fd,"\n");
        while(d!=NULL){
            fprintf(fd, "  /%s ",d->cles);
            pdf_fprint_object(fd,d->valeur);
            fprintf(fd, "\n");
            d=d->next;
        }
        fprintf(fd, ">>");fprintf(fd, "\n");
        fprintf(fd, "stream\n");
        fwrite(o->object_direct.stream.data,sizeof(char),ta,fd);
        fprintf(fd,"\n");
        fprintf(fd, "endstream");
    }else if(o->type==tableaux){
        int i=0;
        int taille=o->object_direct.tableaux.taille;
        fprintf(fd, "[ ");
        for (i = 0; i < taille ; i++){
            pdf_fprint_object(fd,o->object_direct.tableaux.tab[i]);
            fprintf(fd, " ");
        }
        fprintf(fd, "]");
    }else if(o->type==dictionaires){
        dictionaires_t * d =o->object_direct.dictionaires;
        fprintf(fd, "<<");
        fprintf(fd,"\n");
        while(d!=NULL){
            fprintf(fd, "  /%s ",d->cles);
            pdf_fprint_object(fd,d->valeur);
            fprintf(fd, "\n");
            d=d->next;
        }
        fprintf(fd, ">>");
    }else if(o->type==reference){
        fprintf(fd,"%d ",o->object_direct.reference.id);
        fprintf(fd,"%d ",o->object_direct.reference.version);
        fprintf(fd,"%c",o->object_direct.reference.ref);
    }else{
        fprintf(stderr, "faillure pdf_fprint_object \n");
        exit(EXIT_FAILURE); 
    }	
}

pdf_object_t* pdf_default_catalog(pdf_object_t* c){
    c=pdf_dictionary(c);
    pdf_name(pdf_dict_get(c, "Type"), "Catalog");
    pdf_reference(pdf_dict_get(c, "Pages"),1);
    return c;
}
pdf_object_t* pdf_default_pages(pdf_object_t* p,unsigned int nb){
    p=pdf_dictionary(p);
    pdf_name(pdf_dict_get(p, "Type"), "Pages");
    pdf_int(pdf_dict_get(p, "Count"), nb);
    pdf_array(pdf_dict_get(p, "Kids"),nb);
    unsigned int j=4;
    for(unsigned int i=0;i<nb;i++){
        pdf_reference(pdf_array_get(pdf_dict_get(p, "Kids"),i),j);
        j++;
    }
    return p;
}

pdf_object_t* pdf_default_page(pdf_object_t* p,float w, float h){
    p=pdf_dictionary(p);
    pdf_name(pdf_dict_get(p, "Type"), "Page");
    pdf_reference(pdf_dict_get(p, "Parent"),1);
    pdf_array(pdf_dict_get(p, "MediaBox"),4);
    pdf_int(pdf_array_get(pdf_dict_get(p, "MediaBox"),0),0);
    pdf_int(pdf_array_get(pdf_dict_get(p, "MediaBox"),1),0);
    pdf_real(pdf_array_get(pdf_dict_get(p, "MediaBox"),2),w);
    pdf_real(pdf_array_get(pdf_dict_get(p, "MediaBox"),3),h);
    pdf_object_t* Resources;
    Resources=pdf_dictionary(pdf_dict_get(p, "Resources"));
    pdf_reference(pdf_dict_get(Resources,"Font"),2);
    pdf_reference(pdf_dict_get(Resources,"XObject"),3);
    pdf_null(pdf_dict_get(p, "Contents"));
    return p;
}
pdf_object_t* pdf_default_fonts(pdf_object_t* f) {
    f = pdf_dictionary(f);
    pdf_object_t* font;
    
    font = pdf_dictionary(pdf_dict_get(f, "FT"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Times-Roman");
    
    font = pdf_dictionary(pdf_dict_get(f, "FTB"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Times-Bold");
    
    font = pdf_dictionary(pdf_dict_get(f, "FTI"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Times-Italic");
    
    font = pdf_dictionary(pdf_dict_get(f, "FTBI"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Times-BoldItalic");
    
    font = pdf_dictionary(pdf_dict_get(f, "FV"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Verdana");
    
    font = pdf_dictionary(pdf_dict_get(f, "FVB"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Verdana-Bold");
    
    font = pdf_dictionary(pdf_dict_get(f, "FVI"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Verdana-Italic");
    
    font = pdf_dictionary(pdf_dict_get(f, "FVBI"));
    pdf_name(pdf_dict_get(font, "Type"), "Font");
    pdf_name(pdf_dict_get(font, "SubType"), "Type1");
    pdf_name(pdf_dict_get(font, "BaseFont"), "Verdana-BoldItalic");
    
    return f;
}

pdf_t* pdf_create(unsigned int nb, float w, float h){
    pdf_t * pdf=(pdf_t *)mymalloc(sizeof(pdf_t *));
    pdf->size=4+nb;
    pdf->PDF=mymalloc(pdf->size*(sizeof(pdf_object_t)));
    pdf->PDF[0]=NULL;
    pdf->PDF[0]=pdf_default_catalog(pdf->PDF[0]);
    pdf->PDF[1]=NULL;
    pdf->PDF[1]=pdf_default_pages(pdf->PDF[1],nb);
    pdf->PDF[2]=NULL;
    pdf->PDF[2]=pdf_default_fonts(pdf->PDF[2]);
    pdf->PDF[3]=NULL;
    pdf->PDF[3]=pdf_dictionary(pdf->PDF[3]);
    for(unsigned int i=4;i<nb+4;i++){
        pdf->PDF[i]=NULL;
        pdf->PDF[i]=pdf_default_page(pdf->PDF[i],w,h);
    }
    return pdf;
}

void pdf_delete(pdf_t* p){
    for(unsigned int i=0;i<p->size;i++){
        pdf_object_free(p->PDF[i]);
    }
    free(p);
}

size_t pdf_new_id(pdf_t* p){
    size_t taille=p->size;
    p->PDF=realloc(p->PDF,p->size*(sizeof(pdf_object_t)));
    if(p->PDF==NULL)fprintf(stderr,"faillure realloc");
    p->PDF[taille]=NULL;
    p->PDF[taille]=pdf_null(p->PDF[taille]);
    p->size +=1;
    return taille;
}

void pdf_save(const char* fname, pdf_t* p){
    FILE *fd=myfopen(fname,"w");
    int xref[p->size];
    char * c="%";
    fprintf(fd, "%s",c);
    fprintf(fd, "PDF-1.7\n");
    fprintf(fd, "%s",c);
    int h=0xE2E3CFD3;
    fwrite(&h,sizeof(char),4,fd);
    fprintf(fd, "\n");
    for(size_t i=0;i<p->size;i++){
        xref[i]=ftell(fd);
        fprintf(fd, "%ld 0 obj\n",i+1);
        pdf_fprint_object(fd,(pdf_object_t *)p->PDF[i]);
        fprintf(fd,"\n");
        fprintf(fd,"endobj\n");
        fprintf(fd,"\n");
    }
    int pos=ftell(fd);
    fprintf(fd,"xref\n");
    fprintf(fd, "0 %ld\n",p->size+1);
    fprintf(fd, "0000000000 65535 f\n");
    for(size_t i=0;i<p->size;i++){
        if(xref[i]<10){
            fprintf(fd, "000000000");
        }else if(xref[i]<100){
            fprintf(fd, "00000000");
        }else if(xref[i]<1000){
            fprintf(fd, "0000000");	
        }else if(xref[i]<10000){
            fprintf(fd, "000000");
        }else if(xref[i]<100000){
            fprintf(fd, "00000");	
        }else if(xref[i]<1000000){
            fprintf(fd, "0000");
        }else if(xref[i]<10000000){
            fprintf(fd, "000");
        } 	
        fprintf(fd, "%d 00000 n\n",xref[i]);
    }
    fprintf(fd, "trailer\n");
    fprintf(fd, "<<");
    fprintf(fd,"\n");
    fprintf(fd, "/Root 1 0 R");
    fprintf(fd, "\n");
    fprintf(fd, "/Size %ld\n",p->size+1);
    fprintf(fd, ">>");
    fprintf(fd, "startxref\n");
    fprintf(fd, "%d\n",pos);
    fprintf(fd, "%s",c);
    fprintf(fd, "%%EOF");
    
}
void pdf_set_content(pdf_t* p, unsigned int n, const char* instr){
    int j=pdf_new_id(p);
    p->PDF[j]=pdf_stream_from_string(p->PDF[j],instr);
    pdf_reference(pdf_dict_get(p->PDF[n],"Contents"),j);
}
bool jpeg_info(const void* i, size_t* width, size_t* height, size_t* bpc, const char** cs) {
    const unsigned char* im = (const unsigned char*)i;
    if (im[0]!=0xff || im[1]!=0xd8) return false;
    im += 2;
    while(im[0] == 0xff) {
        unsigned char frame_id = im[1];
        if (frame_id == 0xc0) break;
        size_t frame_length = 256 * im[2] + im[3];
        im += frame_length + 2;
    }
    size_t h = im[5] * 256 + im[6];
    size_t w = im[7] * 256 + im[8];
    size_t b = im[4];
    const char* c;
    switch(im[9]) {
        case 1: c = "DeviceGray"; break;
        case 3: c = "DeviceRGB"; break;
        case 4: c = "DeviceCMYK"; break;
        default:return false;
    }
    if (width) *width = w;
    if (height) *height = h;
    if (bpc) *bpc = b;
    if (cs) *cs = c;
    return true;
}

char* pdf_load_image(pdf_t* p, const char* fname){
    int j=pdf_new_id(p);
    p->PDF[j]=pdf_stream_from_file(p->PDF[j],fname);
    size_t  width =0;
    size_t  height=0;
    size_t  bpc=0;
    const char* cs=mymalloc(10);
    if(jpeg_info(p->PDF[j]->object_direct.stream.data,&width,&height,&bpc,&cs)){
        dictionaires_t * d=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        d->cles=mymalloc(strlen("Type")+1);
        strcpy(d->cles,"Type");
        d->valeur=NULL;
        d->valeur=pdf_name(d->valeur,"XObject");
        p->PDF[j]->object_direct.stream.dic->next=d;
        dictionaires_t * t=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        t->cles=mymalloc(strlen("Subtype")+1);
        strcpy(t->cles,"Subtype");
        t->valeur=NULL;
        t->valeur=pdf_name(t->valeur,"Image");
        d->next=t;
        dictionaires_t * w=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        w->cles=mymalloc(strlen("Width")+1);
        strcpy(w->cles,"Width");
        w->valeur=NULL;
        w->valeur=pdf_int(w->valeur,width);
        t->next=w;
        dictionaires_t * h=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        h->cles=mymalloc(strlen("Height")+1);
        strcpy(h->cles,"Height");
        h->valeur=NULL;
        h->valeur=pdf_int(h->valeur,height);
        w->next=h;
        dictionaires_t * b=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        b->cles=mymalloc(strlen("BitsPerComponent")+1);
        strcpy(b->cles,"BitsPerComponent");
        b->valeur=NULL;
        b->valeur=pdf_int(b->valeur,bpc);
        h->next=b;
        dictionaires_t * c=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        c->cles=mymalloc(strlen("ColorSpace")+1);
        strcpy(c->cles,"ColorSpace");
        c->valeur=NULL;
        c->valeur=pdf_name(c->valeur,cs);
        b->next=c;
        dictionaires_t * f=(dictionaires_t *)mymalloc(sizeof(dictionaires_t));
        f->cles=mymalloc(strlen("Filter")+1);
        strcpy(f->cles,"Filter");
        f->valeur=NULL;
        f->valeur=pdf_name(f->valeur,"DCTDecode");
        c->next=f;
        f->next=NULL;
        int pos_im=0;
        dictionaires_t * D=p->PDF[3]->object_direct.dictionaires;
        while(D!=NULL){
            pos_im++;
            D=D->next;
        }
        char ret [4];
        ret[0]='I';
        ret[1]='m';
        ret[2]=pos_im+48;
        ret[3]='\0';
        pdf_reference(pdf_dict_get(p->PDF[3],ret),j);
        char *r=mymalloc(strlen(ret));
        strcpy(r,ret);
        return r; 
    }else{
        fprintf(stderr, "faillure pdf_load_image \n");
        exit(EXIT_FAILURE); 
    }    
}
