#define _GNU_SOURCE
#include "svg.h"
#include "pdf.h"
#include "xml.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<math.h>

void print_coord(coord_t c){
    printf("(%f,%f)\n",c.x,c.y);
}
coord_t coordinate(double x, double y) {
    coord_t c ;
    c.x=x;
    c.y=y;
    return c;
}

coord_t inv_translate_point(coord_t p, coord_t v){
    coord_t P;
    P.x=p.x-v.x;
    P.y=p.y-v.y;
    return P;
}
coord_t translate_point(coord_t p, coord_t v){
    coord_t P;
    P.x=p.x+v.x;
    P.y=p.y+v.y;
    return P;
}

coord_t inv_rotate_point(coord_t p, double t){
    coord_t P;
    P.x=p.x*(cos(-t))-p.y*(sin(-t));
    P.y=p.x*(sin(-t))+p.y*(cos(-t));
    return P;
}
figure_t * circle(double cx,double cy,double radius,char * fill){
    figure_t * f=mymalloc(sizeof(figure_t));
    f->color='0';
    f->centre.x=cx;
    f->centre.y=cy;
    f->angle_rotation=0;
    f->figure=Cercle;
    f->carat_figure.circle.rayon_carre=radius*radius;
    f->fill=fill;
    return f;
}

figure_t * rectangle(double x,double y,double width, double height,char *fill){
    figure_t * f=mymalloc(sizeof(figure_t));
    f->color='+';
    f->centre.x=x;
    f->centre.y=y;
    f->angle_rotation=0;
    f->figure=Rectangle;
    f->carat_figure.rectangle.demie_largeur=width/2;
    f->carat_figure.rectangle.demie_hauteur=height/2;
    f->fill=fill;
    return f;
}

figure_t * line(double x1,double y1,double x2,double y2, char * fill,double stroke_width){
    figure_t * f=mymalloc(sizeof(figure_t));
    double length=sqrt(((y2-y1)*(y2-y1))+((x2-x1)*(x2-x1)));
    f->color='*';
    f->centre.x=x1;
    f->centre.y=y1;
    f->angle_rotation=0;
    f->figure=Lignes;
    f->carat_figure.line.demie_longueur=length/2;
    f->fill=fill;
    f->carat_figure.line.grain=stroke_width;
    return f;    
}

figure_t  * color(figure_t * f, color_t c){
    f->color=c;
    return f;
}

figure_t * translate(figure_t * f, double dx, double dy){
    coord_t p=coordinate(dx,dy);
    f->centre=translate_point(f->centre,p);
    return f;
    
}

figure_t * rotate(figure_t * f, double dt){
    f->angle_rotation= f->angle_rotation+dt;
    return f;
    
}
figure_t * scale(figure_t * f, double s){
    s=1;
    f->centre.x=f->centre.x*s;
    f->centre.y=f->centre.y*s;
    return f;
}

color_t intersect(coord_t p, figure_t * f, double grain){
    p=inv_translate_point(p,f->centre);
    p=inv_rotate_point(p,f->angle_rotation);
    if(f->figure==Cercle){
        if(((p.x*p.x)+(p.y*p.y))<=f->carat_figure.circle.rayon_carre){
            return f->color;
        }
    }else if(f->figure==Rectangle){
        if(fabs(p.x)<=f->carat_figure.rectangle.demie_largeur&&fabs(p.y)<=f->carat_figure.rectangle.demie_hauteur){
            return f->color;
        }     
    }else if(f->figure==Lignes){
        if(fabs(p.x)<=f->carat_figure.line.demie_longueur&&fabs(p.y)<=grain){
            return f->color;
        }
        
    }else{
        printf(" ");
        
    }
    return 0;
}

image_t image(unsigned int width, unsigned int height, double grain){
    image_t img;
    img.largeur=width;
    img.hauteur=height;
    img.grain=grain;
    img.title=NULL;
    img.desc=NULL;
    img.figure=NULL;
    img.nb_figure=0;
    return img;
}

image_t append(image_t img,figure_t * f){
    if(img.figure==NULL){
        img.figure=mymalloc(sizeof(figure_t));
        img.figure=f;
        img.figure->next=NULL;
        
    }else{
        figure_t * temp=img.figure;
        while(temp->next!=NULL){
            temp=temp->next;
        }
        temp->next=f;
    }
    img.nb_figure++;
    return img;
}

