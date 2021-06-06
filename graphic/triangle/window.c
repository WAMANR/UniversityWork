/*!\file window.c 
 *\brief rastérisation de triangles entièrement faite à la main.
 *
 * \author Farès BELHADJ, amsi@up8.edu
 * \date June 04 2020
 */

/* inclusion des entêtes de fonctions de gestion de primitives simples
 * de dessin. La lettre p signifie aussi bien primitive que
 * pédagogique. */
#include <GL4D/gl4dp.h>
/* pour la macro assert */
#include <assert.h>

/* inclusion des entêtes de fonctions de création et de gestion de
 * fenêtres système ouvrant un contexte favorable à GL4dummies. Cette
 * partie est dépendante de la bibliothèque SDL2 */
#include <GL4D/gl4duw_SDL2.h>

#define W 1080
#define H 720

/*\brief type de donnée pour un sommet */
typedef struct vertex_t vertex_t; 

/*\brief structure de sommet */
struct vertex_t {
  GLint x, y;
  GLubyte r, g, b;
};

/* \brief type de donnée pour un mobile */
typedef struct mobile_t mobile_t;

/* \brief structure de données du mobile : coordonnée 2d flottante,
 * vitesse 2d flottante et couleur. */
struct mobile_t {
  float x, y;
  float vx, vy;
  GLuint color;
};

/* \brief l'ensemble des mobiles */
static mobile_t * _mobiles = NULL;
/* \brief le nombre de mobiles dans la simulation (c'est un multiple
 * de 3) */
static int _nb_mobiles = 0;

static int init = 0;

static inline void lineAbscisses(vertex_t * abscisses, vertex_t p0, vertex_t p1, int replace);
static inline void hLine(GLuint * s, vertex_t v0, vertex_t v1, int w, int h, GLuint color);
static void mobileInit(int nb_mobiles);
static void mobileFree(void);
static void mobileMove(void);

typedef struct point2di_t point2di_t; 
struct point2di_t {
  GLint x, y;
};

static point2di_t lettre[][3] = {  
           {  {W/10, H-H/10}, {W/3+W/5, H-H/10}, {W/3+W/5, H-H/5-H/20} }, 
           {  {W/3+W/5, H-H/5-H/20}, {W/10, H-H/5-H/20}, {W/10, H-H/10} }, 
           {  {W/10, H-H/5-H/20}, {W/5, H-H/5-H/20}, {W/5, H/8} }, 
           {  {W/5, H/8}, {W/10, H/8}, {W/10, H-H/5-H/20} }, 
           {  {W/3+W/5, H-H/5-H/20}, {W/3+W/5, H/8}, {W/3+W/10, H/8} }, 
           {  {W/3+W/10, H/8}, {W/3+W/10, H-H/5-H/20}, {W/3+W/5, H-H/5-H/20} }, 
           {  {W/5, H-H/3-H/20}, {W/3+W/10, H-H/3-H/20}, {W/3+W/10, H-H/2} }, 
           {  {W/3+W/10, H-H/2}, {W/5, H-H/2}, {W/5, H-H/3-H/20} }
   }; 

