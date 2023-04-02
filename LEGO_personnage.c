#include "LEGO_personnage.h"
#include "stdbool.h"    // pour utiliser les booléans

bool jambe_vers_lavant  = true;    // true si la jambe va vers l'avant, false si elle va vers l'arrière

static const GLfloat HEAD_DIAMETER       = 1.275;
static const GLfloat HEAD_HEIGHT         = 1.0625;
static const GLfloat TORUS_INNER_RADIUS  = 0.17705;

float angle_corps_bucheron = 0;
float angle_bras_bucheron  = 45;
bool  corps_bucheron_vers_lavant = true;
bool  bras_bucheron_vers_lavant  = true ;

/*
    Construit le petit rond au dessus de la tête.
*/
void creer_rond()
{
    glPushMatrix();
    {
        glRotatef(-90,1,0,0);

        GLUquadric *quadric = gluNewQuadric();

        GLfloat disque_radius = (1.275/2) - 0.17705;

        gluCylinder(quadric, 0.3, 0.3, 0.2, 30, 30);
        gluDisk(quadric, 0, disque_radius, 30, 30);
        glRotatef(90,1,0,0);
        glTranslatef(0, 0.2, 0);
        glRotatef(-90,1,0,0);
        gluDisk(quadric, 0, 0.3, 30, 30);
    }
    glPopMatrix();
}

/*
    Construit l'ensemble de la tête (cou, tete, petit rond)
*/
void tete()
{
    glPushMatrix();
    {
        glColor3f(1, 0.89, 0); // couleur jaune

        glRotatef(-90,1,0,0);   // rotation pour mettre les cylindres et les tores dans le bon sens

        GLUquadric *quadric = gluNewQuadric();

        // Head
            GLfloat Head_size = 1.275;

        // Torus
            GLdouble Torus_outerRadius = (HEAD_DIAMETER/2) - TORUS_INNER_RADIUS;
            GLint Torus_sides = 30;
            GLint Torus_rings = 30;

        // Cylinder
            GLdouble Cylinder_height = HEAD_HEIGHT - TORUS_INNER_RADIUS * 2;
            GLdouble Cylinder_radius = HEAD_DIAMETER/2;

        // Neck
            GLdouble heightNeck = 0.125;
            GLdouble radiusNeck = 0.945 / 2;

        gluCylinder(quadric, radiusNeck, radiusNeck, heightNeck, 30, 10);
        glTranslatef(0,0,(heightNeck+ TORUS_INNER_RADIUS));

        glutSolidTorus(TORUS_INNER_RADIUS, Torus_outerRadius, Torus_sides, Torus_rings);    // premier beignet

        gluCylinder(quadric, Cylinder_radius, Cylinder_radius, Cylinder_height, 30, 30);    // Gros cylindre de la tete
        glTranslatef(0, 0, Cylinder_height);                                                // translation vers le haut du cylindre
        
        glutSolidTorus(TORUS_INNER_RADIUS, Torus_outerRadius, Torus_sides, Torus_rings);    // deuxième beignet
        glTranslatef(0, 0, TORUS_INNER_RADIUS);                                             // translation vers le petit rond
        
        glRotatef(90,1,0,0);    // rotation pour remettre les axes dans le bon sens pour le petit rond
        creer_rond();           // petit rond de la tete
    }
    glPopMatrix();
}

/*
    Construit deux prisme de triangle
*/
void triangularPrism()
{
    glBegin(GL_QUADS);
        glVertex3f(0.5, 0, 0.5);    
        glVertex3f(0.5, 0, -0.5);   
        glVertex3f(-0.5, 0, -0.5);  
        glVertex3f(-0.5, 0, 0.5);   

        glVertex3f(0.5,0,-0.5);     
        glVertex3f(0.5,1,-0.5);     
        glVertex3f(-0.5,1,-0.5);    
        glVertex3f(-0.5,0,-0.5);    

        glVertex3f(0.5,1,-0.5);      
        glVertex3f(-0.5,1,-0.5);     
        glVertex3f(-0.5,0,0.5);      
        glVertex3f(0.5,0,0.5);       
    glEnd();
    glBegin(GL_TRIANGLES);
        glVertex3f(0.5,0,0.5);
        glVertex3f(0.5,1,-0.5);
        glVertex3f(0.5,0,-0.5);

        glVertex3f(-0.5,0,0.5);
        glVertex3f(-0.5,1,-0.5);
        glVertex3f(-0.5,0,-0.5);
    glEnd();
}

