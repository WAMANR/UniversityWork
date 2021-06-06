#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "scrabble.h"


//////////////////////////////////////COUP///////////////////////////////////////////


liste_coup_t init_liste_coup(){
	return NULL;
}

int empty_liste_coup(liste_coup_t listeCoup){

	if(listeCoup == NULL){
		return 1;
	}
	return 0;
}

liste_coup_t insertion_liste_coup(liste_coup_t listeCoup, coup_t * coup){

	if(empty_liste_coup(listeCoup))
		return coup;

	coup_t * tmp;
	tmp=listeCoup;

	while(tmp->suivant != NULL){
		tmp=tmp->suivant;
	}

	tmp->suivant=coup;
	return listeCoup;
}



//////////////////////////////////////MOT COUP/////////////////////////////////////////////

liste_mot_coup_t init_liste_mot_coup(){
	return NULL;
}

int empty_liste_mot_coup(liste_mot_coup_t listeMotCoup){

	if(listeMotCoup == NULL){
		return 1;
	}
	return 0;
}

liste_mot_coup_t insertion_liste_mot_coup(liste_mot_coup_t listeMotCoup, mot_coup_t * motCoup){

	if(empty_liste_mot_coup(listeMotCoup))
		return motCoup;

	mot_coup_t * tmp;
	tmp=listeMotCoup;

	while(tmp->suivant != NULL){
		tmp=tmp->suivant;
	} 

	tmp->suivant=motCoup;
	return listeMotCoup;
}

void print_liste_mot_coup(liste_mot_coup_t listeMotCoup){

	int i;

	while(listeMotCoup){
		for(i=0; listeMotCoup->mot->lettres[i].lettre!=' ' && listeMotCoup->mot->lettres[i].lettre!='\0'; i++){
			printf("lettre : %c, position : %d, type : %d; score : %d\n", listeMotCoup->mot->lettres[i].lettre,listeMotCoup->mot->lettres[i].iPlateau, listeMotCoup->mot->lettres[i].type, listeMotCoup->mot->lettres[i].score);
		}
		printf("SCORE MOT : %d\n\n", listeMotCoup->scoreMot);
	listeMotCoup=listeMotCoup->suivant;

	}	
	printf("\n");
}

case_mot_t * creer_case_mot(char * mot, int iPlateau, int sens, case_t * plateau){//0 vertical / 1 horizontal

	int i;
	case_mot_t * newMot=malloc (sizeof(case_mot_t));
	
	newMot->n=nombre_lettre(mot);
	for(i=0; i<newMot->n; i++){
		if(plateau[iPlateau].lettre==' '){
			newMot->lettres[i].lettre=mot[i];
			newMot->lettres[i].iPlateau=iPlateau;
			newMot->lettres[i].type=plateau[i].type;
			newMot->lettres[i].score=return_valeur_lettre(mot[i]);
		}
		if(plateau[iPlateau].lettre!=' '){
			i--;
		}
		if(!sens)
			iPlateau+=15;
		else
			iPlateau++;
		
	}
	return newMot;
}

//////////////////////////////////////DICO/////////////////////////////////////////////////



dico_t init_dico(){
	return NULL;
}

int empty_dico(dico_t dico){

	if(dico == NULL){
		return 1;
	}
	return 0;
}


///////////////////////////////////////////MANIPULATION DICO////////////////////////////////////////////////////////////


mot_t * creer_mot(char * mot){

	int i;
	mot_t * newMot=malloc (sizeof(mot_t));
	
	newMot->n=nombre_lettre(mot);
	for(i=0; i<newMot->n; i++){
		newMot->lettres[i]=mot[i];
	}

	return newMot;
}


dico_t insertion_dico(dico_t dico, char * mot){

	node_t * nouveau = malloc(sizeof(node_t));
	assert(nouveau);
	nouveau->mot=creer_mot(mot);
	nouveau->suivant=NULL;

	if(empty_dico(dico))
		return nouveau;

	node_t * tmp;
	tmp=dico;

	while(tmp->suivant != NULL){
		tmp=tmp->suivant;
	} 

	tmp->suivant=nouveau;
	return dico;
}



void print_dico(dico_t dico){

	while(dico){
	printf("%s ",dico->mot->lettres);
	dico=dico->suivant;

	}	
	printf("\n");
}

	
///////////////////////////////////////CASE////////////////////////////////////////////////




///////////////FONCTIONS CASE/////////////////////

case_t * getCase(int x, int y, case_t * plateau){ //retourne adresse de la case (x,y)
	
	int position = x + 15*y;
	return &plateau[position];
}



void ajout_lettre_plateau(int x, int y, char lettre, case_t * plateau){//Ajoute lettre a case (x,y)

	getCase(x,y,plateau)->lettre=lettre;


}

void set_type(int x, int y, int type, case_t * plateau){ //Defini type de la case

	getCase(x,y,plateau)->type=type;
}

int return_valeur_lettre(char lettre){

	switch(lettre){

		case '?' : //JOKER
			return 0;
		case 'A' : case 'E' : case 'I' : case 'L' : case 'N' : case 'O' : case 'R' : case 'S' : case 'T' : case 'U' :
			return 1;
		case 'D' : case 'G' : case 'M' :
			return 2;
		case 'B' : case 'C' : case 'P' :
			return 3;
		case 'F' : case 'H' : case 'V' :
			return 4;
		case 'J' : case 'Q' :
			return 8;
		case 'K' : case 'W' : case 'X' : case 'Y' : case 'Z' :
			return 10;
	}
	return 0;
}