void gl4dpDrawTriangle(vertex_t * vertices) {
  int haut, median, bas, hauteur;
  int i, signe, w = gl4dpGetWidth(), h = gl4dpGetHeight();
  vertex_t * aG = NULL; /* abscisses à gauche */
  vertex_t * aD = NULL; /* abscisses à droite */
  GLuint * s = gl4dpGetPixels(), color = gl4dpGetColor();
  if(vertices[0].y > vertices[1].y) {
    if(vertices[0].y > vertices[2].y) {
      haut = 0;
      if(vertices[1].y > vertices[2].y) {
	median = 1;
	bas = 2;
      } else {
	median = 2;
	bas = 1;
      }
    } else {
      haut = 2;
      median = 0;
      bas = 1;
    }
  } else {
    if(vertices[1].y > vertices[2].y) {
      haut = 1;
      if(vertices[0].y > vertices[2].y) {
	median = 0;
	bas = 2;
      } else {
	median = 2;
	bas = 0;
      }
    } else {
      haut = 2;
      median = 1;
      bas = 0;
    }
  }
  hauteur = vertices[haut].y - vertices[bas].y + 1;
  aG = malloc(hauteur * sizeof *aG);
  assert(aG);
  aD = malloc(hauteur * sizeof *aD);
  assert(aD);

  /* (D): px + y + c = 0 */
  /* px0 + y0 + c = 0 => c = -px0 - y0 (1) */
  /* px1 + y1 + c = 0 => c = -px1 - y1 (2) */
  /* (2) - (1) => 0 = -p(x1 - x0) - (y1 - y0) => p = (y0 - y1) / (x1 - x0) => c = ... on injecte dans (1) ou (2) */
  /* si x0 == x1 alors (D): x0 - x = 0  */

  if(vertices[haut].x == vertices[bas].x) { /* (D): -x + x[haut] = 0  */
    signe = vertices[haut].x - vertices[median].x < 0 ? -1 : 1;
  } else { /* (D): px + y + c = 0 */
    float p = (vertices[bas].y - vertices[haut].y) / (float)(vertices[haut].x - vertices[bas].x);
    float c = -p * vertices[bas].x - vertices[bas].y;
    signe = p * vertices[median].x + vertices[median].y + c;
    signe = signe < 0 ? -1 : (signe > 0 ? 1 : 0);
  }

  /* printf("haut : (%d, %d)\n", vertices[haut].x, vertices[haut].y); */
  /* printf("median : (%d, %d)\n", vertices[median].x, vertices[median].y); */
  /* printf("bas : (%d, %d)\n", vertices[bas].x, vertices[bas].y); */
  /* printf("signe %d\n", signe); */
  /* fflush(stdout); */
  
  if(signe < 0) { /* median est à droite de haut-bas */
    lineAbscisses(aG, vertices[haut], vertices[bas], 1);
    lineAbscisses(aD, vertices[haut], vertices[median], 1);
    lineAbscisses(&aD[vertices[haut].y - vertices[median].y], vertices[median], vertices[bas], 0);
  } else if(signe > 0) { /* median est gauche de haut-bas */
    lineAbscisses(aD, vertices[haut], vertices[bas], 1);
    lineAbscisses(aG, vertices[haut], vertices[median], 1);
    lineAbscisses(&aG[vertices[haut].y - vertices[median].y], vertices[median], vertices[bas], 0);
  } else { /* triangle plat */
    if(hauteur == 1) {
      //aG[0] = MIN(MIN(vertices[0].x, vertices[1].x), vertices[2].x);
      //aD[0] = MAX(MAX(vertices[0].x, vertices[1].x), vertices[2].x);
    } else {
      lineAbscisses(aG, vertices[haut], vertices[bas], 1);
      lineAbscisses(aD, vertices[haut], vertices[bas], 0);
    }
  }

  /* for(i = 0; i < hauteur; ++i) */
  /*   printf("A la ligne %d, hLine de %d à %d\n", vertices[haut].y - i, aG[i], aD[i]); */
  /* fflush(stdout); */

  for(i = 0; i < hauteur; ++i)
    hLine(s, aG[i], aD[i], w, h, color);
  free(aG);
  free(aD);
}

static void dessin(void) {
  /* créez à l'aide de plusieurs triangle une forme correspondant le
   * mieux possible à la première lettre de votre prénom. */ 
  /* for(int i = 0; i < (sizeof p / sizeof *p); ++i) { */
  /*   gl4dpSetColor(RGB(rand()&255, rand()&255, rand()&255)); */
  /*   gl4dpDrawTriangle(p[i], GL_TRUE); */
  /* } */

  /* effacement du screen en cours en utilisant la couleur par défaut,
   * le noir */
  gl4dpClearScreen();
  for(int i = 0; i < _nb_mobiles / 3; ++i) {
    vertex_t p[3] = {
		     {
		      (GLint)_mobiles[0 + 3 * i].x, (GLint)_mobiles[0 + 3 * i].y,
		      RED(_mobiles[0 + 3 * i].color), GREEN(_mobiles[0 + 3 * i].color), BLUE(_mobiles[0 + 3 * i].color)
		     },
		     {
		      (GLint)_mobiles[1 + 3 * i].x, (GLint)_mobiles[1 + 3 * i].y,
		      RED(_mobiles[1 + 3 * i].color), GREEN(_mobiles[1 + 3 * i].color), BLUE(_mobiles[1 + 3 * i].color)
		     },
		     {
		      (GLint)_mobiles[2 + 3 * i].x, (GLint)_mobiles[2 + 3 * i].y,
		      RED(_mobiles[2 + 3 * i].color), GREEN(_mobiles[2 + 3 * i].color), BLUE(_mobiles[2 + 3 * i].color)
		     }
    };
    gl4dpSetColor(RGB(p[0].r, p[0].g, p[0].b));
    gl4dpDrawTriangle(p);
  }

  gl4dpUpdateScreen(NULL);
}

