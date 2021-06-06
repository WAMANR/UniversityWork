/**
 * \file Tetris.hpp
 * \brief Gère tout le programme.
 * \author BONNEROT Thomas, ROUICHI Adil
 * \version 1.0
 * \date 31 mai 2020
 *
 * Contient tout les objets du jeu ainsi que les fonctions liées à son déroulement.
 *
 */
#include "Display.hpp"
#include <climits>
#include <unistd.h>
using namespace std;


class Tetris{

private:
  vector<int> easy{0,1,2,3,4,5,18,19,25,26,29,30,40,42,53,54,55,56};///< Cette variable est un vector contenant tous les ID des blocs faciles à insérer.
  vector<int> medium{6,7,8,9,10,11,12,13,14,15,16,17,28,31,43,44,45,46,47,48,49,50,51,52};///< Cette variable est un vector contenant tous les ID des blocs moyennement difficile à insérer.
  vector<int> hard{20,21,22,23,24,27,32,33,34,35,36,37,38,39,41};///< Cette variable est un vector contenant tous les ID des blocs difficile à insérer.
  vector<Bloc> vectorBloc;///<Cette variable est un vector de bloc qui correspond aux blocs suggérés.
  Board gameBoard;///<Cette variable correspond au plateau du jeu.
  int pickType;///<Cette variable correspond au type de blocs à selectionné pour la suggestion (1 : cercle, 2 : losange, 3 : triangle).
  bool possible;///<Cette variable permet de savoir si le joueur à choisit uniquement les blocs possibles à insérer ou non.
  bool difficultyIncrease;///<Cette variable permet de savoir si le joueur à choisit d'avoir une difficulté graduelle ou non.
  int gameMode;///<Cette variable permet de connaître le mode choisit par l'utilisateur (1 : normal, 2 : maxBlocBefore, 3 : survie).
  int difficulty=0;///<Cette variable permet de connaître la diffculté à l'état actuel du jeu (1 : easy, 2 : medium, 3 : hard).
  Display display;///<Cette variable correspond à un objet de type display gérant tout l'affichage du jeu.
  sf::Time gameTime;///<Cette variable de type time correspond à un temps utilisé pour la fonction time_game.
  sf::Clock clock;///<Cette variable de type clock correpond à une horloge utilisée pour la fonction time_game.

  //dancing dancing head
  sf::Time headTime;///<Cette variable de type time correspond à un temps utilisé pour la fonction dancing_head.
  sf::Clock headClock;///<Cette variable de type clock correspond à une horloge utilisée pour la fonction dancing_head.
  sf::Clock music;///<Cette variable de type clock correspond à une horloge utilisée pour la fonction music_manage.

  //introduction
  int tamtamAnim=0;///<Cette variable permet de choisir quelle sprite de l'animation d'introduction afficher.


public:

  /**
 * \fn introduction()
 * \brief Cette fonction gère et affiche l'introduction de lancement de jeu.
 *
 * \param void
 * \return void
 */
  void introduction(void){

    this->headTime=this->music.getElapsedTime();

    if(this->tamtamAnim==42){
      this->display.tamtamSound.play();
      this->display.intro=false;
      this->display.mainMenu=true;
    }

    else if(this->tamtamAnim==14 || this->tamtamAnim==27){
      this->display.tamtamSound.play();
      this->display.real=false;
    }

    else if(this->tamtamAnim==19 || this->tamtamAnim==36){
      this->display.tamtamSound.play();
      this->display.real=true;
    }

    if(this->headTime.asMilliseconds()>35){

      this->display.tamtam.sprite.setTextureRect(sf::IntRect(this->tamtamAnim*20,0,20,26));
      this->music.restart();
      this->tamtamAnim++;
    }
  }

  /**
 * \fn init_game()
 * \brief Cette fonction permet l'initialisation du jeu.
 *
 * \param int height, int width, int boardType
 * \return void
  */