void valeur_lettre(int x, int y, case_t * plateau){//Affecte valeur pour une lettre

	char lettre;
	lettre=getCase(x,y,plateau)->lettre;

	switch(lettre){

		case '?' : //JOKER
			getCase(x,y,plateau)->score=0;
			break;
		case 'A' : case 'E' : case 'I' : case 'L' : case 'N' : case 'O' : case 'R' : case 'S' : case 'T' : case 'U' :
			getCase(x,y,plateau)->score=1;
			break;
		case 'D' : case 'G' : case 'M' :
			getCase(x,y,plateau)->score=2;
			break;
		case 'B' : case 'C' : case 'P' :
			getCase(x,y,plateau)->score=3;
			break;
		case 'F' : case 'H' : case 'V' :
			getCase(x,y,plateau)->score=4;
			break;
		case 'J' : case 'Q' :
			getCase(x,y,plateau)->score=8;
			break;
		case 'K' : case 'W' : case 'X' : case 'Y' : case 'Z' :
			getCase(x,y,plateau)->score=10;
			break;		

	}
	
}

void valeur_lettre_plateau(case_t * plateau){//Affecte valeur toutes lettres plateau
	int i;
	for (i=0; i<225; i++){

		if(plateau[i].lettre==' ')
			continue;

		valeur_lettre(i%15,i/15,plateau);
	}
}

void valeur_lettre_main(case_t * hand){//Affecte valeur toutes lettres hand
	int i;
	for (i=0; i<7; i++){

		if(hand[i].lettre==' ')
			continue;

		valeur_lettre(i,0,hand);
	}

}


//////////////////////////////////////PLATEAU////////////////////////////////////////////////


liste_plateau_t init_liste_plateau(){
	return NULL;
}

int empty_liste_plateau(liste_plateau_t listePlateau){

	if(listePlateau == NULL){
		return 1;
	}
	return 0;
}

void copy_plateau(case_t * plateau, case_t * plateauCopy){

	int i;
	for(i=0; i<225; i++){
		plateauCopy[i].iPlateau=plateau[i].iPlateau;
		plateauCopy[i].type=plateau[i].type;
		plateauCopy[i].lettre=plateau[i].lettre;
		plateauCopy[i].score=plateau[i].score;
	}
}


liste_plateau_t insertion_liste_plateau(liste_plateau_t listePlateau, case_t * plateau){

	int i;
	plateau_t * nouveau = malloc(sizeof(plateau_t));
	assert(nouveau);

	//COPY VALEUR PLATEAU
	for(i=0; i<225; i++){
		nouveau->plateau[i].iPlateau=plateau[i].iPlateau;
		nouveau->plateau[i].type=plateau[i].type;
		nouveau->plateau[i].lettre=plateau[i].lettre;
		nouveau->plateau[i].score=plateau[i].score;
	}
	nouveau->suivant=NULL;

	if(empty_liste_plateau(listePlateau)){
		return nouveau;
	}

	plateau_t * tmp;
	tmp=listePlateau;

	while(tmp->suivant != NULL){
		tmp=tmp->suivant;
	}

	tmp->suivant=nouveau;
	return listePlateau;
}


void affiche_liste_plateau(liste_plateau_t listePlateau){

	while(listePlateau){

		affiche_plateau(listePlateau->plateau);
		listePlateau=listePlateau->suivant;
	}
}


void affiche_plateau(case_t * plateau){//Affiche plateau sans main
	int x,y;

	for(x=1; x<=15; x++){
		printf("\t%d",x);
	}
	for(y=0;y<15;y++){

		printf("\n\n\n%c       ", y+65);

		for(x=0;x<15;x++){

			printf("%c\t",getCase(x,y,plateau)->lettre);

		}
	}

	printf("\n\n");


}




void init_plateau(case_t * plateau){ //init plateau + attribution coordonnées

	int i, x, y;
	i=0;

	for(y=0; y<15 ; y++){

		for (x=0 ; x<15 ; x++){

			plateau[i].iPlateau=x+y*15;
			plateau[i].type=0;
			plateau[i].lettre=' ';
			plateau[i].score=0;
			i++;


		
		}

	}
}

void print_plateau(case_t * plateau){ //affiche toutes cases ligne par ligne

	int i;
	printf("/////////////PLATEAU////////////\n");
	for(i=0; i<225; i++){
		printf("case %d : i=%d type=%d lettre=%c score=%d\n", i, plateau[i].iPlateau, plateau[i].type, plateau[i].lettre, plateau[i].score);
	}
}

void affiche_plateau_entier(case_t * plateau, case_t * hand){//Affiche plateau de manière propre
	int x,y;

	printf("        ");

	for(x=0; x<15; x++){
		printf("\t%d",x);
	}
	for(y=0;y<15;y++){

		printf("\n\n\n%d(%d)           ", y, y*15);

		for(x=0;x<15;x++){

			printf("%c\t",getCase(x,y,plateau)->lettre);

		}
	}

	printf("\n\n\n\n\n\t\t\t\t\t\t MAIN JOUEUR :");

	for(x=0;x<7;x++){
		printf("\t%c",getCase(x,0,hand)->lettre);
	}

	printf("\n\n");


}


////////////////////////////////////MAIN DU JOUEUR/////////////////////////////////////////

void init_hand(case_t * hand){ //init main
	int i, x;
	i=0;

	for (x=0 ; x<7 ; x++){
			
		hand[i].iPlateau=x;
		hand[i].type=0;
		hand[i].lettre=' ';
		hand[i].score=0;
		i++;		

	}
}

void ajout_lettre_hand(int x, char lettre, case_t * hand){//Ajoute lettre main au rang x

	getCase(x,0,hand)->lettre=lettre;

}

