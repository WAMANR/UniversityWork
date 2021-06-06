/**
 * \file Bloc.hpp
 * \brief Gère la création de blocs.
 * \author BONNEROT Thomas, ROUICHI Adil
 * \version 1.0
 * \date 31 mai 2020
 *
 * Gère la création des blocs en fonction de l'ID du bloc transmis afin de les utiliser en jeu.
 *
 */
#include <iostream>
#include<vector>
#include<cassert>
#include <cstdlib>
#include <ctime>

using namespace std;

class Bloc{

private:

public:
  int number;///<Cette variable correspond à l'ID du bloc.
  int ** bloc;///<Cette variable correspond à un tableau d'entier représentant le bloc.
  int i;///<Cette variable correspond à la coordonnée x de la case infèrieure gauche.
  int height;///<Cette variable correspond à la hauteur du bloc et à la coordonnée y de la case infèrieure gauche du bloc.
  int width;///<Cette variable correspond à la largeur du bloc.

  /**
 * \fn Bloc()
 * \brief Cette fonction est le constructeur de l'objet de type Bloc.
 *
 * \param int number
 * \return void
 */
  Bloc(int number){

    this->number=number;

    switch(this->number){
      // Bloc pour le plateau commun
      case 0 ://Bloc 1H/1W
        this->height=1;
        this->width=1;
        break;
      case 1 : case 2 : case 3 : case 4: case 5 ://Bloc 2H/2W
        this->height = 2;
        this->width = 2;
        break;
      case 6 : case 7 : case 8 : case 9 : case 10 : case 11 : // Bloc 2H/3W
        this->height = 2;
        this->width = 3;
        break;
      case 12 : case 13 : case 14 : case 15 : case 16 : case 17 : // Bloc 3H/2W
        this->height = 3;
        this->width = 2;
        break;
      case 18 : // Bloc 4H/1W
        this->height = 4;
        this->width = 1;
        break;
      case 19 :
        this->height = 1; //Bloc 1H/4W
        this->width = 4;
        break;
        // Bloc du plateau Cercle

      case 20 : case 21 : case 22 : case 23 : case 24 : // Bloc 4H/4W
        this->height = 4;
        this->width = 4;
        break;
      case 25 : case 26 : // Bloc 2H/2W
        this->height = 2;
        this->width = 4;
        break;
      case 27 : // Bloc 4H/3W
        this->height = 4;
        this->width = 3;
        break;
      case 28 : //
        this->height = 4; // Bloc 4H/2W
        this->width = 2;
        break;
      case 29 :
        this->height = 5; // Bloc 5H/1W
        this->width = 1;
        break;
      case 30 : // Bloc 1H/5W
        this->height = 1;
        this->width = 5;
        break;
      case 31 : // Bloc 2H/5W
        this->height = 2;
        this->width = 5;
        break;

      // Bloc plateau Losange
      case 32 : case 33 : case 34 : case 35: case 36 : case 37 : case 38 : // 4H/4W meme dimension que pour le plateau cercle mais ici pour le plateau Losange
        this->height = 4;
        this->width = 4;
        break;

      case 39 : // Bloc de 3H/5W
        this->height = 3;
        this->width = 5;
        break;
      case 40 :  // Bloc de 5/1W
        this->height = 5;
        this->width = 1;
        break;
      case 41 : // Bloc de 3H/4W
        this->height = 3;
        this->width = 4;
        break;
      case 42 :  // Bloc de 1H/5W
        this->height = 1;
        this->width = 5;
        break;
      case 43 : // Bloc de 2H/4W
        this->height = 2;
        this->width = 4;
        break;
      case 44 : case 45 : // Bloc de 4H/2W
        this->height = 4;
        this->width = 2;
        break;

        // Bloc plateau Triangle

      case 46 : case 47 : case 48 : case 49 : case 50 : case 51 : case 52 :  // Bloc de 3H/3W
        this->height = 3;
        this->width = 3;
        break;
      case 53 : // Bloc de 3H/1w
        this->height = 3;
        this ->width = 1;
        break;
      case 54 : // Bloc de 1H/3w
        this->height = 1;
        this->width = 3;
        break;
      case 55 : // Bloc de 2H/1W
        this->height = 2;
        this->width = 1;
        break;
      case 56 : // Blco de 1H/2W
        this->height = 1;
        this->width = 2;
        break;
      default :
        cout<<"Bloc constructor Error"<<endl;
    }
    bloc_malloc();
    bloc_fillVoid();
    bloc_make();
    bloc_findI();
  }

  /**
 * \fn bloc_malloc()
 * \brief Cette fonction permet de malloc le bloc selon ses dimensions.
 *
 * \param void
 * \return void
 */

  void bloc_malloc(void){
    int i;

    this->bloc=(int**)malloc(this->height*sizeof(int*));
    assert(this->bloc);
    for(i=0; i<this->height; i++){
      this->bloc[i]=(int*)malloc(this->width*sizeof(int));
      assert(this->bloc[i]);
    }
  }

  /**
 * \fn bloc_make()
 * \brief Cette fonction rempli le bloc selon son ID.
 *
 * \param void
 * \return void
 */