void paint(FILE *fd,image_t img){
    unsigned int i=0;
    unsigned int j=0;
    for(i=0;i<img.hauteur;i++){
        for(j=0;j<img.largeur;j++){
            color_t px=0;
            figure_t * temp=img.figure;
            while(temp!=NULL){
                color_t c;
                c = intersect(coordinate(i*img.grain,j*0.7*img.grain),temp,img.grain);
                if (c){
                    px=c;
                    break;
                }
                temp=temp->next;
            }
            if(px)
                fprintf(fd,"%c", px);
            else
                fprintf(fd," ");
        }
        fprintf(fd,"\n");
        
    }
}


xelement_t* load_svg(const char* fname){
    xelement_t* svg=load_xml(fname);
    if (strcmp(svg->nom_element,"svg")==0){
        return svg;
    }else{
        fprintf(stderr, "c'est pas un fichier svg");
        exit(EXIT_FAILURE);
    }
    
}

char * get_title(xelement_t *title){
    char *ch=NULL;
    if(title->contenu.etiquete==raw){
        ch=mymalloc(strlen(title->contenu.type.raw)+1);
        strcpy(ch,title->contenu.type.raw);
    }
    return ch;
    
}

char * get_desc(xelement_t *desc){
    char * ch=NULL;
    if(desc->contenu.etiquete==raw){
        ch=mymalloc(strlen(desc->contenu.type.raw)+1);
        strcpy(ch,desc->contenu.type.raw);
    }
    return ch;
}

figure_t * transform(figure_t * f,char * transform){
    unsigned int i=0;
    while(transform[i]!='\0'){
        if(transform[i]=='t'){
            while(transform[i]!='('){
                i++;
            }
            i++;
            char x [5];
            unsigned int j=0;
            while(transform[i]!=' '){
                x[j]=transform[i];
                i++;
                j++;
            }
            x[j]='\0';
            double dx=atof(x);
            i++;
            char y [5];
            j=0;
            while(transform[i]!=')'){
                y[j]=transform[i];
                i++;
                j++;
            }
            y[j]='\0';
            double dy=atof(y);
            if(dy>=21)dy=dy/10;
            if(dx>=21)dx=dx/10;
            f=translate(f,dx,dy);  
        }else if(transform[i]=='r'){
            while(transform[i]!='('){
                i++;
            }
            i++;
            char t [5];
            unsigned int j=0;
            while(transform[i]!=')'){
                t[j]=transform[i];
                i++;
                j++;
            }
            t[j]='\0';
            double th=atof(t);
            th=th * M_PI / 180;
            f=rotate(f,th);
        }else if(transform[i]=='s'){
            while(transform[i]!='('){
                i++;
            }
            i++;
            char t [5];
            unsigned int j=0;
            while(transform[i]!=')'){
                t[j]=transform[i];
                i++;
                j++;
            }
            t[j]='\0';
            double s=atof(t);
            f=scale(f,s);
        }
        i++;
    }
    return f;
} 

figure_t * get_rect(xelement_t * rect){
    figure_t * f;
    char * form=NULL;
    double x=0;
    double y=0;
    char * fill=NULL;
    double width=0;
    double height=0;
    liste_attribute l=rect->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;  
        }else if(strcmp(l->nom_attribut,"x")==0){
            x=atof(l->valeur_attribut);
            if(abs(x)>=21)x=x/10;
            // if(x<0)x=0;
        }else if(strcmp(l->nom_attribut,"y")==0){
            y=atof(l->valeur_attribut);
            if(abs(y)>=21)y=y/10;
            //if(y<0)y=0;
        }else if(strcmp(l->nom_attribut,"fill")==0){
            fill=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"width")==0){
            width=atof(l->valeur_attribut);
            if(width>=21)width=width/10;
        }else if(strcmp(l->nom_attribut,"height")==0){
            height=atof(l->valeur_attribut);
            if(height>=21)height=height/10;
        }
        l=l->next;
    }
    f = rectangle(x,y,width,height,fill);
    if(form!=NULL)f=transform(f,form);
    return f;
}

