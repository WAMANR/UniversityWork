/*!\file DrawLineInBmp.cpp
 *
 * \brief dessin d'un segement de droite en mémoire et sauvegarde dans un fichier BMP.
 *
 * Ce code ne fonctionne que pour une droite définie dans le premier
 * octan.
 *
 * Voir les exercices en fin de fichier.
 *
 * \author Farès Belhadj amsi@up8.edu
 * \date May 26, 2020
 */
#include <iostream>
#include <string.h> /* pour memset */
#include <math.h>
#include "images_io.h"

#define signe(a) ((a) < 0 ? -1 : 1)
#define W 200
#define H 200


static void draw_line(unsigned char * image, int w, int x0, int y0, int x1, int y1, unsigned char color){
  float y = 0.0f, pente;
  int u = x1 - x0, v = y1 - y0, pasX;

  if(abs(u) > abs(v)){

      pente = v / (float)abs(u);
      pasX = signe(u);
      for (int x = 0; x != u; x += pasX) {
          if (((y0 + ((int)y)) * w + x + x0) < 0 || ((y0 + ((int)y)) * w + x + x0) >= (H * W)){
              y += pente;
              continue;
          }

          image[(y0 + ((int)y)) * w + x + x0] = color;
          y += pente;
      }
  }
  else{

      pente = u / (float)abs(v);
      pasX = signe(v);
      for (int x = 0; x != v; x += pasX) {
          if (((y0 + x) * w + ((int)y) + x0) < 0 || ((y0 + x) * w + ((int)y) + x0) >= (H * W)){
              y += pente;
              continue;
          }

          image[(y0 + x) * w + ((int)y) + x0] = color;
          y += pente;
      }
  }
}

static void draw_circle(unsigned char* image, int w, int h, unsigned char color){
    int x, y, tmp, eq;
    int H_middle = h / 2;
    int W_middle = w / 2;
    int rayon = 60;

    for (x = 0; x < h; x++)
    {
        for (y = 0; y < w; y++)
        {
            tmp = (((x - H_middle) * (x - H_middle)) + (y - W_middle) * (y - W_middle));
            eq = sqrt(tmp);
            if (rayon == eq) {
                image[x * w + y] = color;
            }
        }
    }
}

int main(void){

  unsigned char image[H * W], color = 255;
  int H_middle = H / 2;
  int W_middle = W / 2;
  int X, Y, theta = 0;
  memset(image, 0, sizeof image);

  /*draw_line(image, W, 5, 10, 70, 50, 255);
  draw_line(image, W, 75, 10, 0, 50, 255);
  draw_line(image, W, 10, 10, 20, 50, 255);
  draw_line(image, W, -10, -10, 120, 150, 255);*/

  for (int i = 0; i < 60; i++){

      X = W_middle + cos(theta) * H_middle;
      Y = H_middle + sin(theta) * H_middle;
      draw_line(image, W, W_middle, H_middle, X, Y, color);
      theta += 5;
      color -= 5;
  }
  draw_circle(image, W, H, 255);

  imageSaveBMP("result.bmp", image, W, H, 1, 8);
  return EXIT_SUCCESS;
}

/*
  Exercices :

  exercice 1 : essayez de dessiner le segment (75, 10, 0, 50, 255), ça ne fonctionne pas. Corriger drawLine.
  exercice 2 : essayez de dessiner le segment (10, 10, 20, 50, 255), ça ne fonctionne pas. Corriger drawLine.
  exercice 3 : essayez de dessiner le segment (-10, -10, 120, 150, 255), ça ne fonctionne pas. Corriger drawLine.
  exercice 4 : dessiner (si ça marche) toute les positions d'une aiguille trotteuse en variant les intensités de gris (vous pouvez utiliser cos et sin dans math.h).
  exercice 5 : l'équation d'un cercle est donnée par y = (+ ou -) sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0)); écrire drawCircle.
*/