/*!\brief créé la fenêtre, un screen 2D effacé en noir et lance une
 *  boucle infinie.*/
int main(int argc, char ** argv) {
  /* tentative de création d'une fenêtre pour GL4Dummies */
  if(!gl4duwCreateWindow(argc, argv, /* args du programme */
			 "GL4Dummies' Hello World", /* titre */
			 10, 10, W, H, /* x,y, largeur, heuteur */
			 GL4DW_SHOWN) /* état visible */) {
    /* ici si échec de la création souvent lié à un problème d'absence
     * de contexte graphique ou d'impossibilité d'ouverture d'un
     * contexte OpenGL (au moins 3.2) */
    return 1;
  }
  /* création d'un screen GL4Dummies (texture dans laquelle nous
   * pouvons dessiner) aux dimensions de la fenêtre */
  gl4dpInitScreen();
  if(!init){
    mobileInit(8);
    init=1;
  }
  
  gl4duwDisplayFunc(dessin);
  gl4duwIdleFunc(mobileMove);
  /* boucle infinie pour éviter que le programme ne s'arrête et ferme
   * la fenêtre */
  gl4duwMainLoop();
  return 0;
}



static inline void lineAbscisses(vertex_t * abscisses, vertex_t p0, vertex_t p1, int replace) {
  int i = 0, lastpp /* dernier + pas */;
  int u = p1.x - p0.x, v = p1.y - p0.y;
  int pasX = (u < 0) ? -1 : 1, pasY = (v < 0) ? -1 : 1;
  int x, y, del, incH, incO;
  float d = sqrt((p1.x - p0.x) * (p1.x - p0.x) + (p1.y - p0.y) * (p1.y - p0.y)), d0, prop0, prop1;
  if(abs(u) < abs(v)) { /* deuxieme octant */
    lastpp = p1.y + pasY;
    del = (incH = ((pasX * u) << 1)) - pasY * v;
    incO = incH - ((pasY * v) << 1);
    for(y = p0.y, x = p0.x; y != lastpp; y += pasY) {
      d0 = sqrt((x - p0.x) * (x - p0.x) + (y - p0.y) * (y - p0.y));
      prop0 = (d - d0) / d; prop1 = 1.0f - prop0;
      abscisses[i].x = x;
      abscisses[i].y = y;
      abscisses[i].r = (GLubyte)(prop0 * p0.r + prop1 * p1.r);
      abscisses[i].g = (GLubyte)(prop0 * p0.g + prop1 * p1.g);
      abscisses[i].b = (GLubyte)(prop0 * p0.b + prop1 * p1.b);
      ++i;
      if(del < 0) del += incH;
      else {
	del += incO;
	x += pasX;
      }
    }
  } else {  /* premier octant */
    lastpp = p1.x + pasX;
    del = (incH = ((pasY * v) << 1)) - pasX * u;
    incO = incH - ((pasX * u) << 1);
    if(replace) {
      for(x = p0.x, y = p0.y; x != lastpp; x += pasX) {
	d0 = sqrt((x - p0.x) * (x - p0.x) + (y - p0.y) * (y - p0.y));
	prop0 = (d - d0) / d; prop1 = 1.0f - prop0;
	abscisses[i].x = x;
	abscisses[i].y = y;
	abscisses[i].r = (GLubyte)(prop0 * p0.r + prop1 * p1.r);
	abscisses[i].g = (GLubyte)(prop0 * p0.g + prop1 * p1.g);
	abscisses[i].b = (GLubyte)(prop0 * p0.b + prop1 * p1.b);
	if(del < 0) del += incH;
	else {
	  del += incO;
	  y += pasY;
	  ++i;
	}
      }
    } else {
      int done = 0;
      for(x = p0.x, y = p0.y; x != lastpp; x += pasX) {
	if(!done) {
	  d0 = sqrt((x - p0.x) * (x - p0.x) + (y - p0.y) * (y - p0.y));
	  prop0 = (d - d0) / d; prop1 = 1.0f - prop0;
	  abscisses[i].x = x;
	  abscisses[i].y = y;
	  abscisses[i].r = (GLubyte)(prop0 * p0.r + prop1 * p1.r);
	  abscisses[i].g = (GLubyte)(prop0 * p0.g + prop1 * p1.g);
	  abscisses[i].b = (GLubyte)(prop0 * p0.b + prop1 * p1.b);
	  done = 1;
	}
	if(del < 0) del += incH;
	else {
	  del += incO;
	  y += pasY;
	  ++i; done = 0;
	}
      }
    }
  }
}

