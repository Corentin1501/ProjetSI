// gcc opmat.c actions.c VM_init.c -c
// gcc main.c actions.o axes.o init.o lumiere.o switch_blend.o switch_light.o VM_init.o opmat.o -lm -lGL -lGLU -lglut -o executable

// COMPILER

// gcc opmat.c actions.c VM_init.c LEGO_personnage.c LEGO_brique.c LEGO_decor.c ppm.c -c; gcc main.c actions.o axes.o init.o lumiere.o switch_blend.o switch_light.o VM_init.o opmat.o LEGO_personnage.o LEGO_brique.o LEGO_decor.o ppm.o -lm -lGL -lGLU -lglut -o executable

#include "init.h"
#include "axes.h"
#include "VM_init.h"
#include "opmat.h"
#include "LEGO_personnage.h"
#include "LEGO_brique.h"
#include "LEGO_decor.h"
#include "stdbool.h"
#include "ppm.h"



float angle_rotation = 0;   // rotation personnage deplacement
float coordonnee_x   = 0;   // deplacement en x du personnage
float coordonnee_z   = 0;   // deplacement en z du personnage

float angle_deplacement = 0;            // angle créé (pour les bras et les jambes)
float angle_coureur_deplacement = 0;    // celui du coureur

float rotation_coureur_y    = 0;
float coordonnee_coureur_x  = 0;
float coordonnee_coureur_z  = 0;

static const int SCENE_SIZE = 35;   // taille de la scène


GLvoid Modelisation()
{
    VM_init();

    glPushMatrix();
    {
        glTranslatef(-((SCENE_SIZE/2)-4), 0, ((SCENE_SIZE/2)-4));
        glTranslatef(coordonnee_coureur_x, 0, coordonnee_coureur_z);    
        glRotatef(rotation_coureur_y, 0, 1, 0);             // translation et rotation causée par le déplacement du personnage
        creer_personnage(angle_coureur_deplacement);
    }
    glPopMatrix();

    glPushMatrix();
    {

        creer_sol(SCENE_SIZE);
        creer_maison(SCENE_SIZE);

        glTranslatef(10,0,10);
        creer_bucheron();
        glTranslatef(0,0,4);

        creer_buche();
        
        glTranslatef(-10,0,-14);

        // empecher le personnage de sortir de la scène
            int border = SCENE_SIZE/2 - 1;
            if(coordonnee_x > border) coordonnee_x = border;
            if(coordonnee_x < -border) coordonnee_x = -border;
            if(coordonnee_z > border) coordonnee_z = border;
            if(coordonnee_z < -border) coordonnee_z = -border;

        glTranslatef(coordonnee_x, 0, coordonnee_z);    
        glRotatef(angle_rotation, 0, 1, 0);             // translation et rotation causée par le déplacement du personnage
        creer_personnage(angle_deplacement);

    }
    glPopMatrix();

    creer_fenetres(SCENE_SIZE);

    axes();
    glutSwapBuffers();
}

int main(int argc, char **argv) 
{  
  return notre_init(argc, argv, &Modelisation);
}