figure_t *  get_circle(xelement_t * Circle){
    figure_t * f;
    char * form=NULL;
    double cx=0;
    double cy=0;
    double r=0;
    char * fill=NULL;
    liste_attribute l=Circle->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"cx")==0){
            cx=atof(l->valeur_attribut);
            if(cx>=21)cx=cx/10;
        }else if(strcmp(l->nom_attribut,"cy")==0){
            cy=atof(l->valeur_attribut);
            if(cy>=21)cy=cy/10;
        }else if(strcmp(l->nom_attribut,"r")==0){
            r=atof(l->valeur_attribut);
            if(r>=21)r=r/10;
        }else if(strcmp(l->nom_attribut,"fill")==0){
            fill=l->valeur_attribut;     
        }
        l=l->next;
    }
    f = circle(cx,cy,r,fill);
    if(form!=NULL)f=transform(f,form);
    return f;
}

figure_t * get_line(xelement_t * Line){
    figure_t * f;
    char * form=NULL;
    double x1=0;
    double y1=0;
    double x2=0;
    double y2=0;
    char * fill=NULL;
    double stroke_width=0;
    liste_attribute l=Line->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"x1")==0){
            x1=atof(l->valeur_attribut);
            if(abs(x1)>=21)x1=x1/10;
        }else if(strcmp(l->nom_attribut,"y1")==0){
            y1=atof(l->valeur_attribut);
            if(abs(y1)>=21)y1=y1/10;
        }else if(strcmp(l->nom_attribut,"x2")==0){
            x2=atof(l->valeur_attribut);
            if(abs(x2)>=21)x2=x2/10;
        }else if(strcmp(l->nom_attribut,"y2")==0){
            y2=atof(l->valeur_attribut);
            if(abs(y2)>=21)y2=y2/10;
        }else if(strcmp(l->nom_attribut,"stroke")==0){
            fill=l->valeur_attribut; 
        }else if(strcmp(l->nom_attribut,"stroke-width")==0){
            stroke_width=atof(l->valeur_attribut);
            if(stroke_width>=10)stroke_width=stroke_width/10;
        }
        l=l->next;
    }
    f =line(x1,y1,x2,y2,fill,stroke_width);
    if(form!=NULL)f=transform(f,form);
    return f;
}


image_t get_groupe(xelement_t * g,image_t img){
    liste_attribute l=g->AT;
    char * form=NULL;
    if(l!=NULL)form=l->valeur_attribut;
    xelement_t * temp=g->contenu.type.fils;
    while(temp!=NULL){
        if(strcmp(temp->nom_element,"rect")==0){
            figure_t *  f=get_rect(temp);
            if(form!=NULL)f=transform(f,form);
            img=append(img,f);
        }else if(strcmp(temp->nom_element,"circle")==0){
            figure_t*  f=get_circle(temp);
            if(form!=NULL)f=transform(f,form);
            img=append(img,f);           
        }else if(strcmp(temp->nom_element,"line")==0){
            figure_t* f=get_line(temp);
            if(form!=NULL)f=transform(f,form);
            img=append(img,f);
        }else if(strcmp(temp->nom_element,"g")==0){
            img=get_groupe(temp,img);
        }
        temp=temp->frere;
    }
    return img;
    
}
image_t load_image_ascii(xelement_t * svg){
    image_t img ;//= image(130,52,0.75);
    liste_attribute l=svg->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"xmlns")==0){
        }else if(strcmp(l->nom_attribut,"width")==0){
            img.largeur=atof(l->valeur_attribut);
            if(img.largeur>=100)img.largeur=img.largeur/10;
        }else if(strcmp(l->nom_attribut,"height")==0){
            img.hauteur=atof(l->valeur_attribut);
            if(img.hauteur>=100)img.hauteur=img.hauteur/10;
        }else if(strcmp(l->nom_attribut,"viewBox")==0){
        }
        l=l->next;
    }
    img = image(img.largeur,img.hauteur,0.75);
    xelement_t * temp=svg->contenu.type.fils;
    while(temp!=NULL){
        if(strcmp(temp->nom_element,"title")==0){
            img.title=get_title(temp);
        }else if(strcmp(temp->nom_element,"desc")==0){
            img.desc=get_desc(temp);
        }else if(strcmp(temp->nom_element,"rect")==0){
            figure_t * f=get_rect(temp);
            img=append(img,f);
        }else if(strcmp(temp->nom_element,"circle")==0){
            figure_t * f=get_circle(temp);
            img=append(img,f);
        }else if(strcmp(temp->nom_element,"line")==0){
            figure_t * f=get_line(temp);
            img=append(img,f);
        }else if(strcmp(temp->nom_element,"g")==0){
            img=get_groupe(temp,img);
        }
        temp=temp->frere;
    }
    return img;
    
}