  void init_game(int height, int width, int boardType){

    this->gameBoard=Board(boardType, height, width);

    switch(this->pickType){
      case 1:
        init_vectorBloc();
        break;
      case 2:
        fill_vectorBloc3();
      case 3:
        fill_vectorBloc3possible();
        break;
      default:
        cout<<"ERROR init_game"<<endl;
    }
  }

  /**
 * \fn find_in_vector()
 * \brief Cette fonction permet de savoir si l'ID d'un bloc éxiste dans un des trois vectors de difficulté.
 *
 * \param int x, vector<int>difficulty
 * \return bool
 */

  bool find_in_vector(int x, vector<int>difficulty){

    uint i;

    for(i=0 ; i<difficulty.size(); i++){

      if(x==difficulty[i])
        return 1;
    }
    return 0;
  }

  /**
 * \fn init_vectorBloc()
 * \brief Cette fonction permet de remplir le vector de bloc suggérés avec tous les blocs.
 *
 * \param void
 * \return bool
 */

  void init_vectorBloc(void){

    int i;
    this->vectorBloc.clear();

    for(i=0; i<20; i++){//bloc de base

      if(this->difficultyIncrease){
        if(!this->difficulty){
          if(!find_in_vector(i,this->easy)){
            if(!find_in_vector(i,this->medium)){
              if(!find_in_vector(i,this->hard))
                continue;
            }
          }
        }
        else if(this->difficulty==1){
          if(!find_in_vector(i,this->medium)){
            if(!find_in_vector(i,this->hard))
              continue;
          }
        }
        else if(this->difficulty==2){
          if(!find_in_vector(i,this->hard))
            continue;
        }
      }

      if(this->possible){
        if(test_blocPossible(Bloc(i)))
          this->vectorBloc.push_back(Bloc(i));
      }
      else
        this->vectorBloc.push_back(Bloc(i));
    }

    switch(this->gameBoard.boardType){

      case 1://bloc cercle
        for(i=20; i<32; i++){

          if(this->difficultyIncrease){
            if(!this->difficulty){
              if(!find_in_vector(i,this->easy)){
                if(!find_in_vector(i,this->medium)){
                  if(!find_in_vector(i,this->hard))
                    continue;
                }
              }
            }
            else if(this->difficulty==1){
              if(!find_in_vector(i,this->medium)){
                if(!find_in_vector(i,this->hard))
                  continue;
              }
            }
            else if(this->difficulty==2){
              if(!find_in_vector(i,this->hard))
                continue;
            }
          }

          if(this->possible){
            if(test_blocPossible(Bloc(i)))
              this->vectorBloc.push_back(Bloc(i));
          }
          else
            this->vectorBloc.push_back(Bloc(i));
        }
        break;

      case 2://bloc losange
        for(i=32; i<46; i++){

          if(this->difficultyIncrease){
            if(!this->difficulty){
              if(!find_in_vector(i,this->easy)){
                if(!find_in_vector(i,this->medium)){
                  if(!find_in_vector(i,this->hard))
                    continue;
                }
              }
            }
            else if(this->difficulty==1){
              if(!find_in_vector(i,this->medium)){
                if(!find_in_vector(i,this->hard))
                  continue;
              }
            }
            else if(this->difficulty==2){
              if(!find_in_vector(i,this->hard))
                continue;
            }
          }

          if(this->possible){
            if(test_blocPossible(Bloc(i)))
              this->vectorBloc.push_back(Bloc(i));
          }
          else
            this->vectorBloc.push_back(Bloc(i));
        }
        break;

      case 3://bloc triangle
        for(i=46; i<57; i++){

          if(this->difficultyIncrease){
            if(!this->difficulty){
              if(!find_in_vector(i,this->easy)){
                if(!find_in_vector(i,this->medium)){
                  if(!find_in_vector(i,this->hard))
                    continue;
                }
              }
            }
            else if(this->difficulty==1){
              if(!find_in_vector(i,this->medium)){
                if(!find_in_vector(i,this->hard))
                  continue;
              }
            }
            else if(this->difficulty==2){
              if(!find_in_vector(i,this->hard))
                continue;
            }
          }

          if(this->possible){
            if(test_blocPossible(Bloc(i)))
              this->vectorBloc.push_back(Bloc(i));
          }
          else
            this->vectorBloc.push_back(Bloc(i));
        }
        break;

      default:
        cout<<"ERROR init_vectorBloc"<<endl;
    }

  }

