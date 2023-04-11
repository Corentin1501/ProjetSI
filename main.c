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


static int faces[][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {1, 4, 7, 2}, {0, 3, 6, 5}, {2, 7, 6, 3}, {1, 0, 5, 4}};
static int sommets[][3] = {{0, 3, 5}, {0, 2, 5}, {0, 2, 4}, {0, 3, 4}, {1, 2, 5}, {1, 3, 5}, {1, 3, 4}, {1, 2, 4}};


struct point { GLfloat x,y,z; };
struct cube { 
    struct point sommets[8]; 
};

struct cube creer_cube1(float taille)
{
    // struct cube* un_cube = (struct cube*)malloc(sizeof(struct cube));
    struct cube un_cube;
    
    un_cube.sommets[0].x = -taille/2;
    un_cube.sommets[0].y = -taille/2;
    un_cube.sommets[0].z = taille/2;
    
    un_cube.sommets[1].x = taille/2;
    un_cube.sommets[1].y = -taille/2;
    un_cube.sommets[1].z = taille/2;

    un_cube.sommets[2].x = taille/2;
    un_cube.sommets[2].y = taille/2;
    un_cube.sommets[2].z = taille/2;

    un_cube.sommets[3].x = -taille/2;
    un_cube.sommets[3].y = taille/2;
    un_cube.sommets[3].z = taille/2;

    un_cube.sommets[4].x = taille/2;
    un_cube.sommets[4].y = -taille/2;
    un_cube.sommets[4].z = -taille/2;

    un_cube.sommets[5].x = -taille/2;
    un_cube.sommets[5].y = -taille/2;
    un_cube.sommets[5].z = -taille/2;

    un_cube.sommets[6].x = -taille/2;
    un_cube.sommets[6].y = taille/2;
    un_cube.sommets[6].z = -taille/2;

    un_cube.sommets[7].x = taille/2;
    un_cube.sommets[7].y = taille/2;
    un_cube.sommets[7].z = -taille/2;

    return un_cube;
}
struct voiture {
    struct cube bottom;
    struct cube top;
};

struct voiture creerVoiture(float taille)
{
    struct voiture uneVoiture;

    uneVoiture.bottom = creer_cube1(taille);
    uneVoiture.top = creer_cube1(taille/2);

    return uneVoiture;
}

struct voiture* Creation_Voitures()
{
    struct voiture* voitures = malloc(1 * sizeof(struct voiture)); // alloue la mémoire pour le tableau de voitures

    struct voiture uneVoiture = creerVoiture(1);

    voitures[0] = uneVoiture;

    return voitures;
}

void affiche_cube1(struct cube un_cube)
{
    int j = 0;
    for(j = 0; j <= 5; j++) 
    {

        glBegin(GL_QUADS);
        int i = 0;
        for (i = 0; i <= 3; i++) 
        {
            glVertex3f( un_cube.sommets[faces[j][i]].x,
                        un_cube.sommets[faces[j][i]].y,
                        un_cube.sommets[faces[j][i]].z);
        }
        glEnd();
    }																																																																																																																																																																																																																																																										
}

void afficherVoiture(struct voiture une_voiture)
{
    affiche_cube1(une_voiture.bottom);
    affiche_cube1(une_voiture.top);
}

GLvoid Modelisation()
{
    VM_init();

    glColor3f(1, 60/255.0, 65/255.0);

    struct voiture* voitures = Creation_Voitures();

    afficherVoiture(voitures[0]);


    axes();
    glutSwapBuffers();
}

int main(int argc, char **argv) 
{  
  return notre_init(argc, argv, &Modelisation);
}

