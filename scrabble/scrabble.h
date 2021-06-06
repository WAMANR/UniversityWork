
///////////////////////////////////////////STRUCTURES//////////////////////////////////////////////////////////

////MOT///////////////////////////////


typedef struct mot_t mot_t;
struct mot_t{
	char lettres[16];
	int n;
};


////NODE/////////////////////////////


typedef struct node_t node_t;
struct node_t{
	mot_t * mot;
	node_t * suivant;
};

typedef struct node_t * dico_t; //ou pointeur liste

////LISTE_OCCU///////////////////////

typedef struct node_occu_t node_occu_t;
struct node_occu_t{
	mot_t * mot;
	int position;
	int sens;//1 : horizontal / 0 : vertical
	node_occu_t * suivant;
};

typedef struct node_occu_t * dico_occu_t;


////CASE/////////////////////////////


typedef struct case_t case_t;
struct case_t{
	//coordonnée
	int iPlateau;

	//type de case (double, triple) : normal -> 0 / lettre : double -> 1 / triple -> 2 / mot : double -> 3 / triple -> 4
	int type;
	char lettre;
	int score;
};

typedef struct case_mot_t case_mot_t;
struct case_mot_t{
	case_t lettres[16];
	int n;
};



////PLATEAU//////////////////////////

typedef struct plateau_t plateau_t;
struct plateau_t{

	case_t plateau[225];
	plateau_t * suivant;
};

typedef struct plateau_t * liste_plateau_t;

////COUP////////////////////////////


typedef struct mot_coup_t mot_coup_t;
struct mot_coup_t{

	case_mot_t * mot;
	mot_coup_t * suivant;
	int scoreMot;
};
typedef struct mot_coup_t * liste_mot_coup_t;

typedef struct coup_t coup_t;
struct coup_t{
	int scoreCoup;
	liste_mot_coup_t mots;
	coup_t * suivant;
	int scrabble;//0 : oui / 1 : non
};

typedef struct coup_t * liste_coup_t;




///////////////////////////////////////////////////PROTOTYPES/////////////////////////////////////////////////////////////

dico_t init_dico();
int empty_dico(dico_t);
mot_t * creer_mot(char *);
dico_t insertion_dico(dico_t, char *);
void print_dico(dico_t);
case_t * getCase(int, int, case_t *);
void ajout_lettre_plateau(int, int, char, case_t *);
void set_type(int, int, int, case_t *);
void valeur_lettre(int, int, case_t *);
void valeur_lettre_plateau(case_t *);
void valeur_lettre_main(case_t *);
void init_plateau(case_t *);
void print_plateau(case_t *);
void affiche_plateau_entier(case_t *, case_t *);
void init_hand(case_t *);
void ajout_lettre_hand(int, char, case_t *);
int lettre_dans_main(case_t *, char);
void print_hand(case_t *);
int nombre_lettre(char *);
int est_dans(char *, dico_t);
int mot_egal(char *, char *);
int appartient(char *, dico_t);
int contient(char *, char *);
int existe(char *);
char * recuperation_verticale(case_t *, int, char *);
char * recuperation_horizontale(case_t *, int, char *);
int recuperation_lettre_up(case_t *, int);
int recuperation_lettre_left(case_t *, int);
int occurence_lettre_mot(char *, char);
dico_t groupe_mot_plateau(case_t *, dico_t);
void algorithme_coup(dico_t, case_t *, case_t *, liste_coup_t, case_t *, case_t *);
int position_lettre_mot(char, char *, int);
liste_plateau_t insertion_liste_plateau(liste_plateau_t, case_t *);
int empty_liste_plateau(liste_plateau_t);
liste_plateau_t init_liste_plateau();
void affiche_plateau(case_t *);
void affiche_liste_plateau(liste_plateau_t);
void copy_plateau(case_t *, case_t *);
void copy_hand(case_t *, case_t *);
void affiche_hand(case_t *);
int effacer_lettre_hand(case_t *, char);
int effacer_joker_hand(case_t *);
liste_coup_t init_liste_coup();
int main_pleine(case_t *);
case_mot_t * creer_case_mot(char *, int, int, case_t *);
liste_mot_coup_t insertion_liste_mot_coup(liste_mot_coup_t, mot_coup_t *);
liste_mot_coup_t init_liste_mot_coup();
void print_liste_mot_coup(liste_mot_coup_t);
int return_valeur_lettre(char);
int main_vide(case_t *);
liste_coup_t insertion_liste_coup(liste_coup_t, coup_t *);