public class Ia{

	private int choixColonne = -1;
	private int level = -1;

	public int getChoix(){ return this.choixColonne; }

	
	private Grille grilleCopy;


	public Ia(Grille grille, int level){

		this.grilleCopy = new Grille(grille);
		switch(level){
			case 0 :
				this.level=level;
				break;
			case 1 :
				this.level=2;
				break;
			case 2 : 
				this.level=4;
				break;
			case 3 :
				this.level=6;
				break;
			case 4 :
				this.level=8;
				break;

		}
	}

	//ALGORITHME DE CHOIX

	public int heuristique(int colonne, int rang, int tour){

		int joueur=(tour%2==1 ? 1 : 2);
		int joueur2=(tour%2==1 ? 1 : 2);
		this.grilleCopy.i=tour;

		if(this.grilleCopy.detectionVictoire()==joueur){
			return 100000;
		}
		else if(this.grilleCopy.detectionVictoire()==joueur2){
			return -100000;
		}
		else{
			return checkHori(tour) + checkVert(tour) + checkDiagHGBD(tour) + checkDiagHDBG(tour);
		}
	}

	public int minimax(int profondeur, int colonne, int rang, int alpha, int beta, int joueur){

		char symbole=(joueur%2==1 ? 'O' : 'X');

		if(profondeur==0 || finDeJeu()==1){
			return heuristique(colonne+1, rang, joueur);
		}

		for(int y=0; y<=6; y++){
			rang=ajoutJeton(y+1, joueur+1);
			if(rang!=-1){
				int score = -minimax(profondeur-1, y, rang, -beta, -alpha, joueur+1);
				enleverJeton(y+1);
				if(score>=alpha){
					alpha=score;
					if(alpha>=beta) break;
				}
			}
		}
		return alpha;
	}

	public int iaChoice(){

		int max=-999999999;
		int choix=-1;
		int joueur=this.grilleCopy.i;

		if(this.level!=0){

			for(int y=0; y<=6; y++){

				int rang=ajoutJeton(y+1, joueur+1);
				if(rang!=1){
					this.grilleCopy.i=joueur+1;
					if(this.grilleCopy.detectionVictoire()==1){
						enleverJeton(y+1);
						this.choixColonne=y+1;
						return 0;
					}
					this.grilleCopy.i=joueur;
				}
				enleverJeton(y+1);
			}
		}

		if(this.level!=0 && this.level!=1){

			for(int y=0; y<=6; y++){

				int rang=ajoutJeton(y+1, joueur);
				if(rang!=-1){
					if(this.grilleCopy.detectionVictoire()==2){
						enleverJeton(y+1);
						this.choixColonne=y+1;
						return 0;
					}
				}
				enleverJeton(y+1);
			}
		}

		for(int y=0; y<=6; y++){

			int rang=ajoutJeton(y+1, joueur);
			if(rang!=-1){
				int score=minimax(this.level,y,rang, -999999999, 999999999, joueur);
				if(score>max){
					if(this.grilleCopy.grille[y][0]=='.'){
						max=score;
						choix=y+1;
					}
				}
			}
			enleverJeton(y+1);
		}
		System.out.println("CHOIX : "+choix);
		this.choixColonne=choix;
		return 0;
	}

	//FIN DE JEU

	public int nbCoups(){

		int nb=0;
		for(int y = 0; y<=6 ; y++){
			for(int x=0; x<=5; x++){

				if(this.grilleCopy.grille[y][x]!='.')
					nb++;
			}
		}
		return nb;
	}