  /**
 * \fn fill_vectorBloc3()
 * \brief Cette fonction permet de remplir le vector de 3 blocs suggérés
 *
 * \param void
 * \return void
 */

  void fill_vectorBloc3(void){

    int choice;
    srand (time(NULL));

    while(vectorBloc.size()<3){

      choice = rand() % 2;

      if(choice==0){

        choice = rand() % 20;
      }

      if(choice==1){

        switch(this->gameBoard.boardType){

          case 1:
          choice = rand() % 13+19;
            break;
          case 2:
          choice = rand() % 15+31;
            break;
          case 3:
          choice = rand() % 12+45;
            break;
          default:
            cout<<"ERROR fill_vectorBloc3"<<endl;
            break;
        }
      }

      if(this->difficultyIncrease){
        if(!this->difficulty){
          if(!find_in_vector(choice,this->easy)){
            if(!find_in_vector(choice,this->medium)){
              if(!find_in_vector(choice,this->hard))
                continue;
            }
          }
        }
        else if(this->difficulty==1){
          if(!find_in_vector(choice,this->medium)){
            if(!find_in_vector(choice,this->hard))
              continue;
          }
        }
        else if(this->difficulty==2){
          if(!find_in_vector(choice,this->hard))
            continue;
        }
      }
      this->vectorBloc.push_back(Bloc(choice));

    }

  }

  /**
 * \fn fill_vectorBloc3possible()
 * \brief Cette fonction permet de remplir le vector de 3 blocs suggérés avec uniquement des blocs qui peuvent être insérés sur le plateau.
 *
 * \param void
 * \return void
 */

  void fill_vectorBloc3possible(void){

    vector<int>blocPossible;
    int i, choice;
    srand (time(NULL));

    for(i=0; (uint)i<this->vectorBloc.size(); i++){
      if(!test_blocPossible(this->vectorBloc[i])){
        this->vectorBloc.erase(this->vectorBloc.begin()+i);
        i--;
      }
    }

    for(i=0 ; i<57; i++){

      if(test_blocPossible(Bloc(i))){

        if(i<20)
          blocPossible.push_back(i);

        else{

          switch(this->gameBoard.boardType){

            case 1:
              if(i>19 && i<32)
                blocPossible.push_back(i);
              break;

            case 2:
              if(i>31 && i<46)
                blocPossible.push_back(i);
              break;

            case 3:
              if(i>45)
                blocPossible.push_back(i);
              break;

            default:
              cout<<"ERROR fill_vectorBloc3possible"<<endl;
              break;
          }
        }
      }
    }
    if(this->difficultyIncrease){

      for(i=0; (uint)i < blocPossible.size(); i++){
        if(blocPossible.size()==1)
          break;

        switch(this->difficulty){

          case 0:
            if(!find_in_vector(blocPossible[i],this->easy)){
              if(!find_in_vector(blocPossible[i],this->medium)){
                if(!find_in_vector(blocPossible[i],this->hard)){
                  blocPossible.erase(blocPossible.begin()+i);
                  i--;
                }
              }
            }
            break;
          case 1:
            if(!find_in_vector(blocPossible[i],this->medium)){
              if(!find_in_vector(blocPossible[i],this->hard)){
                blocPossible.erase(blocPossible.begin()+i);
                i--;
              }
            }
            break;
          case 2:
            if(!find_in_vector(blocPossible[i],this->hard)){
              blocPossible.erase(blocPossible.begin()+i);
              i--;
            }
            break;
          default:
            cout<<"ERROR fill_vectorBloc3possible"<<endl;
            break;
        }
      }
    }

    while(this->vectorBloc.size()<3){

      choice = rand()%blocPossible.size();
      choice = blocPossible[choice];
      this->vectorBloc.push_back(Bloc(choice));

    }
  }