int effacer_lettre_hand(case_t * hand, char lettre){

	int i;
	for(i=0; i<7; i++){
		if(hand[i].lettre==lettre){
			hand[i].lettre=' ';
			return 1;
		}
	}
	return 0;
}

int effacer_joker_hand(case_t * hand){

	int i;
	for(i=0; i<7; i++){
		if(hand[i].lettre=='?'){
			hand[i].lettre=' ';
			return 1;
		}
	}
	return 0;
}

void copy_hand(case_t * hand, case_t * handCopy){

	int i;
	for(i=0; i<7; i++){
		handCopy[i].lettre=hand[i].lettre;
	}
}

int lettre_dans_main(case_t * hand, char lettre){

	int i;
	for (i=0; i<7; i++){

		if(hand[i].lettre=='?')
			return 1;

		else if(hand[i].lettre==lettre)
			return 1;
	}
	return 0;

}

int main_pleine(case_t * hand){

	int i;
	for(i=0; i<7; i++){

		if(hand[i].lettre==' ')
			return 0;
	}
	return 1;
}

int main_vide(case_t * hand){

	int i;
	for(i=0; i<7; i++){

		if(hand[i].lettre!=' ')
			return 0;
	}
	return 1;
}

void affiche_hand(case_t * hand){

	int i;

	printf("\n\n\n\n\n\t\t\t\t\t\t MAIN JOUEUR :");

		for(i=0;i<7;i++){
			printf("\t%c",getCase(i,0,hand)->lettre);
		}	
		printf("\n");
}


void print_hand(case_t * hand){ //affiche toutes cases de la main

	int i;
	printf("/////////////MAIN////////////\n");
	for(i=0; i<7; i++){
		printf("case %d : iPlateau=%d type=%d lettre=%c score=%d\n", i, hand[i].iPlateau, hand[i].type, hand[i].lettre, hand[i].score);
	}
}

int trou_main(case_t * hand){

	int i;
	for (i=0; i<7; i++){
		if (hand[i].lettre==' ')
			return i;
	}
	return -1;
}

//////////////////////////////////////ALGORIRTHME////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////OUTILS COMPARAISONS MOTS (LISTE OU MOT A MOT)///////////////////////////////////////////////////////////////

int nombre_lettre(char * mot){
	int i;
	for(i=0; mot[i]!=' ' && mot[i] != '\0'; i++){
	}
	return i;
}


int est_dans(char * mot, dico_t liste){//mot exact présent dans liste

	while(liste){

		if( mot_egal(mot,liste->mot->lettres)==1)
			return 1;

		liste=liste->suivant;
	}
	return 0;
}


int mot_egal(char * mot1, char * mot2){//"est_dans_aux" deux mots sont ils egaux?

	int NbLettreMot1, NbLettreMot2, gate, i;
	NbLettreMot1=nombre_lettre(mot1);
	NbLettreMot2=nombre_lettre(mot2);
	gate=1;

	if(NbLettreMot2==NbLettreMot1){

		for(i=0; mot1[i]!= ' ' && mot1[i]!= '\0'; i++){

			if(mot1[i]=='?' || mot2[i]=='?');

			else if(mot1[i]!=mot2[i])
				gate=0;
		}

		if(gate)
			return 1;
	}
	return 0;
}


int appartient(char * mot, dico_t liste){ //lettres présentes dans les mots de la liste
	
	while(liste){

		if (contient(liste->mot->lettres, mot)==1)
			return 1;
		
		liste=liste->suivant;
	}
	return 0;
}

int contient(char * motContenant , char * motContenu){//"appartient_aux" est ce qu'un mot est contenu dans un autre?

	int i, n, iCopy, gate;
	n=0;
	gate=0;

	for(i=0; i<=16-nombre_lettre(motContenu) && motContenant[i]!='\0' && motContenant[i]!=' '; i++){

		if(motContenant[i]==motContenu[n] || motContenant[i]=='?' || motContenu[n]=='?'){
			iCopy=i;
			while(motContenu[n] != '\0' && motContenu[n]!= ' '){

				if(motContenu[n]=='?' || motContenant[iCopy]=='?');

				else if(motContenu[n]!=motContenant[iCopy]){
					gate=1;
					break;
				}
				iCopy++;
				n++;
			}
			if(!gate)
				return 1;
		}
	}
	return 0;
}

int existe(char * mot){

	FILE * dicoTxt;
	dicoTxt = fopen("dic.txt","r");
	char motDico[16];
	while(fscanf(dicoTxt,"%s",motDico)!=EOF){
		if(mot_egal(motDico,mot)){
			fclose(dicoTxt);
			return 1;
		}
	}
	fclose(dicoTxt);
	return 0;
}

int possible_en_dessous(char * mot, char * motEnDessous, int iMot, int iDessous){//optimisation

	for(; mot[iMot]!=' ' && mot[iMot]!= '\0'; iMot++){
		if(mot[iMot]!=mot[iDessous])
			return 0;
	}
	return 1;
}

int possible_a_droite(char * mot, char * motDroite, int iMot, int iDroite){//optimisation

	for(; mot[iMot]!=' ' && mot[iMot]!= '\0'; iMot++){
		if(mot[iMot]!=mot[iDroite])
			return 0;
	}
	return 1;
}

int espace_up(int i){
	int espace=0;
	while(i>15){
		espace++;
		i-=15;
	}
	return espace;
}