	public int finDeJeu(){

		for(int y=0; y<=6; y++){
			for(int x=0; x<=5; x++){
				if(this.grilleCopy.grille[y][x]!='.'){
					if((y<=3 && this.grilleCopy.grille[y+1][x]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y+2][x]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y+3][x]==this.grilleCopy.grille[y][x])
						|| (x<=2 && this.grilleCopy.grille[y][x+1]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y][x+2]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y][x+3]==this.grilleCopy.grille[y][x])
						|| (y<=3 && x<=2 && this.grilleCopy.grille[y+1][x+1]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y+2][x+2]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y+3][x+3]==this.grilleCopy.grille[y][x])
						|| (y>=3 && x<=2 && this.grilleCopy.grille[y-1][x+1]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y-2][x+2]==this.grilleCopy.grille[y][x] && this.grilleCopy.grille[y-3][x+3]==this.grilleCopy.grille[y][x]))
						return 1;
				}
			}
			if(nbCoups()==7*6)
				return 1;
		}
		return 0;
	}


	//ENLEVER/AJOUTER JETON


	public void enleverJeton(int colonne){

		int rang = 0;
		while(this.grilleCopy.grille[colonne-1][rang] == '.'){
			rang++;
		}
		this.grilleCopy.grille[colonne-1][rang] = '.';
	}

	public int ajoutJeton(int colonne, int joueur){

		char symbole=(joueur%2==1 ? 'O' : 'X');

		if(this.grilleCopy.grille[colonne-1][0] != '.'){
			return -1;
		}

		int rang=5;

		while(this.grilleCopy.grille[colonne-1][rang] != '.'){
			rang--;
		}
		this.grilleCopy.grille[colonne-1][rang] = symbole;
		return rang;
	}

	

	//CHECK VALEUR

	public int checkHori(int tour){

		int joueur=(tour%2==1 ? 'O' : 'X');
		int joueur2=(tour%2==1 ? 'X' : 'O');
		int nbJetons=0, score=0, nbJetonsVS=0;

		for(int x=0; x<=5; x++){
			for(int y=0; y<=3; y++){

				if(this.grilleCopy.grille[y][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y+1][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y+1][x]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y+2][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y+2][x]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y+3][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y+3][x]==joueur2) nbJetonsVS++;
				switch(nbJetons){
					case 1 : score += 1; break;
					case 2 : score += 6; break;
					case 3 : score +=1000; break;
				}
				switch(nbJetonsVS){
					case 1 : score += -1; break;
					case 2 : score += -10; break;
					case 3 : score += -500; break;
				}
				nbJetons=0;
			    nbJetonsVS=0;
			}
		}
		return score;
	}

	public int checkVert(int tour){

		int joueur=(tour%2==1 ? 'O' : 'X');
		int joueur2=(tour%2==1 ? 'X' : 'O');
		int nbJetons=0, score=0, nbJetonsVS=0;

		for(int y=0; y<=6; y++){
			for(int x=0 ; x<=2; x++){

				if(this.grilleCopy.grille[y][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y][x+1]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x+1]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y][x+2]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x+2]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y][x+3]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x+3]==joueur2) nbJetonsVS++;
				switch(nbJetons){
					case 1 : score += 1; break;
					case 2 : score += 10; break;
					case 3 : score +=1000; break;
				}
				switch(nbJetonsVS){
					case 1 : score += -1; break;
					case 2 : score += -10; break;
					case 3 : score += -500; break;
				}
				nbJetons=0;
			    nbJetonsVS=0;
			}
		}
		return score;
	}

	public int checkDiagHGBD(int tour){

		int joueur=(tour%2==1 ? 'O' : 'X');
		int joueur2=(tour%2==1 ? 'X' : 'O');
		int nbJetons=0, score=0, nbJetonsVS=0;

		for(int y=0; y<=3; y++){
			for(int x=0; x<=2; x++){

				if(this.grilleCopy.grille[y][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y+1][x+1]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y+1][x+1]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y+2][x+2]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y+2][x+2]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y+3][x+3]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y+3][x+3]==joueur2) nbJetonsVS++;
				switch(nbJetons){
					case 1 : score += 1; break;
					case 2 : score += 10; break;
					case 3 : score +=1000; break;
				}
				switch(nbJetonsVS){
					case 1 : score += -1; break;
					case 2 : score += -10; break;
					case 3 : score += -500; break;
				}
				nbJetons=0;
			    nbJetonsVS=0;
			}
		}
		return score;
	}


	public int checkDiagHDBG(int tour){

		int joueur=(tour%2==1 ? 'O' : 'X');
		int joueur2=(tour%2==1 ? 'X' : 'O');
		int nbJetons=0, score=0, nbJetonsVS=0;

		for(int y=3; y<=6; y++){
			for(int x=0; x<=2; x++){

				if(this.grilleCopy.grille[y][x]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y][x]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y-1][x+1]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y-1][x+1]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y-2][x+2]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y-2][x+2]==joueur2) nbJetonsVS++;
				if(this.grilleCopy.grille[y-3][x+3]==joueur) nbJetons++;
				else if(this.grilleCopy.grille[y-3][x+3]==joueur2) nbJetonsVS++;
				switch(nbJetons){
					case 1 : score += 1; break;
					case 2 : score += 10; break;
					case 3 : score +=1000; break;
				}
				switch(nbJetonsVS){
					case 1 : score += -1; break;
					case 2 : score += -10; break;
					case 3 : score += -500; break;
				}
				nbJetons=0;
			    nbJetonsVS=0;
			}
		}

		return score;
	}




}