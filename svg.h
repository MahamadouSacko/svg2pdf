#ifndef SVG_H
#define SVG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<math.h>
#include "xml.h"
#include "pdf.h"

typedef struct{
    double x;
    double y;
}coord_t;
void print_coord(coord_t c);
coord_t coordinate(double x, double y);
coord_t inv_translate_point(coord_t p, coord_t v);
coord_t translate_point(coord_t p, coord_t v);
coord_t inv_rotate_point(coord_t p, double t);

enum figure_e{
    Cercle,
    Rectangle,
    Lignes
};

struct circle_s{
    double rayon_carre;
};
struct rectangle_s{
    double demie_largeur;
    double demie_hauteur;
};
struct line_s{
    double demie_longueur;
    double grain;
};
typedef char color_t; 

typedef union{
    struct line_s line;
    struct rectangle_s rectangle;
    struct circle_s circle;
}carat_t;

typedef struct figure_s {
    color_t color;
    coord_t centre;
    double angle_rotation;
    enum figure_e figure;
    carat_t carat_figure;
    char *fill;
    char *form;
    struct figure_s * next;
}figure_t;

figure_t * circle(double x,double y,double radius,char * fill);
figure_t * rectangle(double x,double y, double width, double height,char *fill);
figure_t * line(double x1,double y1,double x2,double y2, char * fill,double stroke_width);
figure_t * color(figure_t * f, color_t c);
figure_t * translate(figure_t * f, double dx, double dy);
figure_t * rotate(figure_t * f, double dt);
figure_t * scale(figure_t * f, double s);
color_t   intersect(coord_t p, figure_t * f, double grain);

typedef struct {
    char * title;
    char * desc;
    unsigned  int largeur; //width
    unsigned int hauteur;//height
    double grain;
    figure_t * figure;
    unsigned int nb_figure;
    
}image_t;

image_t image(unsigned int width, unsigned int height, double grain);
image_t append(image_t img,figure_t * f);
void paint(FILE *fd,image_t img);
xelement_t* load_svg(const char* fname);
char * get_title(xelement_t *title);
char * get_desc(xelement_t *desc);
figure_t * transform(figure_t * f,char * transform);
figure_t * get_rect(xelement_t * rect);
figure_t * get_circle(xelement_t * Circle);
figure_t * get_line(xelement_t * Line);
image_t load_image_ascii(xelement_t * svg);
void affiche_asccii(FILE *fd,image_t img);
char * transform_pdf(char * transform);
char *get_line_pdf(xelement_t * line);
char *get_rect_pdf(xelement_t * line);
char *get_circle_pdf(xelement_t * circle);
char * get_image(pdf_t * p,xelement_t * m);
char * get_text(xelement_t * text );
char * get_group(pdf_t * p,xelement_t * g);
pdf_t * load_image_pdf(xelement_t * svg);
xelement_t * circl(char *r, char * fill);
xelement_t * rect(char * width, char * height,char *fill);
xelement_t * ligne(char * length,char *fill);
#endif //SVG_H
