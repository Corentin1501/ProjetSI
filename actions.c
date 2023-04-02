#include "actions.h"
#include "stdbool.h"    // pour utiliser les booléans

extern int blend;
extern int light;

// angle de la caméra
extern float anglex;    
extern float angley;    

// ce qui est lié au déplacement du personnage
extern float angle_deplacement;     // angle créé (pour les bras et les jambes)
extern bool  jambe_vers_lavant;     // si la jambe va vers l'avant ou vers l'arrière
extern float angle_rotation;        // rotation du personnage sur lui meme

// les coordonées du personnage lié au déplacement
extern float coordonnee_x;          
extern float coordonnee_z;          

// les coordonées de la caméra 
extern float camera_x;              
extern float camera_y;               
extern float camera_z;               

// si la lumière dans la maison est allumée ou non
extern bool lumiereMaisonON;


GLfloat pi = 3.14159265358979323846264338327950288419716939937510582;


void touche_pressee(unsigned char key, int x, int y) 
{
    usleep(100);

    switch (key)
    {    
        case ESCAPE: exit(1); break;                // la touche ECHAP quitte l'application

        case ESPACE:                                // la touche ESPACE allume/éteint la lumière de la maison
            lumiereMaisonON = !lumiereMaisonON;
            break;

        case TOUCHE_MIN_B: 
        case TOUCHE_MAJ_B: 
            blend =  switch_blend(blend);
            break;

        case TOUCHE_MIN_L: 
        case TOUCHE_MAJ_L: 
            light = switch_light(light);
            break;

        //####### DEPLACEMENT PERSO #######

            case TOUCHE_Z:
                //========= animation jambes =========
                    if (jambe_vers_lavant)
                    {
                        if(angle_deplacement < 40) angle_deplacement += 6;
                        else jambe_vers_lavant = false;
                    }
                    else 
                    {
                        if(angle_deplacement > -40) angle_deplacement -= 6;
                        else jambe_vers_lavant = true;
                    }
                //========= deplacement du personnage =========
                    coordonnee_x += 0.2 * sin((angle_rotation * pi) / 180);
                    coordonnee_z += 0.2 * cos((angle_rotation * pi) / 180);
                break;

            case TOUCHE_S:
                //========= animation jambes =========
                    if (jambe_vers_lavant)
                    {
                        if(angle_deplacement > -40) angle_deplacement -= 3;
                        else jambe_vers_lavant = false;
                    }
                    else 
                    {
                        if(angle_deplacement < 40) angle_deplacement += 3;
                        else jambe_vers_lavant = true;
                    }
                //========= deplacement du personnage =========
                    coordonnee_x -= 0.2 * sin((angle_rotation * pi) / 180);
                    coordonnee_z -= 0.2 * cos((angle_rotation * pi) / 180);

                break;

            case TOUCHE_Q:
                angle_rotation += 10;
                break;
            case TOUCHE_D:
                angle_rotation -= 10;
                break;

        //####### DEPLACEMENT CAMERA #######
            case TOUCHE_T:
                camera_z += 0.5;
                break;
            case TOUCHE_G:
                camera_z -= 0.5;
                break;
            case TOUCHE_F:
                camera_x += 0.5;
                break;
            case TOUCHE_H:
                camera_x -= 0.5;
                break;
            case TOUCHE_UP:
                camera_y -= 0.5;
                break;
            case TOUCHE_DOWN:
                camera_y += 0.5;
                break;
    }	
}

void touche(int touche, int x, int y)
{
    switch (touche) 
    {
        case GLUT_KEY_LEFT :
			angley -= 10;
			break;
		case GLUT_KEY_RIGHT :
			angley += 10;
			break;
		case GLUT_KEY_UP :
            anglex -= 10;
			break;
		case GLUT_KEY_DOWN :
			anglex += 10;
			break;
    }
}