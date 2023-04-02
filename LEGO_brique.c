#include "LEGO_brique.h"

/*
    Construit le petit rond au dessus de la brique.
*/
void creer_rond_brique()
{
    glPushMatrix();
    {
        glRotatef(-90,1,0,0);   // rotation pour mettre le cylindre et le disque dans le bon sens

        GLUquadric *quadric = gluNewQuadric();

        GLfloat disque_radius = (1.275/2) - 0.17705;

        gluCylinder(quadric, 0.3, 0.3, 0.2, 30, 30);

        glTranslatef(0, 0, 0.2);
        
        gluDisk(quadric, 0, 0.3, 30, 30);
    }
    glPopMatrix();
}

/*
    Construit une seule brique de base, d'une taille de 1 x 1 x 1.2.
*/
void brique_1x1x1()
{
    glPushMatrix();
    {
        glTranslatef(0, 0.6, 0);

        glScalef(1, 1.2, 1);    
        glutSolidCube(1);
        glScalef(1, (1/1.2), 1);  // annulation de l'homotéthie pour la translation suivante

        glTranslatef(0,0.6,0);
        creer_rond_brique();
    }
    glPopMatrix();

}


/*
    Construit une brique plus fine correspondant à 1/3 de la hauteur d'une brique normale. 
*/
void brique_1x1x03()
{
    glPushMatrix();
    {
        glTranslatef(0, 0.2, 0);

        glScalef(1, 0.4, 1);
        glutSolidCube(1);
        glScalef(1, (1/0.4), 1);      // annulation de l'homotéthie pour la translation suivante

        glTranslatef(0,0.2,0);
        creer_rond_brique();
    }
    glPopMatrix();
}

/*
    Construit un ensemble de brique fine.

    @param width Nombre de briques en largeur.
    @param depth Nombre de briques en profondeur.
    @param height Nombre de briques en hauteur.
*/
void brique_fine(int width, int depth, int height)  
{
    int x,z;
    glPushMatrix();
    {
        for(int i=0; i<height; i++)
        {
            z=0;
            for(int i=0; i<depth; i++)
            {
                x=0;
                for(int i=0; i<width; i++)
                {
                    brique_1x1x03();
                    x++;
                    glTranslatef(1,0,0);
                }
                z++;
                glTranslatef(-x,0,1);
            }
            glTranslatef(0,1.2,-z);
        }
    }
    glPopMatrix();
}

/*
    Construit un ensemble de brique normale.

    @param width Nombre de briques en largeur.
    @param depth Nombre de briques en profondeur.
    @param height Nombre de briques en hauteur.
*/
void brique(int width, int depth, int height)
{
    int x,z;
    glPushMatrix();
    {
        // glTranslatef(0.5, 0, 0);
        for(int i=0; i<height; i++)
        {
            z=0;
            for(int i=0; i<depth; i++)
            {
                x=0;
                for(int i=0; i<width; i++)
                {
                    brique_1x1x1();
                    x++;
                    glTranslatef(1,0,0);
                }
                z++;
                glTranslatef(-x,0,1);
            }
            glTranslatef(0,1.2,-z);
        }
    }
    glPopMatrix();
}

