void affiche_asccii(FILE *fd,image_t img){
    if(img.title!=NULL)fprintf(fd,"%s\n",img.title);
    if(img.desc!=NULL)fprintf(fd,"%s\n",img.desc);
    paint(fd,img);   
}

char * transform_pdf(char * transform){
    char *dx=NULL;
    char *dy=NULL;
    double th=0;
    char *s=NULL;
    char **tab=mymalloc(sizeof(char *)*1);
    unsigned int i=0;
    unsigned int taille=0;
    unsigned int verif=1;
    unsigned int taille_chaine=0;
    while(transform[i]!='\0'){
        if(transform[i]=='t'){
            char * chaine=NULL;
            while(transform[i]!='('){
                i++;
            }
            i++;
            char x [5];
            unsigned int j=0;
            while(transform[i]!=' '){
                x[j]=transform[i];
                i++;
                j++;
            }
            x[j]='\0';
            dx=x;
            i++;
            char y [5];
            j=0;
            while(transform[i]!=')'){
                y[j]=transform[i];
                i++;
                j++;
            }
            y[j]='\0';
            dy=y;
            if(taille>=verif){
                tab=realloc(tab,sizeof(char)*verif+1);
                verif++;
            }
            asprintf(&chaine," 1 0 0 1 %s %s cm",dx,dy);
            tab[taille]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[taille], chaine);
            taille++;
        }else if(transform[i]=='r'){
            char * chaine=NULL;
            while(transform[i]!='('){
                i++;
            }
            i++;
            char t [5];
            unsigned int j=0;
            while(transform[i]!=')'){
                t[j]=transform[i];
                i++;
                j++;
            }
            t[j]='\0';
            th=atof(t);
            if(taille>=verif){
                tab=realloc(tab,sizeof(char)*verif+1);
                verif++;
            }
            double x=cos(th);
            double y=sin(th);
            asprintf(&chaine," %.3g %.3g %.3g %.3g 0 0 cm",x,y,-y,x);
            tab[taille]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[taille], chaine);
            taille++;
        }else if(transform[i]=='s'){
            char * chaine=NULL;
            while(transform[i]!='('){
                i++;
            }
            i++;
            char t [5];
            unsigned int j=0;
            while(transform[i]!=')'){
                t[j]=transform[i];
                i++;
                j++;
            }
            t[j]='\0';
            s=t;
            if(taille>=verif){
                tab=realloc(tab,sizeof(char)*verif+1);
                verif++;
            }
            asprintf(&chaine," %s 0 0 %s 0 0 cm",s,s); 
            tab[taille]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[taille], chaine);
            taille++;
        }
        i++;
    }
    char * chaine_ret=mymalloc(sizeof(char)*(taille_chaine));
    unsigned int cpt=0;
    while(cpt<taille){
        chaine_ret=strcat(chaine_ret,tab[cpt]);
        cpt++;
        
    }
    return chaine_ret;
    
}

char *get_line_pdf(xelement_t * line){
    char * form=NULL;
    char * x1=0;
    char * y1=0;
    char * x2=0;
    char * y2=0;
    //char * fill=NULL;
    char * stroke_width=0;
    liste_attribute l=line->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"x1")==0){
            x1=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"y1")==0){
            y1=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"x2")==0){
            x2=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"y2")==0){
            y2=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"stroke")==0){
            // fill=l->valeur_attribut; 
        }else if(strcmp(l->nom_attribut,"stroke-width")==0){
            stroke_width=l->valeur_attribut;
        }
        l=l->next;
    }
    
    char* cnt = NULL; 
    if(form!=NULL){
        char * chaine=transform_pdf(form);
        asprintf(&cnt," q %s w 1 0 0 RG%s %s %s m %s %s l S Q",stroke_width,chaine,x1,y1,x2,y2);
    }else{
        asprintf(&cnt," q %s w 1 0 0 RG%s %s m %s %s l S Q",stroke_width,x1,y1,x2,y2);
        
    }
    return cnt;
}

