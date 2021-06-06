/**
 * \file Sprite.hpp
 * \brief Gère la création de sprite.
 * \author BONNEROT Thomas, ROUICHI Adil
 * \version 1.0
 * \date 31 mai 2020
 *
 * Gère la création des sprites afin de créer des objets en fonction du nom donné et des dimensions de l'écran pour pouvoir les affichés dans le jeu.
 *
 */
#include <iostream>
using namespace std;

class Sprite{

private:
  sf::Texture texture;///<Cette variable correspond à la texture (l'image) affecté au sprite.

public:
  sf::Sprite sprite;///< Cette variable correspond au sprite de l'objet de type Sprite.


  /**
 * \fn Sprite()
 * \brief Cette fonction est le constructeur des objets de type Sprite.
 *
 * \param string name, int screenW, int screenH
 * \return void
 */
  Sprite(string name, int screenW, int screenH){
      if(!this->texture.loadFromFile("./ressources/images/" + name + ".png"))
        cout << "impossible charger" << endl;
      this->texture.setSmooth(true);
      this->sprite.setTexture(this->texture);

      if(name=="tetris"){
          this->sprite.setTextureRect(sf::IntRect(0,0,32,32));
          this->sprite.scale(6.0f, 6.0f);
          this->sprite.setPosition(sf::Vector2f(screenW/2.4,screenH/14));
      }
      if(name=="guide"){
        this->sprite.setTextureRect(sf::IntRect(0,0,200,32));
        this->sprite.scale(2.5f, 2.5f);
        this->sprite.setPosition(sf::Vector2f(screenW/3.2,screenH/6));
      }
      if(name=="phrase") {
        this->sprite.setTextureRect(sf::IntRect(0,0,100,64));
          this->sprite.scale(3.0f, 3.0f);
          this->sprite.setPosition(sf::Vector2f(screenW/2.6,screenH-60));
      }
      if(name=="settings") {
        this->sprite.setTextureRect(sf::IntRect(0,0,46,32));
        this->sprite.scale(6.2f, 6.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/2.6,screenH/30));
      }
      if(name=="good") {
        this->sprite.setTextureRect(sf::IntRect(0,0,58,32));
        this->sprite.scale(5.0f, 5.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/2.6,screenH-70));
      }
      if(name=="plus") {
        this->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        this->sprite.scale(9.0f, 9.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/1.3,screenH/5.1));
      }
      if(name=="moins") {
        this->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        this->sprite.scale(9.0f, 9.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/6.1,screenH/5.0));
      }
      if(name=="rules") {
        this->sprite.setTextureRect(sf::IntRect(0,0,32,32));
        this->sprite.scale(6.0f, 6.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/2.3,screenH/16));
      }
      if(name=="read"){
        this->sprite.setTextureRect(sf::IntRect(0,0,108,32));
        this->sprite.scale(2.0f, 2.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/2.4,screenH/7));
      }
      if(name=="exit") {
        this->sprite.setTextureRect(sf::IntRect(0,0,82,32));
        this->sprite.scale(5.0f, 6.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/2.75,screenH/16));
      }
      if(name=="back_rules"){
        this->sprite.setTextureRect(sf::IntRect(0,0,235,32));
        this->sprite.scale(3.0f, 3.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/4.5,screenH-120));
      }
      if(name=="thomasPixel"){
        this->sprite.setTextureRect(sf::IntRect(0,0,160,230));
        this->sprite.scale(1.0f, 1.0f);
        this->sprite.setPosition(sf::Vector2f(screenW-screenW/5,screenH/3));
      }
      if(name=="adilPixel"){
        this->sprite.setTextureRect(sf::IntRect(0,0,160,240));
        this->sprite.scale(1.0f, 1.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/20,screenH/3));
      }

      if(name=="adilReal"){
        this->sprite.setTextureRect(sf::IntRect(0,0,270,480));
        this->sprite.scale(1.0f, 1.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/80,screenH/4));
      }

      if(name=="thomasReal"){
        this->sprite.setTextureRect(sf::IntRect(0,0,270,480));
        this->sprite.scale(1.0f, 1.0f);
        this->sprite.setPosition(sf::Vector2f(screenW-screenW/4+screenW/50,screenH/11));
      }

      if(name=="tamtam"){
        this->texture.setSmooth(false);
        this->sprite.setTextureRect(sf::IntRect(0,0,20,26));
        this->sprite.scale(6.0f, 6.0f);
        this->sprite.setPosition(sf::Vector2f(screenW/2-screenW/24, screenH/2-screenH/18));
      }

    }
};
