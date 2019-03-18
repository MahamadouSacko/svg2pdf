#include "menu.h"
#include "args.h"
#include "pdf.h"
#include "svg.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


xelement_t * svg=NULL;
char* fname = NULL;
bool cont = true;

void ouvrir() {
    if(svg != NULL){
        printf("Un fichier svg est déjà chargé en mémoire!\nVeuillez saisir le nom de la sauvegarde:\n");
        char* sauvegarde = malloc(50*sizeof(char));
        scanf("%s", sauvegarde);
        save_xml(sauvegarde, svg);
        printf("Sauvegarde effectuée avec succés\n");
        free(sauvegarde);
        viderBuffer();
    }
    printf("Veuillez saisir le nom du fichier que vous souhaitez ouvrir:\n");
    fname = malloc(50*sizeof(char));	
    scanf("%s", fname);
    svg =  load_svg(fname);
    printf("%s est charge en mémoire avec succés\n",fname);
    viderBuffer();
}

void quit() {
    cont = false;
    if(svg != NULL){
        printf("Veuillez saisir le nom de la sauvegarde:\n");
        char* sauvegarde = malloc(50*sizeof(char));
        scanf("%s", sauvegarde);
        save_xml(sauvegarde, svg);
        printf("Sauvegarde effectuée avec succés\n");
        free(sauvegarde); 
        viderBuffer(); 
    }
}

void formats_SVG(){
    if(svg != NULL){
        int choix;
        printf("Voulez vous saisir le nom de la sauvegarde (saisissez 1 pour oui, 2 pour non)?\n");
        scanf("%d", &choix);
        if(choix == 1){
            printf("Veuillez saisir le nom de la sauvegarde: \n");
            char* sauvegarde = malloc(50*sizeof(char));
            scanf("%s", sauvegarde);
            save_xml(sauvegarde, svg);
            printf("Sauvegarde effectuée avec succés\n");
            free(sauvegarde);
            viderBuffer();
            free(svg);
            svg = NULL;
        }else if(choix == 2){
            save_xml(fname, svg);
            printf("Sauvegarde effectuée avec succés\n");
            free(svg);
            svg = NULL;
        }
    }else if(svg == NULL){
        printf("Action impossible! Veuillez charger un fichier .svg\n");
    }
    
}

void formats_PDF(){
    if(svg != NULL){
        int choix;
        printf("Voulez vous saisir le nom de la sauvegarde (saisissez 1 pour oui, 2 pour non)?\n");
        scanf("%d", &choix);
        viderBuffer();
        if(choix == 1){
            printf("Veuillez saisir le nom de la sauvegarde: \n");
            char* sauvegarde = malloc(50*sizeof(char));
            scanf("%s", sauvegarde);
            pdf_t * p=load_image_pdf(svg);
            pdf_save(sauvegarde,p);
            printf("Sauvegarde effectuée avec succés\n");
            free(sauvegarde);
            free(svg);
            svg = NULL;
            viderBuffer();
        }else if(choix == 2){
            pdf_t * p=load_image_pdf(svg);
            pdf_save(fname,p);
            printf("Sauvegarde effectuée avec succés\n");
            free(svg);
            svg = NULL;
        }
        
    }else if(svg == NULL){
        printf("Action impossible! Veuillez charger un fichier .svg\n");
        //viderBuffer();
    }
    
}

void info(){
    if(svg != NULL){
        image_t im=load_image_ascii(svg);
        printf("\n");
        printf("nom du fichier SVG d'origine: %s\n", fname);
        if(im.title!=NULL)fprintf(stdout,"%s\n",im.title);
        if(im.desc!=NULL)fprintf(stdout,"%s\n",im.desc);
        printf("nombre de figures :%d\n",im.nb_figure);
        printf("taille de l'image :(%d,%d)\n",im.largeur,im.hauteur );  
    }else if(svg == NULL){
        printf("Action impossible! Veuillez charger un fichier .svg\n");
    }
    
}

