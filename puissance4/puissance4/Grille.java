public class Grille{

	public int i;
	public char[][] grille = new char[7][6];

	//creation grille vide

	public Grille(){

		for(int x = 0 ; x < 7 ; x++)
			for(int y = 0 ; y < 6 ; y++)
				this.grille[x][y] = '.';
	}

	//creation copie grille

	public Grille(Grille grille){

		for(int x = 0 ; x < 7 ; x++)
			for(int y = 0 ; y < 6 ; y++)
				this.grille[x][y] = grille.grille[x][y];
		this.i=grille.i;
	}

	public void afficheGrille(){

			System.out.println("Tour " + i + ", Etat du plateau :");

			//ligne haut		
			for(int loop = 0 ; loop < 7+2+2*7 ; loop++)System.out.print('-');
			System.out.println();

		 	//contenu grille		
			for(int y = 0 ; y < 6 ; y++){
				System.out.print('|');
				for(int x = 0 ; x < 7 ; x++){
					System.out.print(" " + grille[x][y] + " ");
				}
				System.out.print('|');
				System.out.println();
			}

			//ligne bas
			for(int loop = 0 ; loop < 7+2+2*7 ; loop++)System.out.print('-');
			System.out.println();
 	 }

 	 public void afficheResultat(int gagnant){

 	 		System.out.println();

			//ligne haut		
			for(int loop = 0 ; loop < 7+2+2*7 ; loop++)System.out.print('-');
			System.out.println();

		 	//contenu grille		
			for(int y = 0 ; y < 6 ; y++){
				System.out.print('|');
				for(int x = 0 ; x < 7 ; x++){
					System.out.print(" " + grille[x][y] + " ");
				}
				System.out.print('|');
				System.out.println();
			}

			//ligne bas
			for(int loop = 0 ; loop < 7+2+2*7 ; loop++)System.out.print('-');
			

			System.out.println();
			System.out.println("*********************");
			System.out.println("****FIN DE PARTIE****");
			System.out.println("*********************");
			if(gagnant == 0)
				System.out.println("*******EGALITE*******");
			if(gagnant == 1)
				System.out.println("****VICTOIRE DE J1****");
			if(gagnant == 2)
				System.out.println("****VICTOIRE DE J2****");
			System.out.println("*********************");
 	 }


 	 public int detectionVictoire(){

 	 	//symbole en cours:
		char symbole = (i%2==1 ? 'O' : 'X');
		//nombre alignés maximal: 
		for(int y=0; y<=6; y++){
			for(int x=0; x<=5; x++){

				if(this.grille[y][x]!='.'){
					if((y<=3 && this.grille[y+1][x]==this.grille[y][x] && this.grille[y+2][x]==this.grille[y][x] && this.grille[y+3][x]==this.grille[y][x])
						|| (x<=2 && this.grille[y][x+1]==this.grille[y][x] && this.grille[y][x+2]==this.grille[y][x] && this.grille[y][x+3]==this.grille[y][x])
						|| (y<=3 && x<=2 && this.grille[y+1][x+1]==this.grille[y][x] && this.grille[y+2][x+2]==this.grille[y][x] && this.grille[y+3][x+3]==this.grille[y][x])
						|| (y>=3 && x<=2 && this.grille[y-1][x+1]==this.grille[y][x] && this.grille[y-2][x+2]==this.grille[y][x] && this.grille[y-3][x+3]==this.grille[y][x])){
						if(this.grille[y][x]=='O')
							return 1;
						else
							return 2;
					}
				}
			}
		}
		return 0;
	}
 		 

 	 
}