  void bloc_make(void){

    int color;
    color = rand() % 7+2;


    switch(this->number){
      case 0://Bloc base 11 : 1 carré
        this->bloc[0][0]=color;
        break;
      case 1 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        break;
      case 2 :
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        break;
      case 3 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        break;
      case 4 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        break;
      case 5 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        break;
      case 6 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        break;
      case 7 :
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        break;
      case 8 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        break;
      case 9 :
        this->bloc[0][2]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        break;
      case 10 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[1][1]=color;
        break;
      case 11 :
        this->bloc[0][2]=color;
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        break;
      case 12 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        this->bloc[2][1]=color;
        break;
      case 13 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][0]=color;
        break;
      case 14 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][1]=color;
        break;
      case 15 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        this->bloc[2][1]=color;
        break;
      case 16 :
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][1]=color;
        break;
      case 17 :
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][0]=color;
        break;
      case 18 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        this->bloc[3][0]=color;
        break;
      case 19 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        break;

        //Bloc du plateau Cercle
      case 20 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[1][3]=color;
        this->bloc[2][0]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        this->bloc[2][3]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        this->bloc[3][3]=color;
        break;
      case 21 :
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[1][3]=color;
        this->bloc[2][0]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        this->bloc[2][3]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        break;
      case 22 :
        this->bloc[0][0]=color;
        this->bloc[0][3]=color;
        this->bloc[1][0]=color;
        this->bloc[1][3]=color;
        this->bloc[2][0]=color;
        this->bloc[2][3]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        this->bloc[3][3]=color;
        break;
      case 23 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][3]=color;
        this->bloc[2][3]=color;
        this->bloc[3][3]=color;
        break;
      case 24 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        this->bloc[2][3]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        this->bloc[3][3]=color;
        break;
      case 25 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        break;
      case 26 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[1][3]=color;
        break;
      case 27 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[1][2]=color;
        this->bloc[2][2]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        break;
      case 28 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][0]=color;
        this->bloc[2][1]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        break;
      case 29 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        this->bloc[3][0]=color;
        this->bloc[4][0]=color;
        break;
      case 30 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[0][4]=color;
        break;
      case 31 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[0][4]=color;
        this->bloc[1][0]=color;
        this->bloc[1][4]=color;
        break;
      case 32 :
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][1]=color;
        this->bloc[2][0]=color;
        this->bloc[3][0]=color;
        break;
      case 33 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][2]=color;
        this->bloc[2][3]=color;
        this->bloc[3][3]=color;
        break;
      case 34 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        break;
      case 35 :
        this->bloc[0][0]=color;
        this->bloc[0][3]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        this->bloc[3][0]=color;
        this->bloc[3][3]=color;
        break;
      case 36 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[1][3]=color;
        this->bloc[2][0]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        this->bloc[2][3]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        this->bloc[3][2]=color;
        this->bloc[3][3]=color;
        break;
      case 37 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        this->bloc[3][2]=color;
        this->bloc[3][3]=color;
        break;
      case 38 :
        this->bloc[0][3]=color;
        this->bloc[1][3]=color;
        this->bloc[1][2]=color;
        this->bloc[2][2]=color;
        this->bloc[2][1]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        break;
      case 39 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[0][4]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[1][3]=color;
        this->bloc[2][2]=color;
        break;
      case 40 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        this->bloc[3][0]=color;
        this->bloc[4][0]=color;
        break;
      case 41 :
        this->bloc[0][3]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[1][3]=color;
        this->bloc[2][3]=color;
        break;
      case 42 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[0][4]=color;
        break;
      case 43 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        this->bloc[0][3]=color;
        this->bloc[1][3]=color;
        break;
      case 44 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        this->bloc[2][1]=color;
        this->bloc[3][1]=color;
        break;
      case 45 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        this->bloc[3][0]=color;
        this->bloc[3][1]=color;
        break;
      case 46 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][2]=color;
        break;
      case 47 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        this->bloc[2][1]=color;
        this->bloc[2][2]=color;
        break;
      case 48 :
        this->bloc[0][2]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][0]=color;
        break;
      case 49 :
        this->bloc[0][2]=color;
        this->bloc[0][1]=color;
        this->bloc[1][1]=color;
        this->bloc[2][0]=color;
        this->bloc[2][1]=color;
        break;
      case 50 :
        this->bloc[0][2]=color;
        this->bloc[1][1]=color;
        this->bloc[2][0]=color;
        break;
      case 51 :
        this->bloc[0][0]=color;
        this->bloc[1][1]=color;
        this->bloc[2][2]=color;
        break;
      case 52 :
        this->bloc[0][1]=color;
        this->bloc[1][0]=color;
        this->bloc[1][1]=color;
        this->bloc[1][2]=color;
        this->bloc[2][1]=color;
        break;
      case 53 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        this->bloc[2][0]=color;
        break;
      case 54 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        this->bloc[0][2]=color;
        break;
      case 55 :
        this->bloc[0][0]=color;
        this->bloc[1][0]=color;
        break;
      case 56 :
        this->bloc[0][0]=color;
        this->bloc[0][1]=color;
        break;
      default:
        cout<<"makeBloc ERROR"<<endl;
    }
  }

  /**
 * \fn bloc_findI()
 * \brief Cette fonction permet de trouver la coordonnée x de la case infèrieure gauche du bloc et de l'affecter à la variable i.
 *
 * \param void
 * \return void
 */

  void bloc_findI(void){
    int width;
    for(width=0; this->bloc[this->height-1][width]==0; width++){}
    this->i=width;
  }

  /**
 * \fn bloc_fillVoid()
 * \brief Cette fonction permet de remplir le bloc avec des 0.
 *
 * \param void
 * \return void
 */

  void bloc_fillVoid(void){
    int i, j;

    for(j=0; j<this->height; j++){
      for(i=0; i<this->width; i++){
          this->bloc[j][i]=0;
      }
    }
  }

};