/*
    Construit le corps du LEGO avec un quadrilatère et deux prisme de triangle
*/
void corps()
{
    glPushMatrix();
    {
        // body
            GLfloat body_width_bottom = 1.605;
            GLfloat body_width_top = body_width_bottom - 0.165 * 2; 
            GLfloat body_height = 1.6;
            GLfloat body_depth = 0.98;

        // triangular prism
            GLfloat prism_width  = body_depth;
            GLfloat prism_depth  = (body_width_bottom - body_width_top) / 2 ;
            GLfloat prism_height = body_height;

        //little bottom
            GLfloat littleBottom_height = 0.125;

        glScalef(body_width_bottom, littleBottom_height, body_depth);
        glutSolidCube(1);

        glScalef(1/body_width_bottom, 1/0.125, 1/body_depth);
        glTranslatef(0, (littleBottom_height/2 + body_height/2), 0);

        glScalef(body_width_top, body_height, body_depth);        // homotéthie pour le tronc
        glutSolidCube(1);

        glScalef(1/body_width_top, 1/body_height, 1/body_depth);  // on annule l'homotéthie pour la translation suivante
        glTranslatef(-(body_width_top/2 + prism_depth/2), -(body_height/2), 0); // translation vers le prisme de gauche

        glRotatef(-90,0,1,0);                                     // rotation pour mettre le triangle dans le bon sens
        glScalef(prism_width, prism_height, prism_depth);         // homotéthie du triangle
        triangularPrism();

        glScalef(1/prism_width, 1/prism_height, 1/prism_depth);   // on annule l'homotéthie du triangle
        glRotatef(90,0,1,0);                                      // on annule la rotation pour mieux comprendre la translation suivante
        glTranslatef((body_width_bottom - prism_depth), 0, 0);    // translation vers le prisme de droite

        glRotatef(90,0,1,0);                                     // rotation pour mettre le triangle dans le bon sens
        glScalef(prism_width, prism_height, prism_depth);         // homotéthie du triangle
        triangularPrism();
    }
    glPopMatrix();
}

/*
    Construit un bras

    @param brasDroit 0 = bras gauche, 1 = bras droit
*/
void bras(GLint brasDroit)
{
    glPushMatrix();
    {
        GLfloat shoulder_angle = 20 * (-brasDroit);
        GLfloat shoulder_radius = 0.2;
        GLfloat elbow_angle = 10 * (-brasDroit);

        GLfloat upperArm_radius  = shoulder_radius;
        GLfloat upperArm_length  = 0.6;
        GLfloat lowerArm_radius  = shoulder_radius;
        GLfloat lowerArm_length  = 0.7;

        GLfloat wrist_radius  = shoulder_radius * 0.7;
        GLfloat wrist_length  = 0.1;

        GLfloat handTop_width   = lowerArm_radius*1.75; // 0.35
        GLfloat handTop_height  = 0.1;
        GLfloat handTop_depth   = lowerArm_radius*1.5;

        GLfloat handBottom_width   = handTop_height;
        GLfloat handBottom_height  = handTop_width - handTop_height; // 0.25
        GLfloat handBottom_depth   = handTop_depth;

        GLUquadric *quadric = gluNewQuadric();


        //##### Epaule #####
            glutSolidSphere(shoulder_radius, 20,20);    // Epaule

        //##### Biceps #####
            glRotatef(-shoulder_angle, 0, 0, 1);    // rotation pour continuer le bras
            glRotatef(90,1,0,0);    // rotation pour mettre les cylindres dans le bon sens
            gluCylinder(quadric, upperArm_radius, upperArm_radius, upperArm_length, 30, 30);    // partie haute du bras

        //##### Avant-bras #####
            glTranslatef(0, 0, upperArm_length);    // translation vers le coude
            glRotatef(elbow_angle, 0, 1, 0);        // rotation du coude

            glutSolidSphere(shoulder_radius, 10, 10);    // jointure du coude
            gluCylinder(quadric, lowerArm_radius, lowerArm_radius, lowerArm_length, 30, 30);    // partie basse du bras (avant-bras)

        //##### Poignée #####
            glColor3f(1, 0.89, 0); // couleur jaune

            glTranslatef(0, 0, lowerArm_length);                                    // translation vers le poignée
            gluDisk(quadric, 0, lowerArm_radius, 30, 30);
            gluCylinder(quadric, wrist_radius, wrist_radius, wrist_length, 30, 30); // cylindre du poignée

        //##### Main #####
            glTranslatef(0, 0, wrist_length + handTop_height/2);    // translation vers la main

            glRotatef(-90,1,0,0);   // annulation des rotations pour bien mettre les cylindres
            glScalef(handTop_width, handTop_height, handTop_depth);         // homotéthie de la partie haute de la main
            glutSolidCube(1);
            glScalef(1/handTop_width, 1/handTop_height, 1/handTop_depth);   // annulation de l'homotéthie pour la translation suivante

            glTranslatef(-(handTop_width/2 - handBottom_width/2), -(handTop_height/2 + handBottom_height/2), 0);    // translation vers la partie gauche de la main

            glScalef(handBottom_width, handBottom_height, handBottom_depth);        // homotéthie de la partie gauche de la main
            glutSolidCube(1);
            glScalef(1/handBottom_width, 1/handBottom_height, 1/handBottom_depth);  // annulation de l'homotéthie pour la translation suivante

            glTranslatef(handTop_width - handBottom_width, 0, 0);   // translation vers la partie droite de la main
            
            glScalef(handBottom_width, handBottom_height, handBottom_depth);    // homotéthie de la partie droite de la main
            glutSolidCube(1);

    }
    glPopMatrix();
}

