////////////////////////////////////////////////////////////////FIN PARCOURS VERTICAL///////////////////////////////////////////////////////////





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