static inline void hLine(GLuint * s, vertex_t v0, vertex_t v1, int w, int h, GLuint color) {
  //assert(x0 <= x1); \todo remettre cet assert / détecter et arranger le cas avant
  if(v0.x > v1.x) { vertex_t t = v0; v0 = v1; v1 = t; }
  int hm1 = h - 1, wm1 = w - 1;
  if( v0.y < 0 ||
      v0.y > hm1 ||
      (v0.x < 0 && v1.x < 0) ||
      (v0.x > wm1 && v1.x > wm1) )
    return;
  int x, pasX = (v1.x - v0.x) < 0 ? -1 : 1, x1pp, yw = v0.y * w;
  float d = (v1.x - v0.x), d0, prop0, prop1;
  v0.x = MAX(0, v0.x);
  v1.x = MIN(v1.x, wm1);
  x1pp = v1.x + pasX;
  for(x = v0.x; x != x1pp; x += pasX) {
    GLubyte r, g, b;
    d0 = (x - v0.x);
    prop0 = (d - d0) / d; prop1 = 1.0f - prop0;
    r = (GLubyte)(prop0 * v0.r + prop1 * v1.r);
    g = (GLubyte)(prop0 * v0.g + prop1 * v1.g);
    b = (GLubyte)(prop0 * v0.b + prop1 * v1.b);
    s[yw + x] = RGB(r, g, b);
  }
}


/* \brief initialise (ou réinitialise) l'ensemble des mobiles de la
 * simulation, leur nombre est donné par \a nb_mobiles * 3.
 *
 * \param nb_mobiles le tier du nombre de mobiles de la simulation.
 */
static void mobileInit(int nb_mobiles) {
  int i;
  if(_mobiles) {
    mobileFree();
  } else {
    atexit(mobileFree);
  }
  _nb_mobiles = 3 * nb_mobiles;
  _mobiles = malloc(_nb_mobiles * sizeof *_mobiles);
  assert(_mobiles);
  for(i = 0; i < _nb_mobiles; ++i) {
    _mobiles[i].x = (float) (lettre[i/3][i%3].x);
    _mobiles[i].y = (float) (lettre[i/3][i%3].y);
    _mobiles[i].vx = (float) (W/2);
    _mobiles[i].vy = (float) (H/2);

    if(i==2 || i==3 || i==12 || i==17)
      _mobiles[i].color = RGB(139,26,26);
    else if(i<2 || (i>3 && i<6) || (i>12 && i<15) || (i>14 && i<17) || i==6 || i==11 || i==20 || i==21)
      _mobiles[i].color = RGB(238,118,0);
    else if((i>6 && i<9) || (i>8 && i<11) || (i>17 && i<20) || i>21)
      _mobiles[i].color = RGB(238,180,34);
  }
}

/* \brief libère la mémoire et réinitialise les paramètres de la
 *  simulation. Cette fonction est potentiellement appelée à
 *  l'événement exit.
 */
static void mobileFree(void) {
  if(_mobiles) {
    free(_mobiles);
    _mobiles = NULL;
    _nb_mobiles = 0;
  }
}

/* \brief déplace tous les mobiles en fonction de leur position,
 * vitesse et du temps.
 */
static void mobileMove(void) {
  int i, w = gl4dpGetWidth(), h = gl4dpGetHeight();
  /* récupération du dt */
  static double t0 = 0.0;
  double t = gl4dGetElapsedTime() / 1000.0;
  double dt = t - t0;
  t0 = t;
  /* fin de récupération du dt */
  for(i = 0; i < _nb_mobiles; ++i) {
    /* futur x et futur y */
    float fx, fy;
    fx = _mobiles[i].x + _mobiles[i].vx * dt;
    fy = _mobiles[i].y + _mobiles[i].vy * dt;
    if((fx < 0.0f && _mobiles[i].vx < 0.0f) ||
       (fx >= w && _mobiles[i].vx > 0.0f))
      _mobiles[i].vx = -_mobiles[i].vx;
    if((fy < 0.0f && _mobiles[i].vy < 0.0f)||
       (fy >= h && _mobiles[i].vy > 0.0f))
      _mobiles[i].vy = -_mobiles[i].vy;
    _mobiles[i].x += _mobiles[i].vx * dt;
    _mobiles[i].y += _mobiles[i].vy * dt;
  }
}