  /**
 * \fn test_blocPossible()
 * \brief Cette fonction permet de tester si un bloc peut être inséré sur le plateau.
 *
 * \param Bloc bloc
 * \return bool
 */

  bool test_blocPossible(Bloc bloc){

    int i, j;

    for(j=0; j<this->gameBoard.boardH; j++){
      for(i=0; i<this->gameBoard.boardW; i++){

        if(test_and_place(0, bloc, i, j))
          return 1;
      }
    }
    return 0;
  }

  /**
 * \fn music_manage()
 * \brief Cette fonction gère la musique du jeu.
 *
 * \param void
 * \return void
 */

  void music_manage(void){

    if(this->display.end){
      this->display.gameTheme.stop();
      this->display.gameTheme.setLoop(false);
      this->display.gameLoop=false;
    }

    if(this->display.gameInitialised && this->display.menuLoop){
      this->display.menuTheme.stop();
      this->display.menuTheme.setLoop(false);
      this->display.menuLoop=false;
    }

    if(!this->display.gameInitialised && !this->display.menuLoop){
      this->display.menuTheme.play();
      this->display.menuTheme.setLoop(true);
      this->display.menuLoop=true;
    }

    if(this->display.gameInitialised && !this->display.gameLoop){
      this->display.gameTheme.play();
      this->music.restart();
      this->display.gameTheme.setLoop(true);
      this->display.gameLoop=true;

    }
  }

  /**
 * \fn dancing_head()
 * \brief Cette fonction gère la fonctionnalité "dancing head".
 *
 * \param void
 * \return void
 */

  void dancing_head(void){

    if(this->display.gameInitialised){

      this->headTime=this->music.getElapsedTime();

      if(this->headTime.asMilliseconds()>89600){
        this->music.restart();
      }

      if(this->headTime.asMilliseconds()<12800)
        this->display.boom=false;

      else if(this->headTime.asMilliseconds()>25300 && this->headTime.asMilliseconds()<27100)
        this->display.boom=false;

      else if(this->headTime.asMilliseconds()>38400 && this->headTime.asMilliseconds()<51500)
        this->display.boom=false;

      else if(this->headTime.asMilliseconds()>60800 && this->headTime.asMilliseconds()<64200)
        this->display.boom=false;

      else if(this->headTime.asMilliseconds()>76700 && this->headTime.asMilliseconds()<78300)
        this->display.boom=false;

      else if(this->headTime.asMilliseconds()>51500 && this->headTime.asMilliseconds()<60500){
        this->headTime=headClock.getElapsedTime();

        if(this->headTime.asMilliseconds()>800)
          headClock.restart();

        if(this->headTime.asMilliseconds()<400)
          this->display.boom=true;

        else
          this->display.boom=false;
      }


      else{
        this->headTime=headClock.getElapsedTime();

        if(this->headTime.asMilliseconds()>400)
          headClock.restart();

        if(this->headTime.asMilliseconds()<200)
          this->display.boom=true;

        else
          this->display.boom=false;
      }

    }
  }

  /**
 * \fn game()
 * \brief Cette fonction correspond à la boucle de jeu. Elle appelle toutes les autres fonctions par rapport à l'état du jeu.
 *
 * \param void
 * \return void
 */

