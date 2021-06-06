/*!\file window.c 
 *\brief GL4Dummies, dessiner soit-même un polygone et faire le
 * remplissage.
 *
 * \author Farès BELHADJ, amsi@up8.edu
 * \date May 29 2020
 */

/* inclusion des entêtes de fonctions de gestion de primitives simples
 * de dessin. La lettre p signifie aussi bien primitive que
 * pédagogique. */
#include <GL4D/gl4dp.h>
/* pour la macro assert */
#include <assert.h>

#define W 1080
#define H 720

static int initialised = 0;
static int currentDir = 0;
static int polygon = 0;
static int y=0;
static int x=0;
static int dir[8][2] = { {1,1}, {1,0}, {1,-1}, {0,-1}, {-1,-1}, {-1,0}, {-1,1}, {0,1} };

/*\brief type de donnée pour un sommet 2d à coordonnées entières */
typedef struct point2di_t point2di_t; 

/*\brief structure de sommet 2d à coordonnées entières */
struct point2di_t {
  GLint x, y;
};

/* créez à l'aide de plusieurs polygones convexes une forme
   * correspondant le mieux possible à la première lettre de votre
   * prénom. */
static point2di_t p[][4] = { /* c'est un A */

  			{ {W/10, H-H/10}, {W/3+W/5, H-H/10}, {W/3+W/5, H-H/5-H/20}, {W/10, H-H/5-H/20}    },
  			{ {W/10, H-H/5-H/20}, {W/5, H-H/5-H/20}, {W/5, H/8}, {W/10, H/8}     		      },
  			{ {W/3+W/5, H-H/5-H/20}, {W/3+W/5, H/8}, {W/3+W/10, H/8}, {W/3+W/10, H-H/5-H/20}  },
  			{ {W/5, H-H/3-H/20}, {W/3+W/10, H-H/3-H/20}, {W/3+W/10, H-H/2}, {W/5, H-H/2}      }
};

/*\brief dessine, dans le screen en cours, à partir d'une liste de
 * sommets (points 2D) les segments reliant ces sommets deux à deux
 * puis referme la forme en reliant le dernier sommet au premier.
 *
 * Cette fonction ne gère que des polygones convexes.
 *
 *\param vertices (pluriel de vertex qui signifie sommet) le tableau
 * contenant les n sommets du polygone.
 *\param n le nombre de sommets du polygone (n doit être >= à 3).
 *\param fill booléen permettant d'indiquer si la forme est remplie
 *(colorée à l'intérieur) ou dessinée en mode fil de fer (il n'y a que
 *le contour qui est dessiné.
 */
void gl4dpDrawPolygon(point2di_t * vertices, int n, GLboolean fill) {
  int i;
  assert(n >= 3);
  for(i = 0; i < n; ++i) {
    gl4dpLine(vertices[i].x, vertices[i].y, vertices[(i + 1) % n].x, vertices[(i + 1) % n].y);
  }
}

int next_pixelDraw(){

	int i,a;

	for(i=0+currentDir, a=0; a<9; a++, i=(i+1)%8){

		if(IN_SCREEN(x+dir[i][0],y+dir[i][1])){
			if(!(gl4dpGetPixel(x+dir[i][0], y+dir[i][1]))){

				x+=dir[i][0];
				y+=dir[i][1];
				gl4dpPutPixel(x,y);
				currentDir=(i+8-1)%8;
				return 1;
			}
		}
	}
	return 0;
}

void find_baricentre(){

	int i,n;
	x=0, y=0;
	n=sizeof(p[polygon])/sizeof*(p[polygon]);

	for(i=0 ; i<n; i++){
		x+=p[polygon][i].x;
		y+=p[polygon][i].y;
	}
	y/=n;
	x/=n;
}

/*trouve le pixel le plus haut à l'intérieur du polygon à partir du baricentre*/

void find_firstPixel(){	

	find_baricentre();

	while(!(gl4dpGetPixel(x,y+1)))
		y++;

	gl4dpPutPixel(x,y);
}


void draw(){

  int iteration;

	/* appel de la fonction qui dessine les polygones convexes avec, à
   * chaque i, la liste de points p[i] */
  
  if(!initialised){
    for(int i = 0; i < 4; ++i)
      gl4dpDrawPolygon(p[i], sizeof p[i] / sizeof *p[i], GL_TRUE);
    initialised=1;
  }

  else if(initialised==1){
    find_firstPixel();
    initialised=2;
  }
  else if(initialised==2){

	for(iteration=0; iteration<1000; iteration++){

	  if(!next_pixelDraw()){
	  	if(polygon==3){
	  		initialised=3;
	  		polygon=0;
	  		find_baricentre();
	  	}
	  	else{
	  		initialised=1;
	  		polygon++;
	    }
	  	break; 	 
	  }
	}
  }

  if(initialised==3){
  	for(iteration=0; iteration<1000; iteration++){

  		if(!next_pixelDraw()){
  			if(polygon==3){
  				find_baricentre();
  				x+=2;
  				initialised=4;
  			}
  			else{
  				polygon++;
  				find_baricentre();
  			}
  			break;
  		}
  	}
  }

  if(initialised==4){
  	for(iteration=0; iteration<1000; iteration++){

  		if(!next_pixelDraw()){
  			initialised=5;
  			break;
  		}
  	}
  }


  
  gl4dpUpdateScreen(NULL);

}




/* inclusion des entêtes de fonctions de création et de gestion de
 * fenêtres système ouvrant un contexte favorable à GL4dummies. Cette
 * partie est dépendante de la bibliothèque SDL2 */
#include <GL4D/gl4duw_SDL2.h>

/*!\brief créé la fenêtre, un screen 2D effacé en noir et lance une
 *  boucle infinie.*/
int main(int argc, char ** argv) {
  int i;
  
  /* tentative de création d'une fenêtre pour GL4Dummies */
  if(!gl4duwCreateWindow(argc, argv, /* args du programme */
			 "GL4Dummies' Hello World", /* titre */
			 10, 10, W, H, /* x,y, largeur, hauteur */
			 GL4DW_SHOWN) /* état visible */) {
    /* ici si échec de la création souvent lié à un problème d'absence
     * de contexte graphique ou d'impossibilité d'ouverture d'un
     * contexte OpenGL (au moins 3.2) */
    return 1;
  }
  /* création d'un screen GL4Dummies (texture dans laquelle nous
   * pouvons dessiner) aux dimensions de la fenêtre */
  gl4dpInitScreen();
  /* effacement du screen en cours en utilisant la couleur par défaut,
   * le noir */
  gl4dpClearScreen();

  /* mettre la couleur en cours à rouge bordeaux */
  gl4dpSetColor(RGB(180, 40, 0));


  /*affiche le dessin en continu*/

  gl4duwDisplayFunc(draw);
  
  
  /* boucle infinie pour éviter que le programme ne s'arrête et ferme
   * la fenêtre */
  gl4duwMainLoop();
  return 0;
}
