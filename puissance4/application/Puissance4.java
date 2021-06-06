import java.util.Scanner;

//Code suffisement simple pour être réalisé en 1 seule classe
//Jeu jouable dans la console


public class Puissance4{

	public static void main(String[] args) {

		Grille grille = new Grille();
		Scanner scanner = new Scanner(System.in);
		int gagnant = 0;

		//choix mode

		System.out.println("Choix du mode :");
		System.out.println("Joueur contre joueur taper \"0\" / Joueur contre ia taper \"1\" :");
		boolean mode = false;
		int choix = 2;

		while(!mode){

			String ligneChoix = scanner.nextLine();

			try{
				choix = Integer.valueOf(ligneChoix);

				if(choix != 0 && choix != 1){
					System.out.println("Veuillez entrer un choix valable");
				}

				else{
					mode = true;
				}
			}catch(Exception e){System.out.println("Veuillez entrer un choix valable");}
		}

		//joueur contre joueur

		if(choix == 0){

			System.out.println("Vous avez choisi le mode joueur contre joueur");

			//boucle de jeu

			for(grille.i=1 ; grille.i <= 7*6 ; grille.i++){

				System.out.print("\033[H\033[2J");  
    			System.out.flush();  
				grille.afficheGrille();

				//Placements du jeton:
				System.out.println("Tour du joueur " + (grille.i%2==1 ? 'O' : 'X') );
				System.out.println("Entrez le numéro de la colonne entre 1 et " + 7 + " ...");
				boolean placement = false;
				int colonne = -1;
				while(!placement){
					colonne = -1;
					String ligne = scanner.nextLine();
					//vérification que la ligne est un entier entre 1 et C:
					try{
						colonne = Integer.valueOf(ligne);
					
						if(colonne >= 1 && colonne <= 7){
							if(grille.grille[colonne - 1][0] != '.'){
								System.out.println("Colonne pleine, réitérez");
							} else {
								placement = true;
							}
						} else {
							System.out.println("Nombre incorrect, réitérez");
						}
					
					}catch(Exception e){System.out.println("Nombre incorrect, réitérez");}
				
				}
				//placement du jeton:
				int rang = 5;
				while(grille.grille[colonne - 1][rang] != '.'){
					rang--;
				}
				grille.grille[colonne - 1][rang] = (grille.i%2==1 ? 'O' : 'X');

				//detection victoire

				gagnant = grille.detectionVictoire();

				if(gagnant==1 || gagnant ==2){

					grille.i = 7*6+1;
				}

			}

			//affichage victoire	

			System.out.print("\033[H\033[2J");  
    		System.out.flush();  
			grille.afficheResultat(gagnant);

		}

		//joueur contre ia

		if(choix==1){

			System.out.print("\033[H\033[2J");  
  		    System.out.flush();  
			System.out.println("Vous avez choisi le mode joueur contre ia");
			System.out.println("Choix de la diffculté de l'IA");
			System.out.println(" 0 -> très facile / 1 -> facile / 2 -> moyen / 3 -> difficile / 4 -> très difficile / 5 -> extrême / 6 -> GOD LEVEL :");

			//choix mode difficulté

			boolean difficulte=false;
			int level=6;

			while(!difficulte){

				String ligneLevel = scanner.nextLine();

				try{
					level = Integer.valueOf(ligneLevel);

					if(level != 0 && level != 1 && level != 2 && level != 3 && level != 4 && level != 5 && level != 6){
						System.out.println("Veuillez entrer un choix valable");
					}

					else{
						difficulte = true;
					}
				}catch(Exception e){System.out.println("Veuillez entrer un choix valable");}
			}

			//boucle de jeu
		
			for(grille.i=1 ; grille.i <= 7*6 ; grille.i++){

				System.out.print("\033[H\033[2J");  
    			System.out.flush();  
				grille.afficheGrille();

				int colonne = -1;

				if(grille.i%2==1){//tour joueur

					System.out.println("Entrez le numéro de la colonne entre 1 et " + 7 + " ...");
					boolean placement = false;
					
					while(!placement){
						colonne = -1;
						String ligne = scanner.nextLine();
						//vérification que la ligne est un entier entre 1 et C:
						try{
							colonne = Integer.valueOf(ligne);
					
							if(colonne >= 1 && colonne <= 7){
								if(grille.grille[colonne - 1][0] != '.'){
									System.out.println("Colonne pleine, réitérez");
								} else {
									placement = true;
								}
							} else {
								System.out.println("Nombre incorrect, réitérez");
							}
					
						}catch(Exception e){System.out.println("Nombre incorrect, réitérez");}
				
					}

				}

				else{//tour ia

					Ia ia = new Ia(grille, level);
					ia.iaChoice();
					colonne=ia.getChoix();



				}

				//placement du jeton:
				int rang = 5;
				while(grille.grille[colonne - 1][rang] != '.'){
					rang--;
				}
				grille.grille[colonne - 1][rang] = (grille.i%2==1 ? 'O' : 'X');

				//detection victoire

				gagnant = grille.detectionVictoire();

				if(gagnant==1 || gagnant ==2){

					grille.i = 7*6+1;
				}

			}

			//affichage victoire

			System.out.print("\033[H\033[2J");  
   		    System.out.flush();  
			grille.afficheResultat(gagnant);


		}

	}
}