  void game(void){

    this->display.init_window();


    while(this->display.mainWindow.isOpen()){

      if(this->display.intro){
        if(this->display.escape){
          this->display.intro=false;
          this->display.push=false;
          this->display.mainMenu=true;
        }
        introduction();
        this->display.event_manage();
        this->display.display(this->gameBoard, this->vectorBloc, this->display.gameInitialised);
      }

      else{

        this->display.event_manage();
        music_manage();
        if(this->display.head)
          dancing_head();

        this->display.display(this->gameBoard, this->vectorBloc, this->display.gameInitialised);

        if(this->display.game && this->gameMode==3){
          time_game();
          this->display.survival_end_game();
          this->display.push=true;
        }

        if(this->display.push){

          if(!this->display.gameInitialised){

            if(!this->display.game){
              this->display.menu();

              if(this->display.game){
                this->possible=this->display.blocPossible;
                this->difficultyIncrease=this->display.difficultyIncrease;
                this->gameMode=this->display.gameMode;
                this->pickType=this->display.pickType;
                init_game(this->display.boardH, this->display.boardW, this->display.boardType);
                this->display.boardH=this->gameBoard.boardH;
                this->display.boardW=this->gameBoard.boardW;
                this->display.selectedX=this->gameBoard.boardW/2;
                this->display.selectedY=this->gameBoard.boardH/2;
                this->display.gameInitialised=true;
                this->display.push=true;
                if(this->gameMode==3)
                  this->display.score=500;
              }
            }
          }

          else{

            if(!this->display.end){
              this->display.manage_game(this->vectorBloc, this->gameBoard);
              if(this->display.place){
                if(test_and_place(0, this->vectorBloc[this->display.piece],this->display.selectedX,this->display.selectedY)){//test if place
                  this->display.setBloc.play();
                  test_and_place(1, this->vectorBloc[this->display.piece],this->display.selectedX,this->display.selectedY);//place
                  if(this->gameMode==2)
                    this->display.blocNumber++;
                  ////////////////////Modify blocks//////////////////
                  if(this->pickType!=1){
                    this->vectorBloc.erase(this->vectorBloc.begin()+this->display.piece);
                    if(this->pickType==2){
                      if(!this->possible)
                        fill_vectorBloc3();
                      else
                        fill_vectorBloc3possible();
                    }
                  }
                  else
                    init_vectorBloc();
                  ///////////////////Modify blocks///////////////////
                  this->display.selectedPiece=false;

                }
                else{
                  this->display.errorBloc.play();
                  this->display.life--;
                }
                this->display.place=false;
                this->display.selectedPiece=false;
                }

              line_horizontal();
              line_vertical();

              this->display.scoreMultiplier=1;
              if(this->display.score>10000){
                this->difficulty=2;
                if(this->pickType==1){
                  init_vectorBloc();
                  this->display.selectedX=0;
                  this->display.selectedY=0;
                }
              }
              else if(this->display.score>5000){
                this->difficulty=1;
                if(this->pickType==1){
                  init_vectorBloc();
                  this->display.selectedX=0;
                  this->display.selectedY=0;
                }
              }
            }
            if(!this->display.end && this->gameMode!=3)
              this->display.end_game();
          }

          this->display.push=false;
        }
      }
    }
  }

  /**
 * \fn test_and_place()
 * \brief Cette fonction permet de vérifier si un bloc peut être posé à un endroit donné du plateau et le place sur le plateau en fonction d'un int (0 : test, 1 : place).
 *
 * \param int type, Bloc &bloc, int selectedX, int selectedY
 * \return bool
 */

  bool test_and_place(int type, Bloc &bloc, int selectedX, int selectedY){

    int i, j, x, y;
    y=selectedY-(bloc.height-1);//boardY

    for(j=0; j<bloc.height; j++, y++){
      for(i=0, x=selectedX-bloc.i; i<bloc.width; i++, x++){


        if(type){
          if(bloc.bloc[j][i]>0)
            this->gameBoard.board[y][x]=bloc.bloc[j][i];
        }

        else{
          if(bloc.bloc[j][i]>0){

            if(x<0 || y<0 || x>=this->display.boardW || y>=this->display.boardH){
              return 0;
            }

            else if(this->gameBoard.board[y][x]!=0){
              return 0;
            }
          }
        }
      }
    }
    return 1;
  }