/*
    Construit le slip qui est entre les deux jambes
*/
void slip()
{
    glPushMatrix();
    {
        glColor3f(0, 0.56, 0.65);     // couleur bleu jean

        GLfloat slip_width  = 1.44;
        GLfloat slip_height = 0.1875;
        GLfloat slip_depth  = 0.98;
        GLfloat betweenLegs_width  = 0.1125;
        GLfloat betweenLegs_height = 0.65;
        GLfloat betweenLegs_depth  = 0.98;

        glScalef(slip_width, slip_height, slip_depth);
        glutSolidCube(1);

        glScalef(1/slip_width, 1/slip_height, 1/slip_depth);
        glTranslatef(0,-(slip_height/2 + betweenLegs_height/2),0);

        glScalef(betweenLegs_width, betweenLegs_height, betweenLegs_depth);
        glutSolidCube(1);
    }
    glPopMatrix();
}

/*
    Construit une jambe
*/
void jambe()
{
    glPushMatrix();
    {
        glColor3f(0, 0.56, 0.65);     // couleur bleu jean
        
        GLfloat leg_width  = 0.7;
        GLfloat leg_height = 1.74;
        GLfloat leg_depth  = 0.8;
        GLfloat foot_width  = leg_width;
        GLfloat foot_height = 0.4;
        GLfloat foot_depth  = 0.18;

        // on part du haut de la jambe
        
        glTranslatef(0, -(leg_height/2), 0);

        glScalef(leg_width, leg_height, leg_depth);
        glutSolidCube(1);

        glScalef(1/leg_width, 1/leg_height, 1/leg_depth);
        glTranslatef(0, (-leg_height/2) + (foot_height/2), (leg_depth/2 + foot_depth/2));

        glScalef(foot_width, foot_height, foot_depth);
        glutSolidCube(1);

    }
    glPopMatrix();
}

/*
    Construit le personnage principal à partir du milieu entre les jambes au sol
*/
void creer_personnage(float angle_deplacement)
{
    glPushMatrix();
    {

        // on part du milieu entre les jambe au sol 

        glTranslatef(0, 1.74 + 0.1875 + 0.125/2, 0);

        glColor3f(0.09, 0.65, 0); // couleur vert
        corps();

        glTranslatef(0, 1.6 + 0.125/2, 0);

        // effet de plastique JUSTE sur la tete
            GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
            GLfloat diffuse[] = {0.5, 0.5, 0.5, 1.0};
            glMaterialfv(GL_FRONT, GL_SPECULAR,  specular);
            glMaterialfv(GL_FRONT, GL_DIFFUSE,   diffuse);
            glMaterialf (GL_FRONT, GL_SHININESS, 100.0);

        tete();

        // désactivation du rendu plastique
            GLfloat specular_disable[] = { 0.0, 0.0, 0.0, 1.0 };
            GLfloat diffuse_disable[] = { 0.8, 0.8, 0.8, 1.0 };
            GLfloat shininess[] = { 0.0 };
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specular_disable);
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse_disable);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

        glTranslatef(0, -(1.6 + 0.125 + 0.375/4), 0);
        
        slip();

        glTranslatef(-(0.1125/2 + 0.7/2), -(0.1875/2), 0);

        glRotatef(angle_deplacement, 1, 0, 0);
        jambe();
        glRotatef(-angle_deplacement, 1, 0, 0);

        glTranslatef(0.7+0.1125, 0, 0);

        glRotatef(-angle_deplacement, 1, 0, 0);
        jambe();
        glRotatef(angle_deplacement, 1, 0, 0);
    }
    glPopMatrix();

    glPushMatrix(); // on repart du sol entre les jambes pour que ce soit plus simple
    {
        glTranslatef(- (1.605/2 - 0.165/2), (1.74 + 0.1875 + 0.125 + 1.6*0.8), 0);

        glColor3f(0.09, 0.65, 0); // couleur vert
        glRotatef(-angle_deplacement, 1, 0, 0);
        bras(-1);
        glRotatef(angle_deplacement, 1, 0, 0);

        glTranslatef((1.605 -0.165), 0, 0);

        glColor3f(0.09, 0.65, 0); // couleur vert
        glRotatef(angle_deplacement, 1, 0, 0);
        bras(1);
        glRotatef(-angle_deplacement, 1, 0, 0);
    }
    glPopMatrix();
}

