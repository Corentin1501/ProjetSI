#include "LEGO_decor.h"
#include "LEGO_brique.h"
#include "ppm.h"
#include <math.h>

/*
    Fais un glcolor3f pour mettre différents couleurs

    @param n Corresponds à une couleur spécifique :
    1 = rouge, 2 = orange, 3 = jaune, 4 = vert, 5 = cyan, 6 = bleu, 7 = violet
*/
void couleur(int n)
{
    switch (n)
    {
        case 1: glColor3f(  1  , 0.07, 0.07); break;    // rouge
        case 2: glColor3f(  1  , 0.67, 0.07); break;    // orange
        case 3: glColor3f(  1  ,  1  , 0.07); break;    // jaune
        case 4: glColor3f( 0.07,  1  , 0.07); break;    // vert
        case 5: glColor3f( 0.07,  1  ,  1  ); break;    // cyan
        case 6: glColor3f( 0.07, 0.07,  1  ); break;    // bleu
        case 7: glColor3f(  1  , 0.07,  1  ); break;    // violet
    }
}

/*
    Génère une texture avec des paramètres par défaut : une seule application de la texture, répétition de la texture et enchantillonnage filtré

    @param texture TEXTURE_STRUCT *
*/
void genererTexture(TEXTURE_STRUCT * texture)
{
    // active une texture 2D pour l'objet
    glBindTexture(GL_TEXTURE_2D, texture->bpp);


    // fixe les paramètres par défaut : une seule application de la texture, REPETITION
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // fixe les paramètres par défaut : échantillonnage filtré (interpolation bi-linéaire)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // remplit la texture et fixe le format des données (couleurs, combien, etc.)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

/*
    Construit le sol avec une texture "Lego.ppm" répété 3 fois

    @param SCENE_SIZE taille de la scène
*/
void creer_sol(int SCENE_SIZE)
{
    glPushMatrix();
    {
        TEXTURE_STRUCT * texture = readPpm("textures/Lego.ppm");

        genererTexture(texture);

        glActiveTexture(GL_TEXTURE0);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D, texture->bpp);

        float coord = SCENE_SIZE/2.0;
        int repText = 3;                // nombre de repetition de la texture

        glColor3f(126/255.0, 255/255.0, 113/255.0);       // vert clair

        glTranslatef(0, -0.05, 0);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);             glVertex3f(-coord, 0, -coord);
            glTexCoord2f(repText, 0);       glVertex3f( coord, 0, -coord);
            glTexCoord2f(repText, repText); glVertex3f( coord, 0,  coord);
            glTexCoord2f(0, repText);       glVertex3f(-coord, 0,  coord);
        glEnd();

        glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

/*
    Construit la maison dans le coin de la scène avec un toit.
    La maison est constitué de 5 murs dont 2 avec un trou pour une fenêtre, et d'un trou pour une porte.


    @param SCENE_SIZE taille de la scène

*/
void creer_maison(int SCENE_SIZE)
{
    /*
    
    □                                         □
    □                                         □
    □ (1)                                     □
    □                                         □ (5)
    □                                         □
    □                 (4)                     □
    □             □ □ □ □ □ □ □ □ □ □         □                ^ -z
    □             □                                            |
    □     (2)     □ (3)                                        |
    □ □ □ □ □ □ □ □                                             ----> x
    
    */
    glPushMatrix();
    {
        couleur(3);     // les murs auront une couleur jaune

        glTranslatef(-(SCENE_SIZE/2), 0, -(SCENE_SIZE/2)); // a partir du coin de la scene

        //######## MUR 1 ########
            glRotatef(-90,0,1,0);

            brique(11,1,6);

        //######## MUR 2 ########
            glTranslatef(10,0,-1);
            glRotatef(90,0,1,0);

            brique(6,1,2);
            glTranslatef(0,2*1.2,0);
            brique(1,1,3);
            glTranslatef(5,0,0);
            brique(1,1,3);
            glTranslatef(-5,3*1.2,0);
            brique(6,1,1);

        //######## MUR 3 ########
            glTranslatef(6,-(5*1.2),0);
            glRotatef(90,0,1,0);

            brique(4,1,6);

        //######## MUR 4 ########
            glTranslatef(3,0,1);
            glRotatef(-90,0,1,0);

            brique(9,1,2);
            glTranslatef(0,2*1.2,0);
            brique(2,1,3);
            glTranslatef(6,0,0);
            brique(3,1,3);
            glTranslatef(-6,3.*1.2,0);
            brique(13,1,1);

        //######## MUR 5 ########

            glTranslatef(13,-(5*1.2),0);
            glRotatef(90,0,1,0);

            brique(8,1,6);

    }
    glPopMatrix();
    glPushMatrix();
    {
        // Construction du toit rouge

        glTranslatef(-(SCENE_SIZE/2), (1.2*6), -(SCENE_SIZE/2)); // a partir du coin de la scene

        couleur(1);             brique_fine(8,11,1);
        glTranslatef(8,0,0);    brique_fine(14,8,1);
    }
    glPopMatrix();
}



/*
    Construit une buche avec une texture de buche sur son sommet.
*/
void creer_buche()
{
    glPushMatrix();
    {
        glRotatef(-90,1,0,0);   // rotation pour mettre les cylindres et les tores dans le bon sens

        GLUquadric *quadric = gluNewQuadric();

        GLfloat Log_radius = 1;
        GLfloat Log_height = 2;

        glColor3f(102/255.0, 67/255.0, 21/255.0);   // couleur marron

        gluCylinder(quadric, Log_radius, Log_radius, Log_height, 30, 30);

        glTranslatef(0,0,Log_height);

        //######## Texture du haut de la buche ########

            TEXTURE_STRUCT * texture = readPpm("textures/buche_haut.ppm");
            genererTexture(texture);

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture->bpp);

            glBegin(GL_TRIANGLE_FAN);
            glTexCoord2f(0.5f, 0.5f); glVertex3f(0.0f, 0.0f, 0.0f);
            for (int i = 0; i <= 20; i++) {     // le disque aura 20 sections
                float angle = i * 2.0f * 3.14159f / 20.0f;  
                float x = cos(angle);
                float y = sin(angle);
                glTexCoord2f(x * Log_radius + 0.5, y * Log_radius + 0.5); glVertex3f(x * Log_radius, y * Log_radius, 0);
            }
            glEnd();

            glDisable(GL_TEXTURE_2D);
    }
    glPopMatrix();
}

