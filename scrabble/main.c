#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "scrabble.h"

int main(){

////////////CONSTRUCTION PLATEAU//////////////

	//environnement
	case_t plateau[225];
	case_t hand[7];
	init_hand(hand);
	init_plateau(plateau);

	//Attribution type de case

	//mot triple
	set_type(0,0,4,plateau);
	set_type(7,0,4,plateau);
	set_type(14,0,4,plateau);
	set_type(0,14,4,plateau);
	set_type(7,14,4,plateau);
	set_type(14,14,4,plateau);
	//lettre doublée
	set_type(3,0,1,plateau);
	set_type(11,0,1,plateau);
	set_type(6,2,1,plateau);
	set_type(8,2,1,plateau);
	set_type(0,3,1,plateau);
	set_type(7,3,1,plateau);
	set_type(14,3,1,plateau);
	set_type(2,6,1,plateau);
	set_type(6,6,1,plateau);
	set_type(8,6,1,plateau);
	set_type(12,6,1,plateau);
	set_type(2,8,1,plateau);
	set_type(6,8,1,plateau);
	set_type(8,8,1,plateau);
	set_type(12,8,1,plateau);
	set_type(0,11,1,plateau);
	set_type(7,11,1,plateau);
	set_type(14,11,1,plateau);
	set_type(6,12,1,plateau);
	set_type(8,12,1,plateau);
	set_type(3,14,1,plateau);
	set_type(11,14,1,plateau);
	//mot doublé
	set_type(1,1,3,plateau);
	set_type(13,1,3,plateau);
	set_type(2,2,3,plateau);
	set_type(12,2,3,plateau);
	set_type(3,3,3,plateau);
	set_type(11,3,3,plateau);
	set_type(4,4,3,plateau);
	set_type(10,4,3,plateau);
	set_type(4,10,3,plateau);
	set_type(10,10,3,plateau);
	set_type(3,11,3,plateau);
	set_type(11,11,3,plateau);
	set_type(2,12,3,plateau);
	set_type(12,12,3,plateau);
	set_type(1,13,3,plateau);
	set_type(13,13,3,plateau);
	//lettre triple
	set_type(5,1,2,plateau);
	set_type(9,1,2,plateau);
	set_type(1,5,2,plateau);
	set_type(5,5,2,plateau);
	set_type(9,5,2,plateau);
	set_type(13,5,2,plateau);
	set_type(1,9,2,plateau);
	set_type(5,9,2,plateau);
	set_type(9,9,2,plateau);
	set_type(13,9,2,plateau);
	set_type(5,13,2,plateau);
	set_type(9,13,2,plateau);

///////////////////LETTRE PLATEAU///////////////////////

	/*ajout_lettre_plateau(9,0,'R',plateau);
	ajout_lettre_plateau(9,1,'E',plateau);
	ajout_lettre_plateau(9,2,'C',plateau);
	ajout_lettre_plateau(9,4,'P',plateau);
	ajout_lettre_plateau(9,5,'T',plateau);
	ajout_lettre_plateau(9,6,'A',plateau);
	ajout_lettre_plateau(9,4,'C',plateau);
	ajout_lettre_plateau(9,5,'L',plateau);
	ajout_lettre_plateau(9,6,'E',plateau);*/

	//ajout_lettre_plateau(0,3,'L',plateau);

	ajout_lettre_plateau(0,0,'S',plateau);
	ajout_lettre_plateau(0,1,'I',plateau);
	ajout_lettre_plateau(0,2,'F',plateau);
	ajout_lettre_plateau(0,4,'L',plateau);
	ajout_lettre_plateau(0,5,'E',plateau);
	ajout_lettre_plateau(0,6,'R',plateau);

	ajout_lettre_plateau(0,3,'F',plateau);
	ajout_lettre_plateau(1,3,'R',plateau);
	ajout_lettre_plateau(2,3,'A',plateau);
	ajout_lettre_plateau(3,3,'T',plateau);
	ajout_lettre_plateau(4,3,'E',plateau);
	ajout_lettre_plateau(5,3,'R',plateau);
	ajout_lettre_plateau(6,3,'N',plateau);
	ajout_lettre_plateau(7,3,'I',plateau);
	ajout_lettre_plateau(8,3,'S',plateau);
	ajout_lettre_plateau(9,3,'E',plateau);
	ajout_lettre_plateau(10,3,'R',plateau);
	ajout_lettre_plateau(11,3,'I',plateau);
	ajout_lettre_plateau(12,3,'O',plateau);
	ajout_lettre_plateau(13,3,'N',plateau);
	ajout_lettre_plateau(14,3,'S',plateau);


	ajout_lettre_plateau(1,6,'N',plateau);
	ajout_lettre_plateau(2,6,'U',plateau);
	ajout_lettre_plateau(3,6,'M',plateau);
	ajout_lettre_plateau(4,6,'E',plateau);
	ajout_lettre_plateau(5,6,'R',plateau);
	ajout_lettre_plateau(6,6,'O',plateau);
	ajout_lettre_plateau(7,6,'T',plateau);
	ajout_lettre_plateau(8,6,'A',plateau);
	ajout_lettre_plateau(9,6,'T',plateau);
	ajout_lettre_plateau(10,6,'I',plateau);
	ajout_lettre_plateau(11,6,'O',plateau);
	ajout_lettre_plateau(12,6,'N',plateau);
	ajout_lettre_plateau(13,6,'S',plateau);

	ajout_lettre_plateau(12,5,'U',plateau);

	ajout_lettre_plateau(1,11,'I',plateau);
	ajout_lettre_plateau(2,11,'M',plateau);
	ajout_lettre_plateau(3,11,'M',plateau);
	ajout_lettre_plateau(4,11,'E',plateau);
	ajout_lettre_plateau(5,11,'D',plateau);
	ajout_lettre_plateau(6,11,'I',plateau);
	ajout_lettre_plateau(7,11,'A',plateau);
	ajout_lettre_plateau(8,11,'T',plateau);
	ajout_lettre_plateau(9,11,'E',plateau);
	ajout_lettre_plateau(10,11,'M',plateau);
	ajout_lettre_plateau(11,11,'E',plateau);
	ajout_lettre_plateau(12,11,'N',plateau);
	ajout_lettre_plateau(13,11,'T',plateau);

	ajout_lettre_plateau(14,12,'R',plateau);
	ajout_lettre_plateau(14,13,'E',plateau);
	ajout_lettre_plateau(14,14,'S',plateau);
	ajout_lettre_plateau(14,11,'T',plateau);

	ajout_lettre_plateau(13,0,'S',plateau);
	ajout_lettre_plateau(14,0,'I',plateau);

	ajout_lettre_plateau(6,10,'R',plateau);
	ajout_lettre_plateau(6,9,'A',plateau);
	ajout_lettre_plateau(6,8,'P',plateau);

	ajout_lettre_plateau(0,14,'T',plateau);
	ajout_lettre_plateau(1,14,'E',plateau);

	ajout_lettre_plateau(4,4,'N',plateau);
	ajout_lettre_plateau(4,5,'C',plateau);
	ajout_lettre_plateau(4,7,'R',plateau);
	ajout_lettre_plateau(4,8,'C',plateau);
	ajout_lettre_plateau(4,9,'L',plateau);
	ajout_lettre_plateau(4,10,'E',plateau);

	ajout_lettre_plateau(9,0,'E',plateau);
	ajout_lettre_plateau(9,1,'N',plateau);
	ajout_lettre_plateau(9,2,'O',plateau);
	ajout_lettre_plateau(9,3,'R',plateau);
	ajout_lettre_plateau(9,4,'G',plateau);
	ajout_lettre_plateau(9,5,'U',plateau);
	ajout_lettre_plateau(9,6,'E',plateau);
	ajout_lettre_plateau(9,7,'I',plateau);
	ajout_lettre_plateau(9,8,'L',plateau);
	ajout_lettre_plateau(9,9,'L',plateau);
	ajout_lettre_plateau(9,10,'I',plateau);
	ajout_lettre_plateau(9,11,'R',plateau);
	ajout_lettre_plateau(9,12,'A',plateau);
	ajout_lettre_plateau(9,13,'I',plateau);
	ajout_lettre_plateau(9,14,'T',plateau);

	ajout_lettre_plateau(10,8,'A',plateau);

	ajout_lettre_plateau(10,13,'N',plateau);
	ajout_lettre_plateau(8,13,'T',plateau);
	ajout_lettre_plateau(7,13,'A',plateau);
	ajout_lettre_plateau(6,13,'M',plateau);
	





	





	ajout_lettre_hand(0, 'M', hand);
	ajout_lettre_hand(1, 'A', hand);
	ajout_lettre_hand(2, 'E', hand);
	ajout_lettre_hand(3, 'T', hand);
	ajout_lettre_hand(4, '?', hand);
	ajout_lettre_hand(5, 'N', hand);
	ajout_lettre_hand(6, 'R', hand);

	valeur_lettre_plateau(plateau);
	valeur_lettre_main(hand);

	//////////////////////////////////////////PRINT PLATEAU/////////////////////////////////////////

	if(0){
		print_plateau(plateau);
		print_hand(hand);	
	}
	affiche_plateau_entier(plateau,hand);



	///////////////////CREATION LISTE GRP LETTRE///////////////////

	dico_t liste = init_dico();
	dico_t listeTmp;

	//////////////////AFFECTATION GRP LETTRE A LISTE//////////////////

	liste = groupe_mot_plateau(plateau, liste);
	//print_dico(liste);//OPTIONNEL
	
	//Copie dicotxt dans dico avec tri groupes lettres sur plateau

	dico_t dico = init_dico();

	FILE *dicoTxt;
	dicoTxt = fopen("dic.txt","r");
	char mot[16];

	while(fscanf(dicoTxt, "%s", mot) != EOF){
		listeTmp=liste;

		while(listeTmp){
			if(contient(mot,listeTmp->mot->lettres)){
				dico=insertion_dico(dico, mot);
				break;
			}
			listeTmp=listeTmp->suivant;
		}
	}



	fclose(dicoTxt);
	//print_dico(dico);//OPTIONNEL

	//ALGORITHME COUP

	//LISTE PLATEAU//

	case_t plateauCopy[225];
	case_t handCopy[7];
	liste_coup_t listeCoup=init_liste_coup();
	
	//dico=insertion_dico(dico, "NEANT");


	algorithme_coup(dico, plateau, hand, listeCoup, plateauCopy, handCopy);

	//printf("%d\n", 224/15);




	return 0;
}