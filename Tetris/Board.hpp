/**
 * \file Board.hpp
 * \brief Gère la création du plateau.
 * \author BONNEROT Thomas, ROUICHI Adil
 * \version 1.0
 * \date 31 mai 2020
 *
 * Gère la création du plateau en fonction de sa hauteur, de sa largeur et de sa forme afin de l'utiliser en jeu.
 *
 */
#include <iostream>
#include<cassert>
using namespace std;

class Board{

private:

public:
  int ** board;///<Cette variable correspond est un tableau d'entier correspondant au plateau.
  int boardH;///<Cette variable correspond à la hauteur du plateau.
  int boardW;///<Cette variable correspond à la largeur du plateau.
  int boardType;///<Cette variable correspond à la forme du plateau. (1 : cercle, 2 : losange, 3 : triangle).

  /**
 * \fn Board()
 * \brief Cette fonction est un constructeur utilisé uniquement pour la déclaration de la variable dans Tetris.hpp.
 *
 * \param void
 * \return void
 */
  Board(void){}

  /**
 * \fn Board()
 * \brief Cette fonction est le constructeur de l'objet de type Board.
 *
 * \param int boardType, int height, int width
 * \return void
 */

  Board(int boardType, int height, int width){
    this->boardType=boardType;

    switch(this->boardType){
      case 1:
        board_circle(height, width);
        break;
      case 2:
        board_losange(height,width);
        break;
      case 3:
        board_triangle(height, width);
        break;
      default:
        cout<<"ERROR in board constructor"<<endl;
    }
  }

  /**
  * \fn board_malloc()
  * \brief Cette fonction permet de malloc le plateau.
  *
  * \param void
  * \return void
  */

  void board_malloc(void){
    int i;

    this->board=(int**)malloc(this->boardH*sizeof(int*));
    assert(this->board);
    for(i=0; i<this->boardH; i++){
      this->board[i]=(int*)malloc(this->boardW*sizeof(int));
      assert(this->board[i]);
    }
  }

  /**
  * \fn board_circle()
  * \brief Cette fonction permet de créer un plateau de type cercle.
  *
  * \param int height, int width
  * \return void
  */

  void board_circle(int height, int width){

    int i, j, size;

    this->boardW=width;
    this->boardH=height;
    board_malloc();
    fill_board0();

    if(this->boardW<this->boardH)
      size=this->boardW/4;
    else
      size=this->boardH/4;

    for(j=0; j<size; j++){
      for(i=0; i<size-j; i++){
        this->board[j][i]=-1;
      }
    }

    for(i=0 ; i<size; i++){
      for(j=this->boardH-1; j>this->boardH-1-size+i; j--){
        this->board[j][i]=-1;
      }
    }

    for(j=0; j<size; j++){
      for(i=this->boardW-size+j; i<this->boardW; i++){
        this->board[j][i]=-1;
      }
    }

    for(i=0; i<size; i++){
      for(j=this->boardW-1; j>this->boardW-1-size+i; j--){
        this->board[this->boardH-1-i][j]=-1;
      }
    }
  }

  /**
  * \fn board_triangle()
  * \brief Cette fonction permet de créer un plateau de type triangle.
  *
  * \param int height, int width
  * \return void
  */

  void board_triangle(int height, int width){

    int i, j;

    if(height%2==0)
      height--;

    if(width%2==0)
      width--;

    if(height>width/2+1){
      while(height>width/2+1){
        height--;
      }
    }

    else{
      while(width/2+1>height){
        width--;
      }
    }

    this->boardW=width;
    this->boardH=height;
    board_malloc();
    fill_board0();

    for(j=0; j<this->boardH-1; j++){
      for(i=0; i<this->boardW/2-j; i++){
        this->board[j][i]=-1;
      }
    }

    for(j=0; j<this->boardH-1; j++){
      for(i=this->boardW/2+1+j; i<this->boardW; i++){
        this->board[j][i]=-1;
      }
    }
  }

  /**
  * \fn board_losange()
  * \brief Cette fonction permet de créer un plateau de type losange.
  *
  * \param int height, int width
  * \return void
  */

  void board_losange(int height, int width){

    int size, i, j;

    if(height<width)
      size=height;
    else
      size=width;

    if(size%2==0)
      size--;

    this->boardW=size;
    this->boardH=size;
    board_malloc();
    fill_board0();

    for(j=0; j<this->boardH-1; j++){
      for(i=0; i<this->boardW/2-j; i++){
        this->board[j][i]=-1;
      }
    }

    for(j=0; j<this->boardH-1; j++){
      for(i=this->boardW/2+1+j; i<this->boardW; i++){
        this->board[j][i]=-1;
      }
    }

    for(j=this->boardH/2; j<this->boardH; j++){
      for(i=0; i<j-this->boardH/2; i++){
        this->board[j][i]=-1;
      }
    }

    for(j=this->boardH-1; j>this->boardH/2; j--){
      for(i=this->boardW/2+(this->boardH-1-j)+1; i<this->boardW; i++){
        this->board[j][i]=-1;
      }
    }
  }

  /**
  * \fn fill_board0()
  * \brief Cette fonction permet de remplir le plateau avec des 0.
  *
  * \param void
  * \return void
  */

  void fill_board0(void){

    int i, j;

    for(j=0; j<this->boardH; j++){
      for(i=0; i<this->boardW; i++){
        this->board[j][i]=0;
      }
    }
  }

};