/*
    Construit une fenêtre transparente de 4 x 3 blocs
*/
void fenetre() 
{
    glPushMatrix();
    {
        GLfloat WindowLittle_height = 0.4;
        GLfloat WindowBig_height = 3*1.2 - 2*WindowLittle_height;
        GLfloat WindowBig_depth = 1/3.0;
        GLfloat Window_width = 4.0;

        glScalef(Window_width, WindowLittle_height, 1);
        glutSolidCube(1);
        glScalef(1/Window_width, 1/WindowLittle_height, 1);

        glTranslatef(0,(WindowBig_height/2 + WindowLittle_height/2.0), 1/3.0);
        glScalef(Window_width, WindowBig_height, 1/3.0);
        glutSolidCube(1);
        glScalef(1/Window_width, 1/WindowBig_height, 3);

        glTranslatef(0,WindowBig_height/2 + WindowLittle_height/2, -1/3.0);
        glScalef(Window_width, WindowLittle_height, 1);
        glutSolidCube(1);
        glScalef(1/Window_width, 1/WindowLittle_height, 1);
    }
    glPopMatrix();
}

/*
    Construit les 2 fenêtres de la maison

    @param SCENE_SIZE taille de la scène
*/
void creer_fenetres(int SCENE_SIZE)
{
    glPushMatrix();
    {
        glTranslatef(-(SCENE_SIZE/2), 0, -(SCENE_SIZE/2)); // a partir du coin de la scene
        glTranslatef(2, (1.2*2), 10);

        glEnable(GL_BLEND);
        glColor4f(1,1,1,0.5);

        glTranslatef(1.5, 0.2, 0);

        fenetre();

        glTranslatef(8, 0, -3);

        fenetre();
    }
    glPopMatrix();
}