char *get_rect_pdf(xelement_t * line){
    char * form=NULL;
    char * x=0;
    char * y=0;
    char * width=NULL;
    char * height=NULL;
    //char * fill=NULL;
    liste_attribute l=line->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"x")==0){
            x=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"y")==0){
            y=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"width")==0){
            width=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"height")==0){
            height=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"fill")==0){
            // fill=l->valeur_attribut; 
        }
        l=l->next;
    }
    
    char* cnt = NULL; 
    if(form!=NULL){
        char * chaine=transform_pdf(form);
        asprintf(&cnt," q 0 0.5 0 rg%s %s %s %s %s re F Q",chaine,x,y,width,height);
    }else{
        asprintf(&cnt," q 0 0.5 0 rg%s %s %s %s re F Q",x,y,width,height);
    }
    return cnt;
}

char *get_circle_pdf(xelement_t * circle){
    char * form=NULL;
    char * cx=NULL;
    char * cy=NULL;
    char * r=NULL;
    //char * fill=NULL;
    liste_attribute l=circle->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"cx")==0){
            cx=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"cy")==0){
            cy=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"fill")==0){
            // fill=l->valeur_attribut; 
        }else if(strcmp(l->nom_attribut,"r")==0){
            r=l->valeur_attribut;
        }
        l=l->next;
    }
    
    char* cnt = NULL; 
    if(form!=NULL){
        char * chaine=transform_pdf(form);
        asprintf(&cnt," q 0 0 1 rg%s %s %s %s %s %s %s cm -1 0 m -1 0.553 -0.553 1 0 1 c 0.553 1 1 0.553 1 0 c 1 -0.553 0.553 -1 0 -1 c -0.553 -1 -1 -0.553 -1 0 c F Q",chaine,r,cx,cy,r,cx,cy);
    }else{
        asprintf(&cnt," q 0 0 1 rg %s %s %s %s %s %s cm -1 0 m -1 0.553 -0.553 1 0 1 c 0.553 1 1 0.553 1 0 c 1 -0.553 0.553 -1 0 -1 c -0.553 -1 -1 -0.553 -1 0 c F Q",r,cx,cy,r,cx,cy);
        
    }
    return cnt;
}

char * get_image(pdf_t * p,xelement_t * m){
    char * form=NULL;
    char * x=NULL;
    char * y=NULL;
    char * width=NULL;
    char * height=NULL;
    char * xlink=NULL;
    char * preserveAspectRatio=NULL;
    liste_attribute l=m->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"x")==0){
            x=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"y")==0){
            y=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"width")==0){
            width=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"height")==0){
            height=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"href")==0){
            xlink=l->valeur_attribut; 
        }else if(strcmp(l->nom_attribut,"preserveAspectRatio")==0){
            preserveAspectRatio=l->valeur_attribut;
        }
        l=l->next;
    }
    const char* im = pdf_load_image(p,xlink);
    char* cnt = NULL; 
    if(strcmp(preserveAspectRatio,"none")!=0){
        if(form!=NULL){
            char * chaine=transform_pdf(form);
            asprintf(&cnt," q%s /%s Do Q",chaine,im);
        }else{
            asprintf(&cnt," q /%s Do Q",im); 
        }
    }else{   
        if(form!=NULL){
            char * chaine=transform_pdf(form);
            asprintf(&cnt," q%s %s %s %s %s %s %s cm /%s Do Q",chaine,width,x,y,height,x,y,im);
        }else{
            asprintf(&cnt," q %s %s %s %s %s %s cm /%s Do Q",width,x,y,height,x,y,im);
        }
    }
    return cnt;
}

char * get_text(xelement_t * text ){
    char * form=NULL;
    //char * x=0;
    //char * y=0;
    //char * font_family=NULL;
    //char * font_size=NULL;
    //char * fill=NULL;
    liste_attribute l=text->AT;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"transform")==0){
            form=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"x")==0){
            // x=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"y")==0){
            //y=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"font-family")==0){
            //font_family=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"font-size")==0){
            //font_size=l->valeur_attribut;
        }else if(strcmp(l->nom_attribut,"fill")==0){
            // fill=l->valeur_attribut; 
        }
        l=l->next;
    }
    
    char* cnt = NULL; 
    if(form!=NULL){
        char * chaine=transform_pdf(form);
        asprintf(&cnt," q%s 1 0 1 rg BT /FV 20. Tf 1 Tc 2 Tw (%s) Tj ET Q ",chaine,text->contenu.type.raw);
    }else{
        asprintf(&cnt," q 1 0 1 rg BT /FV 20. Tf 1 Tc 2 Tw (%s) Tj ET Q ",text->contenu.type.raw);
    }
    return cnt;
}