int espace_down(int i){
	int espace=0;
	while(i<210){
		espace++;
		i-=15;
	}
	return espace;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char * recuperation_verticale(case_t * plateau, int i, char * mot){

	int x, y, n;
	x=i%15;
	y=i/15;

	for(; y>0; y--){

		if(getCase(x,y-1,plateau)->lettre == ' '){
			break;
		}
	}

	n=0;

	for(; y<15; y++){

		mot[n] = getCase(x,y,plateau)->lettre;
		n=n+1;

		if(getCase(x,y+1,plateau)->lettre == ' ')
			break;
	}

	for(; n<15 ; n++){
		mot[n]=' ';
	}

	return mot;
}

char * recuperation_horizontale(case_t * plateau, int i, char * mot){

	int x, y, n;
	x=i%15;
	y=i/15;

	for(; x!=14; x--){

		if(getCase(x-1,y,plateau)->lettre == ' ')
			break;
	}

	n=0;

	for(; x<15; x++){

		mot[n] = getCase(x,y,plateau)->lettre;
		n=n+1;

		if(getCase(x+1,y,plateau)->lettre == ' ')
			break;
	}

	for(; n<15 ; n++){
		mot[n]=' ';
	}

	return mot;
}

int recuperation_lettre_up(case_t * plateau, int i){
	int x, y;
	x=i%15;
	y=i/15;

	for(; y>0; y--){
		if(getCase(x,y-1,plateau)->lettre==' ')
			break;
	}

	return y*15+x;
}

int recuperation_lettre_left(case_t * plateau, int i){
	int x, y;
	x=i%15;
	y=i/15;

	for(; x!=14; x--){
		if(getCase(x-1,y,plateau)->lettre==' ')
			break;
	}
	return y*15+x;
}

int occurence_lettre_mot(char * mot, char lettre){
	int i, occurence;
	occurence=0;
	
	for (i=0; mot[i]!= ' ' && mot[i]!='\0'; i++){

		if(mot[i]=='?' || lettre=='?')
			occurence++;

		else if(mot[i]==lettre)
			occurence++;
	}
	return occurence;
}

int position_lettre_mot(char lettre, char * mot, int n){

	int i;

	for(i=0; mot[i]!= '\0' && mot[i]!=' '; i++){

		if(mot[i]=='?' || lettre=='?')
			n--;

		else if(mot[i]==lettre)
			n--;

		if(n==0)
			return i;
	}
	return 0;
}




dico_t groupe_mot_plateau(case_t * plateau, dico_t liste){//CREER UNE LISTE DE CHAQUE GROUPES DE LETTRES PRESENT SUR LE PLATEAU QUE CE SOIT VERTICALEMENT OU HORIZONTALEMENT(omet les occurences multiples)

	int i;
	char mot[16];

	for(i=0; i<225; i++){//GROUPE LETTRES VERTICALE

		if(plateau[i].lettre!=' '){

			recuperation_verticale(plateau,i,mot);

			if(est_dans(mot,liste)==0)
				liste = insertion_dico(liste, mot);
		}

	}

	for(i=0; i<225; i++){

		if(plateau[i].lettre!=' '){

			recuperation_horizontale(plateau,i,mot);

			if(est_dans(mot,liste)==0)
				liste=insertion_dico(liste,mot);
		}
	}

	return liste;
}


void algorithme_coup(dico_t dico, case_t * plateau, case_t * hand, liste_coup_t listeCoup, case_t * plateauCopy, case_t * handCopy){

	int i, iPlateau, positionLettre, occurenceMot, iMot, iParcoursMot, choixParcours, iPlateauCopy, exit, copyYetHand, iParcoursMotVertical, iParcoursMotHorizontal, iPlateauCopyVertical, iPlateauCopyHorizontal;
	char mot[16], motExiste[16];
	dico_t dicoCopy;


	/////////////////////////////////////////////////////////////////PARCOURS VERTICAL///////////////////////////////////////////////////////////////

	for(i=0; i<225; i++){//BOUCLE TOUTES LES CASES

		dicoCopy=dico;

		if(plateau[i].lettre!=' ' && (i<15 || plateau[i-15].lettre==' ')){//CASE PREMIERE LETTRE MOT HAUTEUR?

			recuperation_verticale(plateau,i,mot);

			while(dicoCopy){////////////////////////////CHAQUE MOT DICO//////////////////////////////////////////////////

				if(contient(dicoCopy->mot->lettres,mot)){/////////////////////////////CONTIENT MOT///////////////////////////////

					positionLettre=1;
					occurenceMot=occurence_lettre_mot(dicoCopy->mot->lettres,plateau[i].lettre);

					while(occurenceMot){/////////////////////////////////////////////OCCURENCE MOT////////////////////////////////

						exit=0;
						copyYetHand=0;
						iPlateau=i;
						iMot=position_lettre_mot(plateau[iPlateau].lettre,dicoCopy->mot->lettres,positionLettre);

						if(iMot==0)
							choixParcours=0; //descendant uniquement

						else if(dicoCopy->mot->lettres[iMot+1]==' ' || dicoCopy->mot->lettres[iMot+1]=='\0')
							choixParcours=1; //montant uniquement

						else
							choixParcours=2; //montant->descendant

						//printf("Mot : %s, occurence : %d, iMot : %d, iPlateau : %d, choixParcours : %d\n", dicoCopy->mot->lettres, occurenceMot, iMot, iPlateau, choixParcours);
						iParcoursMot=iMot;
						iPlateauCopy=iPlateau;

						if(choixParcours!=0){////////////////////////PARCOURS MONTANT///////////////////////////////////////////////

							while(iParcoursMot){/////////////////////TRI MONTANT////////////////////////////////////////////////////

								iParcoursMot--;
								iPlateauCopy-=15;

								//printf("(%c,%d) : %d\n", dicoCopy->mot->lettres[iParcoursMot], iParcoursMot, iPlateauCopy);

								if(iPlateauCopy<0){
									exit=1;
									break;
								}

								if((plateau[iPlateauCopy].lettre!=dicoCopy->mot->lettres[iParcoursMot]) && plateau[iPlateauCopy].lettre!=' '){
									exit=1;
									break;
								}

								if(plateau[iPlateauCopy].lettre==' '){

									if(!copyYetHand){
										copy_hand(hand,handCopy);
										copyYetHand=1;
									}

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==0){
										if((effacer_joker_hand(handCopy))==0){
											exit=1;
											break;
										}
									}
								}
							}/////////////////////////////////////////TRI MONTANT///////////////////////////////////////////////////

						}////////////////////////////////////////////PARCOURS MONTANT///////////////////////////////////////////////

						iParcoursMot=iMot;
						iPlateauCopy=iPlateau;

						if(exit==0){
							//printf("position : %d MOT(%d) : %s rentre hauteur\n", i, iMot ,dicoCopy->mot->lettres);
						}

						if(choixParcours!=1 && exit==0){////////////PARCOURS DESCENDANT/////////////////////////////////////////////

							while(dicoCopy->mot->lettres[iParcoursMot+1]!=' ' && dicoCopy->mot->lettres[iParcoursMot+1]!='\0'){//////////////TRI DESCENDANT//////////////

								iParcoursMot++;
								iPlateauCopy+=15;

								if(iPlateauCopy/15>14){
									exit=1;
									break;
								}

								if((plateau[iPlateauCopy].lettre!=dicoCopy->mot->lettres[iParcoursMot]) && plateau[iPlateauCopy].lettre!=' '){
									exit=1;
									break;
								}

								if(plateau[iPlateauCopy].lettre==' '){

									if(!copyYetHand){
										copy_hand(hand,handCopy);
										copyYetHand=1;
									}

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==0){
										if((effacer_joker_hand(handCopy))==0){
											exit=1;
											break;
										}
									}
								}

							}////////////////////////////////////////////////////////////////////////////////////////////////////////////////TRI DESCENDANT//////////////


						}///////////////////////////////////////////PARCOURS DESCENDANT/////////////////////////////////////////////

						if(!copyYetHand)
							exit=1;

						if(exit==0){
							//printf("position : %d MOT(%d) : %s rentre haut bas\n", i, iMot ,dicoCopy->mot->lettres);
							//affiche_hand(handCopy);
						}

						if(exit==0){
							iParcoursMot=iMot;
							iPlateauCopy=iPlateau;
							copy_hand(hand,handCopy);
							copy_plateau(plateau,plateauCopy);
						}
						
						//////////////////////////////////////////////////////////////////////////////////////////////////////////PLACE PLATEAU///////////////

						if(choixParcours!=0 && exit==0){////////////////////PLACE MONTANT

							while(iParcoursMot){

								iParcoursMot--;
								iPlateauCopy-=15;

								if(plateauCopy[iPlateauCopy].lettre==' '){

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==1){
										plateauCopy[iPlateauCopy].lettre=dicoCopy->mot->lettres[iParcoursMot];
									}
									else if(effacer_joker_hand(handCopy)==0){
										exit=1;
										break;												
									}

									else
										plateauCopy[iPlateauCopy].lettre='?';


									if(!iParcoursMot){

										if(iPlateauCopy>14){

											if(plateauCopy[iPlateauCopy-15].lettre!=' '){
												recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

												if(!existe(motExiste)){

													if(effacer_joker_hand(handCopy)==1){

															handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
															plateauCopy[iPlateauCopy].lettre='?';
															recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

															if(!existe(motExiste)){
																exit=1;
																break;
															}
													}
													else{
														exit=1;
														break;
													}
												}
											}
										}
									}

									if( (iPlateauCopy%15!=0 && plateauCopy[iPlateauCopy-1].lettre!=' ') || (iPlateauCopy%15!=14 && plateauCopy[iPlateauCopy+1].lettre!=' ') ){

										recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

										if(!existe(motExiste)){

											if(effacer_joker_hand(handCopy)==1){

												handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
												plateauCopy[iPlateauCopy].lettre='?';
												recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

												if(!existe(motExiste)){
													exit=1;
													break;
												}
											}
											else{
												exit=1;
												break;
											}
										}
									}		
								}								
							}

						}////////////////////////////////////////PLACE MONTANT

						iParcoursMot=iMot;
						iPlateauCopy=iPlateau;

						if(choixParcours!=1 && exit==0){////////////////////PLACE DESCENDANT

							while(dicoCopy->mot->lettres[iParcoursMot+1]!=' ' && dicoCopy->mot->lettres[iParcoursMot+1]!='\0'){

								iParcoursMot++;
								iPlateauCopy+=15;

								if(plateauCopy[iPlateauCopy].lettre==' '){

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==1){
										plateauCopy[iPlateauCopy].lettre=dicoCopy->mot->lettres[iParcoursMot];
									}
									else if(effacer_joker_hand(handCopy)==0){
										exit=1;
										break;												
									}

									else
										plateauCopy[iPlateauCopy].lettre='?';

									if(!(dicoCopy->mot->lettres[iParcoursMot+1]!=' ' && dicoCopy->mot->lettres[iParcoursMot+1]!='\0')){

										

										if(plateauCopy[iPlateauCopy+15].lettre!=' '){
											recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

											if(!existe(motExiste)){

												if(effacer_joker_hand(handCopy)==1){

													handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
													plateauCopy[iPlateauCopy].lettre='?';
													recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

													if(!existe(motExiste)){
														exit=1;
														break;
													}
												}
												else{
													exit=1;
													break;
												}
											}
										}
										
									}

									if( (iPlateauCopy%15!=0 && plateauCopy[iPlateauCopy-1].lettre!=' ') || (iPlateauCopy%15!=14 && plateauCopy[iPlateauCopy+1].lettre!=' ') ){

										recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

										if(!existe(motExiste)){

											if(effacer_joker_hand(handCopy)==1){

												handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
												plateauCopy[iPlateauCopy].lettre='?';
												recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

												if(!existe(motExiste)){
													exit=1;
													break;
												}
											}
											else{
												exit=1;
												break;
											}
										}
									}		
								}								
							}

						}////////////////////////////////////////PLACE DESCENDANT

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////PLACE PLATEAU///////////////

						if(exit==0){
							affiche_plateau_entier(plateauCopy,handCopy);
						}

						/*if(exit==0){/////////////////////////////////////////////////STOCK COUP//////////////////////////////////////////////////////

							coup_t * newCoup=malloc(sizeof(coup_t));
							assert(newCoup);
							liste_mot_coup_t listeMotCoup=init_liste_mot_coup();
							iPlateauCopyVertical=recuperation_lettre_up(plateauCopy, i);
							iParcoursMotVertical=0;
							//printf("position : %d MOT(%d): %s iParcoursMot : %d nombrelettre : %d\n", i, iMot ,dicoCopy->mot->lettres, iParcoursMotVertical, nombre_lettre(dicoCopy->mot->lettres));

							while(iParcoursMotVertical<nombre_lettre(dicoCopy->mot->lettres)){

								if(plateau[iPlateauCopyVertical].lettre==' '){
									//printf("NOUVELLE LETTRE / iPlateauCopy : %d\n", iPlateauCopyVertical);
									mot_coup_t * newMotCoup=malloc(sizeof(mot_coup_t));
									assert(newMotCoup);
									newMotCoup->suivant=NULL;
									newMotCoup->scoreMot=0;
									iParcoursMotHorizontal=0;
									iPlateauCopyHorizontal=recuperation_lettre_left(plateauCopy,iPlateauCopyVertical);

									recuperation_horizontale(plateauCopy,iPlateauCopyVertical,motExiste);
								//	printf("MOT CORRESPONDANT : %s\n", motExiste);

									case_mot_t * newMot=malloc(sizeof(case_mot_t));
									assert(newMot);
									newMot->n=0;

									while(iParcoursMotHorizontal<nombre_lettre(motExiste)){

										//printf("IPARCOURS HORIZONTAL : %d\n", iParcoursMotHorizontal);
										

										newMot->lettres[iParcoursMotHorizontal].lettre=plateauCopy[iPlateauCopyHorizontal].lettre;
										//printf("iPlateauCopy : %d, lettre stock : %c\n", iPlateauCopyHorizontal,newMot->lettres[iParcoursMotHorizontal].lettre);


										iParcoursMotHorizontal++;
										iPlateauCopyHorizontal++;
									}


									

								}




								//printf("iparcoursMot : %d / iPlateauCopy : %d\n", iParcoursMot, iPlateauCopy);

								iParcoursMotVertical++;
								iPlateauCopyVertical+=15;
							}
							
							













						}////////////////////////////////////////////////////////////STOCK COUP//////////////////////////////////////////////////////*/


						positionLettre++;
						occurenceMot--;

					}////////////////////////////////////////////////////////////////OCCURENCE MOT///////////////////////////////
				
				}/////////////////////////////////////////////////////////////////////CONTIENT MOT///////////////////////////////

				dicoCopy=dicoCopy->suivant;
			}///////////////////////////////////////////CHAQUE MOT DICO//////////////////////////////////////////////////
			
		}
	}////////////////////////////////////////////////////////////////FIN PARCOURS VERTICAL///////////////////////////////////////////////////////////





	/*if(exit==0){

							coup_t * newCoup=malloc(sizeof(coup_t));
							liste_mot_coup_t listeMotCoup=init_liste_mot_coup();
							assert(newCoup);
							iDescend=recuperation_lettre_up(plateauCopy,iPlateau);
							iParcoursMot=0;
							premierTour=0;
							

							while(iParcoursMot<nombre_lettre(dico->mot->lettres)){

								printf("iDescend (%c,%d)\n", plateauCopy[iDescend].lettre, iDescend/15);
								
								if(!premierTour){

									mot_coup_t * newMotCoup=malloc(sizeof(mot_coup_t));
									assert(newMotCoup);
									newMotCoup->suivant=NULL;
									newMotCoup->scoreMot=0;

									recuperation_verticale(plateauCopy,iPlateau,motSave);

									case_mot_t * newMot=malloc(sizeof(case_mot_t));
									assert(newMot);
									newMot->n=nombre_lettre(motSave);
									//for(n=0; n<)


									






									premierTour=1;
								}

								else{

									if(plateau[iPlateauCopy].lettre==' '){

										mot_coup_t * newMotCoup=malloc(sizeof(mot_coup_t));
										assert(newMotCoup);
										newMotCoup->suivant=NULL;
										newMotCoup->scoreMot=0;

								

									}
								}






								iDescend+=15;
								iParcoursMot++;

							}








						}

						if(exit==0){

							
						

							coup_t * newCoup=malloc(sizeof(coup_t));
							assert(newCoup);



							//debut liste mot coup

							liste_mot_coup_t listeMotCoup=init_liste_mot_coup();

							//tant que autre mot coup

							mot_coup_t * newMotCoup=malloc(sizeof(mot_coup_t));
							assert(newMotCoup);
							newMotCoup->mot=creer_case_mot(dicoCopy->mot->lettres, iPlateau, 0, plateau);
							newMotCoup->suivant=NULL;
							newMotCoup->scoreMot=0;

							//CALCUL SCORE

							//boucle addition score lettre
							for(a=0; newMotCoup->mot->lettres[a].lettre!=' ' && newMotCoup->mot->lettres[a].lettre!='\0'; a++){
								if(newMotCoup->mot->lettres[a].type==1){
									newMotCoup->scoreMot+=2*newMotCoup->mot->lettres[a].score;
								}	
								else if(newMotCoup->mot->lettres[a].type==2){
									newMotCoup->scoreMot+=3*newMotCoup->mot->lettres[a].score;
								}
								else{
									newMotCoup->scoreMot+=newMotCoup->mot->lettres[a].score;
								}
							}		

							//boucle multiplication mot compte double et triple
							for(a=0; newMotCoup->mot->lettres[a].lettre!=' ' && newMotCoup->mot->lettres[a].lettre!='\0'; a++){
								if(newMotCoup->mot->lettres[a].type==3){
									newMotCoup->scoreMot*=2;
								}
								if(newMotCoup->mot->lettres[a].type==4){
									newMotCoup->scoreMot*=3;
								}
		
							}

							//FIN CALCUL SCORE

							listeMotCoup=insertion_liste_mot_coup(listeMotCoup, newMotCoup);

							//fin tant que
							//fin liste mot coup

							newCoup->mots=listeMotCoup;
							if (main_vide(hand)){
								newCoup->scrabble=1;
							    newCoup->scoreCoup=50;
							}

							else{
								newCoup->scrabble=0;
							    newCoup->scoreCoup=0;
							}

							mot_coup_t * listeMotCoupCopy=listeMotCoup;
							while(listeMotCoupCopy){

								newCoup->scoreCoup+=listeMotCoupCopy->scoreMot;
								listeMotCoupCopy=listeMotCoupCopy->suivant;
							}

							listeCoup=insertion_liste_coup(listeCoup, newCoup);

						}

						///////////////////////////////////////////////FIN STOCK COUP////////////////////////////////////////////////////////////////////////





























	/////////////////////////////////////////////////////////////////PARCOURS HORIZONTAL///////////////////////////////////////////////////////////////

	/*for(i=0; i<225; i++){//BOUCLE TOUTES LES CASES

		dicoCopy=dico;

		if(plateau[i].lettre!=' ' && (i%15==0 || plateau[i-1].lettre==' ')){//CASE PREMIERE LETTRE MOT HAUTEUR?

			recuperation_horizontale(plateau,i,mot);

			while(dicoCopy){////////////////////////////CHAQUE MOT DICO//////////////////////////////////////////////////

				if(contient(dicoCopy->mot->lettres,mot)){/////////////////////////////CONTIENT MOT///////////////////////////////

					positionLettre=1;
					occurenceMot=occurence_lettre_mot(dicoCopy->mot->lettres,plateau[i].lettre);

					while(occurenceMot){/////////////////////////////////////////////OCCURENCE MOT////////////////////////////////

						exit=0;
						copyYetHand=0;
						iPlateau=i;
						iMot=position_lettre_mot(plateau[iPlateau].lettre,dicoCopy->mot->lettres,positionLettre);

						if(iMot==0)
							choixParcours=0; //descendant uniquement

						else if(dicoCopy->mot->lettres[iMot+1]==' ' || dicoCopy->mot->lettres[iMot+1]=='\0')
							choixParcours=1; //montant uniquement

						else
							choixParcours=2; //montant->descendant

						//printf("Mot : %s, occurence : %d, iMot : %d, iPlateau : %d, choixParcours : %d\n", dicoCopy->mot->lettres, occurenceMot, iMot, iPlateau, choixParcours);
						iParcoursMot=iMot;
						iPlateauCopy=iPlateau;

						if(choixParcours!=0){////////////////////////PARCOURS MONTANT///////////////////////////////////////////////

							while(iParcoursMot){/////////////////////TRI MONTANT////////////////////////////////////////////////////

								iParcoursMot--;
								iPlateauCopy--;

								//printf("(%c,%d) : %d\n", dicoCopy->mot->lettres[iParcoursMot], iParcoursMot, iPlateauCopy);

								if(iPlateauCopy%15==14){
									exit=1;
									break;
								}

								if((plateau[iPlateauCopy].lettre!=dicoCopy->mot->lettres[iParcoursMot]) && plateau[iPlateauCopy].lettre!=' '){
									exit=1;
									break;
								}

								if(plateau[iPlateauCopy].lettre==' '){

									if(!copyYetHand){
										copy_hand(hand,handCopy);
										copyYetHand=1;
									}

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==0){
										if((effacer_joker_hand(handCopy))==0){
											exit=1;
											break;
										}
									}
								}
							}/////////////////////////////////////////TRI MONTANT///////////////////////////////////////////////////

						}////////////////////////////////////////////PARCOURS MONTANT///////////////////////////////////////////////

						iParcoursMot=iMot;
						iPlateauCopy=iPlateau;

						if(exit==0){
							//printf("position : %d MOT(%d) : %s rentre hauteur\n", i, iMot ,dicoCopy->mot->lettres);
						}

						if(choixParcours!=1 && exit==0){////////////PARCOURS DESCENDANT/////////////////////////////////////////////

							while(dicoCopy->mot->lettres[iParcoursMot+1]!=' ' && dicoCopy->mot->lettres[iParcoursMot+1]!='\0'){//////////////TRI DESCENDANT//////////////

								iParcoursMot++;
								iPlateauCopy++;

								if(iPlateauCopy%15==0){
									exit=1;
									break;
								}

								if((plateau[iPlateauCopy].lettre!=dicoCopy->mot->lettres[iParcoursMot]) && plateau[iPlateauCopy].lettre!=' '){
									exit=1;
									break;
								}

								if(plateau[iPlateauCopy].lettre==' '){

									if(!copyYetHand){
										copy_hand(hand,handCopy);
										copyYetHand=1;
									}

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==0){
										if((effacer_joker_hand(handCopy))==0){
											exit=1;
											break;
										}
									}
								}

							}////////////////////////////////////////////////////////////////////////////////////////////////////////////////TRI DESCENDANT//////////////


						}///////////////////////////////////////////PARCOURS DESCENDANT/////////////////////////////////////////////

						if(!copyYetHand)
							exit=1;

						if(exit==0){
							//printf("position : %d MOT(%d) : %s rentre haut bas\n", i, iMot ,dicoCopy->mot->lettres);
							//affiche_hand(handCopy);
						}

						if(exit==0){
							iParcoursMot=iMot;
							iPlateauCopy=iPlateau;
							copy_hand(hand,handCopy);
							copy_plateau(plateau,plateauCopy);
						}
						
						//////////////////////////////////////////////////////////////////////////////////////////////////////////PLACE PLATEAU///////////////

						if(choixParcours!=0 && exit==0){////////////////////PLACE MONTANT

							while(iParcoursMot){

								iParcoursMot--;
								iPlateauCopy--;

								if(plateauCopy[iPlateauCopy].lettre==' '){

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==1){
										plateauCopy[iPlateauCopy].lettre=dicoCopy->mot->lettres[iParcoursMot];
									}
									else if(effacer_joker_hand(handCopy)==0){
										exit=1;
										break;
									}

									else
										plateauCopy[iPlateauCopy].lettre='?';

									if(!iParcoursMot){

										if(iPlateauCopy%15!=0){

											if(plateauCopy[iPlateauCopy-1].lettre!=' '){
												recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

												if(!existe(motExiste)){

													if(effacer_joker_hand(handCopy)==1){

															handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
															plateauCopy[iPlateauCopy].lettre='?';
															recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

															if(!existe(motExiste)){
																exit=1;
																break;
															}
													}
													else{
														exit=1;
														break;
													}
												}
											}
										}
									}

									if( (iPlateauCopy>14 && plateauCopy[iPlateauCopy-15].lettre!=' ') || (iPlateauCopy<210 && plateauCopy[iPlateauCopy+15].lettre!=' ') ){

										recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

										if(!existe(motExiste)){

											if(effacer_joker_hand(handCopy)==1){

												handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
												plateauCopy[iPlateauCopy].lettre='?';
												recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

												if(!existe(motExiste)){
													exit=1;
													break;
												}
											}
											else{
												exit=1;
												break;
											}
										}
									}		
								}								
							}

						}////////////////////////////////////////PLACE MONTANT

						iParcoursMot=iMot;
						iPlateauCopy=iPlateau;

						if(choixParcours!=1 && exit==0){////////////////////PLACE DESCENDANT

							while(dicoCopy->mot->lettres[iParcoursMot+1]!=' ' && dicoCopy->mot->lettres[iParcoursMot+1]!='\0'){

								iParcoursMot++;
								iPlateauCopy++;

								if(plateauCopy[iPlateauCopy].lettre==' '){

									if(effacer_lettre_hand(handCopy, dicoCopy->mot->lettres[iParcoursMot])==1){
										plateauCopy[iPlateauCopy].lettre=dicoCopy->mot->lettres[iParcoursMot];
									}
									else if(effacer_joker_hand(handCopy)==0){
										exit=1;
										break;
									}

									else
										plateauCopy[iPlateauCopy].lettre='?';

									if(!(dicoCopy->mot->lettres[iParcoursMot+1]!=' ' && dicoCopy->mot->lettres[iParcoursMot+1]!='\0')){

										

										if(plateauCopy[iPlateauCopy+1].lettre!=' '){
											recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

											if(!existe(motExiste)){

													if(effacer_joker_hand(handCopy)==1){

															handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
															plateauCopy[iPlateauCopy].lettre='?';
															recuperation_horizontale(plateauCopy,iPlateauCopy,motExiste);

															if(!existe(motExiste)){
																exit=1;
																break;
															}
													}
													else{
														exit=1;
														break;
													}
												}
										}
										
									}

									if( (iPlateauCopy>14 && plateauCopy[iPlateauCopy-15].lettre!=' ') || (iPlateauCopy<210 && plateauCopy[iPlateauCopy+15].lettre!=' ') ){

										recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

										if(!existe(motExiste)){

											if(effacer_joker_hand(handCopy)==1){

												handCopy[trou_main(handCopy)].lettre=plateauCopy[iPlateauCopy].lettre;
												plateauCopy[iPlateauCopy].lettre='?';
												recuperation_verticale(plateauCopy,iPlateauCopy,motExiste);

												if(!existe(motExiste)){
													exit=1;
													break;
												}
											}
											else{
												exit=1;
												break;
											}
										}
									}	
								}								
							}

						}////////////////////////////////////////PLACE DESCENDANT

						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////PLACE PLATEAU///////////////

						if(exit==0 && choixParcours!=0){
							printf("position : %d MOT(%d) : %s rentre haut bas\n", i, iMot ,dicoCopy->mot->lettres);
							affiche_plateau_entier(plateauCopy,handCopy);
						}

						positionLettre++;
						occurenceMot--;

					}////////////////////////////////////////////////////////////////OCCURENCE MOT///////////////////////////////
				
				}/////////////////////////////////////////////////////////////////////CONTIENT MOT///////////////////////////////

				dicoCopy=dicoCopy->suivant;
			}///////////////////////////////////////////CHAQUE MOT DICO//////////////////////////////////////////////////
			
		}
	}////////////////////////////////////////////////////////FIN PARCOURS HORIZONTAL///////////////////////////////////////////////////////////////////*/
}