/*
    Construit la hache
*/
void creer_hache()
{
    glPushMatrix();
    {
        glRotatef(90,1,0,0);
        glRotatef(-13,0,1,0);

        glTranslatef(0,0,1.7);

        glRotatef(-26,0,1,0);

        glRotatef(-90,1,0,0);   // rotation pour mettre les cylindres dans le bon sens

        GLUquadric *quadric = gluNewQuadric();

        GLfloat AxeHandle_radius = 0.1;
        GLfloat AxeHandle_height = 2;

        GLfloat AxeBlade_height = AxeHandle_height * 0.4;
        GLfloat AxeBlade_width = 1;
        GLfloat AxeBlade_depth = AxeHandle_radius * 2;

        //######### Handle #########

            glColor3f(100/255.0, 70/255.0, 18/255.0);

            gluDisk(quadric,0,AxeHandle_radius,30,30);
            gluCylinder(quadric,AxeHandle_radius,AxeHandle_radius,AxeHandle_height,30,30);

            glTranslatef(0,0,AxeHandle_height);
            gluDisk(quadric,0,AxeHandle_radius,30,30);

            glRotatef(90,1,0,0);   // rotation pour mettre les cylindres dans le bon sens
        //######### Blade #########

            glColor3f(161/255.0, 161/255.0, 161/255.0);
            glRotatef(90,0,1,0);
            glTranslatef(-(AxeBlade_width/2),-(AxeBlade_height/2),0);

            glScalef(AxeBlade_width, AxeBlade_height, AxeBlade_depth);
            glutSolidCube(1);
            glScalef(1/AxeBlade_width, 1/AxeBlade_height, 1/AxeBlade_depth);
    }
    glPopMatrix();
}

/*
    Construit le personnage bucheron animé
*/
void creer_bucheron()
{
    glPushMatrix();
    {


        // on part du milieu entre les jambe au sol 

        glTranslatef(0, (1.74 + 0.1875/2), 0);

        slip();

        glTranslatef(-(0.1125/2 + 0.7/2), -(0.1875/2), 0);
        
        jambe();
        
        glTranslatef(0.7+0.1125, 0, 0);
        
        jambe();

        glRotatef(angle_corps_bucheron,1,0,0);
        glTranslatef(-(0.7/2 + 0.1125/2), (0.1875 + 0.125/2), 0);
        
        glColor3f(0.62, 0, 0); // couleur rouge foncé
        corps();
        
        glTranslatef(0, 1.6 + 0.125/2, 0);

        tete();

        glTranslatef(-(1.605/2 - 0.165/2), -0.32, 0);

        float angle_hache = 39;
        glColor3f(0.62, 0, 0); // couleur rouge foncé

        glRotatef(-(angle_bras_bucheron),1,0,0);
        glRotatef(angle_hache,0,1,0);
        glRotatef(-90,1,0,0);
        bras(-1);
        creer_hache();
        glRotatef(90,1,0,0);
        glRotatef(-angle_hache,0,1,0);
        glRotatef((angle_bras_bucheron),1,0,0);

        glTranslatef(1.605-0.165,0,0);
        glColor3f(0.62, 0, 0); // couleur rouge foncé

        glRotatef(-(angle_bras_bucheron),1,0,0);
        glRotatef(-angle_hache,0,1,0);
        glRotatef(-90,1,0,0);
        bras(1);
        glRotatef(90,1,0,0);
        glRotatef(angle_hache,0,1,0);
        glRotatef((angle_bras_bucheron),1,0,0);

        glRotatef(-angle_hache, 0, 1, 0);


    }
    glPopMatrix();
}


