char * get_group(pdf_t * p,xelement_t * g){
    unsigned int taille=0;
    unsigned int taille_chaine=0;
    unsigned int cpt=0;
    char * form=NULL;
    liste_attribute l=g->AT;
    if(l!=NULL)form=l->valeur_attribut;
    xelement_t * te=g->contenu.type.fils;
    while(te!=NULL){
        taille++;
        te=te->frere;
    }
    char **tab=mymalloc(sizeof(char *)*taille);
    xelement_t * temp=g->contenu.type.fils;
    while(temp!=NULL){
        if(strcmp(temp->nom_element,"rect")==0){
            char * chaine=get_rect_pdf(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"circle")==0){
            char * chaine=get_circle_pdf(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"line")==0){
            char * chaine=get_line_pdf(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"text")==0){
            char * chaine=get_text(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"image")==0){
            char * chaine=get_image(p,temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"g")==0){
            char * chaine=get_group(p,temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }
        temp=temp->frere;
    }
    char * chaine_ret=mymalloc(sizeof(char)*(taille_chaine));
    cpt=0;
    while(cpt<taille){
        chaine_ret=strcat(chaine_ret,tab[cpt]);
        cpt++;
        
    }
    char* cnt = NULL;
    char * ch=transform_pdf(form);
    asprintf(&cnt," q%s %s Q",ch,chaine_ret);
    return cnt;
}

pdf_t * load_image_pdf(xelement_t * svg){
    liste_attribute l=svg->AT;
    double width=0;
    double height=0;
    while(l!=NULL){
        if(strcmp(l->nom_attribut,"xmlns")==0){
        }else if(strcmp(l->nom_attribut,"width")==0){
            width=atof(l->valeur_attribut);
        }else if(strcmp(l->nom_attribut,"height")==0){
            height=atof(l->valeur_attribut);
        }else if(strcmp(l->nom_attribut,"viewBox")==0){
            
        }
        l=l->next;
    }
    pdf_t* p = pdf_create(1, width, height);
    unsigned int taille=0;
    unsigned int taille_chaine=0;
    unsigned int cpt=0;
    xelement_t * te=svg->contenu.type.fils;
    while(te!=NULL){
        taille++;
        te=te->frere;
    }
    char **tab=mymalloc(sizeof(char *)*taille);
    xelement_t * temp=svg->contenu.type.fils;
    while(temp!=NULL){
        if(strcmp(temp->nom_element,"rect")==0){
            char * chaine=get_rect_pdf(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"circle")==0){
            char * chaine=get_circle_pdf(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"line")==0){
            char * chaine=get_line_pdf(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"text")==0){
            char * chaine=get_text(temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"image")==0){
            char * chaine=get_image(p,temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }else if(strcmp(temp->nom_element,"g")==0){
            char * chaine=get_group(p,temp);
            tab[cpt]=mymalloc(sizeof(char)*(1+strlen(chaine)));
            taille_chaine=taille_chaine+strlen(chaine);
            strcpy(tab[cpt], chaine);
            cpt++;
        }
        temp=temp->frere;
    }
    char * chaine_ret=mymalloc(sizeof(char)*(taille_chaine));
    cpt=0;
    while(cpt<taille){
        chaine_ret=strcat(chaine_ret,tab[cpt]);
        cpt++;
        
    }
    char* cnt = NULL;
    asprintf(&cnt,"q 1 0 0 -1 0 520 cm%s Q",chaine_ret);
    pdf_set_content(p,4,cnt);
    return p;
}

xelement_t * circl(char *r, char * fill){
    xelement_t* e = create_xelement("circle");
    add_xattribute(e,"cx","0");
    add_xattribute(e,"cy","0");
    add_xattribute(e,"r",r);
    add_xattribute(e,"fill",fill);
    return e;
}
xelement_t * rect(char * width, char * height,char *fill){
    xelement_t* e = create_xelement("rect");
    add_xattribute(e,"x","0");
    add_xattribute(e,"y","0");
    add_xattribute(e,"width",width);
    add_xattribute(e,"height",height);
    add_xattribute(e,"fill",fill);
    return e;
}

xelement_t * ligne(char * length,char *fill){
    xelement_t* e = create_xelement("line");
    add_xattribute(e,"x1",length);
    add_xattribute(e,"y1","0");
    add_xattribute(e,"x2",length);
    add_xattribute(e,"y2","0");
    add_xattribute(e,"stroke-width","20");
    add_xattribute(e,"stroke",fill);
    return e;
}



