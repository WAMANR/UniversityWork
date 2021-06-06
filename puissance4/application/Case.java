package application;

import javafx.scene.image.Image;
import javafx.scene.image.ImageView;

public class Case extends ImageView{
	
	private static Image rouge = new Image("JRP4.png");
	private static Image jaune = new Image("JJP4.png");
	
	
	//vide = 0 ; rouge = 1 ; jaune = 2;
	private int statut;
	
	public Case(){
		this.statut = 0;
	}
	
	public void set(int j){
		this.setImage(j == 1 ? rouge : jaune);
		this.statut = j;
	}
	
	
	public int getStatut(){
		return statut;
	}
	
}