  /**
 * \fn line_vertical()
 * \brief Cette fonction gère la destruction verticale des lignes.
 *
 * \param void
 * \return void
 */

  void line_vertical(void){

    int i, j;
    bool line;

    for(i=0; i<this->gameBoard.boardW; i++){
      line=true;
      for(j=0; j<this->gameBoard.boardH; j++){

        if(this->gameBoard.board[j][i]==0)
          line=false;
      }

      if(line){
        this->display.scoreMultiplier++;
        for(j=0; j<this->gameBoard.boardH; j++){

          this->headTime=this->music.getElapsedTime();
          if(this->headTime.asMilliseconds()>89600){
            this->music.restart();
          }

          if(this->gameBoard.board[j][i]>0){
            this->display.blocDestruction.play();
            this->gameBoard.board[j][i]=0;
            this->display.score+=1*this->display.scoreMultiplier;
            this->display.display(this->gameBoard, this->vectorBloc, this->display.gameInitialised);
            usleep(1);
          }
          this->display.lineDestruction.play();
        }
      }
    }
  }

  /**
 * \fn line_horizontal()
 * \brief Cette fonction gère la destruction horizontale des lignes.
 *
 * \param void
 * \return void
 */

  void line_horizontal(void){

    int i, j;
    bool line;

    for(j=0 ; j<this->gameBoard.boardH; j++){
      line=true;
      for(i=0 ; i<this->gameBoard.boardW; i++){

        if(this->gameBoard.board[j][i]==0)
          line=false;

      }

      if(line){
        this->display.scoreMultiplier++;

        for(i=0; i<this->gameBoard.boardW; i++){

          this->headTime=this->music.getElapsedTime();
          if(this->headTime.asMilliseconds()>89600){
            this->music.restart();
          }

          if(this->gameBoard.board[j][i]>0){
            this->display.blocDestruction.play();
            this->gameBoard.board[j][i]=0;
            this->display.score+=1*this->display.scoreMultiplier;
            this->display.display(this->gameBoard, this->vectorBloc, this->display.gameInitialised);
            usleep(1);
          }
        }
        this->display.lineDestruction.play();
        if(j>0)
          fall_line(j);
      }
    }
  }

  /**
 * \fn fall_line()
 * \brief Cette fonction fait tomber les pièces suite à une destruction horizontale.
 *
 * \param int j
 * \return void
 */

  void fall_line(int j){

    int i, jCopy;

    for(j--; j>=0; j--){
      for(i=0; i<this->gameBoard.boardW; i++){

        if(this->gameBoard.board[j][i]>0){
          for(jCopy=j; jCopy<this->gameBoard.boardH-1; jCopy++){

            if(this->gameBoard.board[jCopy+1][i]==0){
              this->gameBoard.board[jCopy+1][i]=this->gameBoard.board[jCopy][i];
              this->gameBoard.board[jCopy][i]=0;
            }
            else
              break;
          }
        }
      }
    }
    this->display.display(this->gameBoard, this->vectorBloc, this->display.gameInitialised);
    usleep(1);
    line_horizontal();
  }

  /**
 * \fn clock_secondsToMinutes()
 * \brief Cette fonction transforme les secondes en minutes quand celles-ci valent 60.
 *
 * \param void
 * \return void
 */

  void clock_secondsToMinutes(void){

    if(this->display.seconde%30==0)
      this->display.scorePerSec*=2;
    if(this->display.seconde==60){
      this->display.seconde=0;
      this->display.minute++;
    }
  }

  /**
 * \fn time_game()
 * \brief Cette fonction gère le temps de jeu pour le mode survie.
 *
 * \param void
 * \return void
 */

  void time_game(void){

    this->gameTime=this->clock.getElapsedTime();
    if(this->gameTime.asSeconds()>1){
      this->display.seconde++;
      clock_secondsToMinutes();
      this->clock.restart();
      this->display.score-=this->display.scorePerSec;
    }
  }

};