void ascii_art(){
    if(svg!=NULL){
        image_t im=load_image_ascii(svg);
        affiche_asccii(stdout,im);
    }else if(svg == NULL){
        printf("Action impossible! Veuillez charger un fichier .svg\n");
    }
}
void ajouter_cercle(){
    if(svg!=NULL){
        printf("Entre une valeur pour le rayon du cercle : ");
        char* r = malloc(50*sizeof(char));
        scanf("%s", r);
        printf("Donner la couleur du cercle : ");
        char* color = malloc(50*sizeof(char));
        scanf("%s", color);
        viderBuffer();
        xelement_t * t=circl(r,color);
        add_sub_xelement(svg,t);
        printf("ajoute cercle effectuée avec succés\n");
    }else{
        printf("Action impossible! Veuillez charger un fichier .svg\n");
        
    }
    
}
void ajouter_rectangle(){
    if(svg!=NULL){
        printf("Entre une valeur pour la largeur du rectangle : ");
        char* largeur = malloc(50*sizeof(char));
        scanf("%s", largeur);
        printf("Entre une valeur pour la hauteur du rectangle : ");
        char* hauteur = malloc(50*sizeof(char));
        scanf("%s", hauteur);
        printf("Donner la couleur du rectangle : ");
        char* color = malloc(50*sizeof(char));
        scanf("%s", color);
        viderBuffer();
        xelement_t * t=rect(largeur,hauteur,color);
        add_sub_xelement(svg,t);
        printf("ajoute du rectangle a etait effectuée avec succés\n");
    }else{
        printf("Action impossible! Veuillez charger un fichier .svg\n");
        
    }
}
void ajouter_ligne(){
    if(svg!=NULL){
        printf("Entre une valeur pour la longueur de la ligne : ");
        char* longueur = malloc(50*sizeof(char));
        scanf("%s", longueur);
        printf("Donner la couleur de la ligne : ");
        char* color = malloc(50*sizeof(char));
        scanf("%s", color);
        viderBuffer();
        xelement_t * t=ligne(longueur,color);
        add_sub_xelement(svg,t);
        printf("ajoute ligne a etait effectuée avec succés\n");
    }else{
        printf("Action impossible! Veuillez charger un fichier .svg\n");
        
    }
}
void ajouter_texte(){
 
}
void ajouter_image(){
    
}
void supprimer(){
    
}
void affiche_xml(){
    if(svg!=NULL){
        print_xelement(svg);
    }else if(svg == NULL){
        printf("Action impossible! Veuillez charger un fichier .svg\n");
    }	
}
int main(){
    menu_t* m;
    menu_t* sm;
    menu_t* ssm;
    
    m = createMenu("Menu Principal");
    
    sm = createMenu("Fichier");
    addSubMenu(m,sm);
    addMenuAction(sm,"Ouvrir",ouvrir);
    ssm = createMenu("Enregistrer");
    addSubMenu(sm,ssm);
    addMenuAction(ssm,"format SVG",formats_SVG);
    addMenuAction(ssm,"format PDF",formats_PDF);
    
    addMenuAction(sm,"Informations",info);
    ssm = createMenu("Affichage");
    addSubMenu(sm,ssm);
    addMenuAction(ssm," ASCII_art",ascii_art);
    addMenuAction(ssm," XML",affiche_xml);
    addMenuAction(sm,"Quitter",quit);
    
    sm = createMenu("Figure de base");
    addSubMenu(m,sm);
    addMenuAction(sm,"Ajouter cercle",ajouter_cercle);
    addMenuAction(sm,"Ajouter rectangle",ajouter_rectangle);
    addMenuAction(sm,"Ajouter ligne",ajouter_ligne);
    addMenuAction(sm,"Ajouter texte",ajouter_texte);
    addMenuAction(sm,"Ajouter image",ajouter_image);
    addMenuAction(sm,"Supprimer",supprimer);
    sm = createMenu("Transformation et groupe");
    addSubMenu(m,sm);
    
    while(cont) launchMenu(m);
    
    deleteMenu(m);
    return EXIT_SUCCESS;
}