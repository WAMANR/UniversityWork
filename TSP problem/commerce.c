#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <signal.h>

struct city{
   unsigned short exist;//is this index has a city in it
   unsigned short id;//postal code of city
   unsigned short nbRelated;//number of related cities
   unsigned short * related;//vec of related cities id
   unsigned int longitude;
   unsigned int latitude;
};
typedef struct city cities [80];//city structure used for parsing in csv file

struct triplet{
   unsigned short i;//city i
   unsigned short j;//city j
   float poids;//distance
};
typedef struct triplet triple_t;

struct gramaco{
   unsigned short nbs;//nbr of vertex
   unsigned short nba;//nbr of edges
   triple_t * ares;//edges vector
};
typedef struct gramaco gramaco_t;

struct noeud {
  unsigned short num;//id of node
  unsigned short nbs;//nbr of succ
  struct noeud ** succ;//vector of pointers of succ nodes
  float * poids;//vector of succ's weights
};
typedef struct noeud noeud;
typedef struct noeud * ptnoeud;

struct lesnoeuds {
  unsigned short vu;//is this node as been explored?
  ptnoeud ne;//node pointer
};
typedef struct lesnoeuds legraphe [80];

//AUXILIARY FUNCTIONS///////////////////////////////////////////////////////////////////////////

float euclidian_distance(unsigned int x1, unsigned int x2, unsigned int y1, unsigned int y2){//calculate euclidian distance between two cities
  unsigned int x = (x1-x2);
  unsigned int y = (y1-y2);
  return sqrtf(x*x + y*y);
}

int is_in_vec(unsigned short number, unsigned short * vec, unsigned short size){//return if a value is in a vec
  int i;
  for(i=0; i<size; i++){
    if(vec[i] == number)
      return 1;
  }
  return 0;
}

int is_empty_vec(unsigned short * vec, unsigned short size){//return if a vec is only composed of zero
  int i;
  for(i=0; i<size; i++){
    if(vec[i])
      return 0;
  }
  return 1;
}

int is_full_vec(unsigned short * vec, unsigned short size){//return if a vec is only composed by value different from zero
  int i;
  for(i=0; i<size; i++){
    if(!vec[i])
      return 0;
  }
  return 1;
}

void copy_vec(unsigned short * sendVec, unsigned short * destVec, unsigned short size){//copy a vec into an other of same size
  int i;
  for(i=0; i<size; i++){
    destVec[i]=sendVec[i];
  }
}

void clear_vec(unsigned short * vec, unsigned short size){//fill a vec with zero
  int i;
  for(i=0; i<size; i++){
    vec[i]=0;
  }
}

void shuffle_vec(unsigned short * vec, unsigned short size){//randomly shuffle vec values

  unsigned short i,j,tmp;
  srand(time(NULL));
  for(i=0; i<size; i++){
    j = rand()%(size-i)+i;
    tmp = vec[i];
    vec[i]=vec[j];
    vec[j]=tmp;
  }
}

float get_gramaco_weight(gramaco_t * gramaco, short city1, short city2){//get weight from gramaco with 2 cities id
  int i;
  for(i=0; i<gramaco->nba; i++){
    if((gramaco->ares[i].i == city1 && gramaco->ares[i].j == city2)
    || (gramaco->ares[i].j == city1 && gramaco->ares[i].i == city2))
        return gramaco->ares[i].poids;
  }
  return 0;
}

void sort_by_weight(gramaco_t * gramaco, unsigned short * vec, unsigned short size, unsigned short current){//sort vector of edges by weight

  unsigned short tmp1, tmp2;
  float weight;
  int i,j;
  for(i=0; i<size; i++){
    if(!vec[i])
      continue;

    weight = get_gramaco_weight(gramaco, current, vec[i]);

    for(j=i; j>0; j--){
      if(vec[j-1] && get_gramaco_weight(gramaco, current, vec[j-1])<weight)
        break;
    }
    if(j!=i){
      tmp1 = vec[i];
      for(; j<=i; j++){
        tmp2 = vec[j];
        vec[j] = tmp1;
        tmp1 = tmp2;
      }
    }
  }
}

long double gramaco_calculate_weight(gramaco_t * gramaco, unsigned short * vec, unsigned short size){//calculate weight of vector with gramaco

  long double weight;
  int i;
  for(i=0, weight=0; i<size-1; i++){
    weight+=get_gramaco_weight(gramaco, vec[i], vec[i+1]);
  }
  weight+=get_gramaco_weight(gramaco,vec[size-1],vec[0]);
  return weight;
}

long double legraphe_calculate_weight(legraphe graphe, unsigned short * vec, unsigned short size){//calculate weight of vector with legraphe

  long double weight;
  unsigned short i, j;

  for(i=0, weight=0 ; i<size-1; i++){
    for(j=0; j<graphe[vec[i]].ne->nbs; j++){
      if(graphe[vec[i]].ne->succ[j]->num == vec[i+1]){
        weight += graphe[vec[i]].ne->poids[j];
        break;
      }
    }
  }
  for(j=0; j<graphe[vec[size-1]].ne->nbs; j++){
    if(graphe[vec[size-1]].ne->succ[j]->num == vec[0]){
      weight += graphe[vec[size-1]].ne->poids[j];
    }
  }
  return weight;

}

void exitfunc(int sig){//stop the program

  _exit(0);
}

//CITIES FUNCTIONS////////////////////////////////////////////////////////////////////

void parse_cities(cities cities){//parse cities and store into cities structure

  unsigned int vecMalloc, step, posInStep, city, i;

  char c;
  char * id, *distance;
  id = malloc(sizeof(char)*5);
  assert(id);
  distance = malloc(sizeof(char)*6);
  assert(distance);

  FILE * file;
  file = fopen("93.csv", "r");

  for (i = 0; i < 80; i++){
    cities[i].exist = 0;//initialise cities -> doesn't exist
  }

  if(file){
    vecMalloc = 0;
    step = posInStep = 0;
    while ((c = getc(file)) != EOF){

      if(c == '\n'){//manage return to line in parsing (new city)
        i = 0;
        vecMalloc = 0;
        posInStep = 0;
        step = 0;
        continue;
      }
      if(c == ','){//manage comma in parsing (new characteristic of city)
        if(step == 3){
          cities[city].related[i] = atoi(id)-93000;
          i = 0;
        }
        ++step;
        posInStep = 0;
        continue;
      }

      if(!step){//put id in id variable
        id[posInStep] = c;
      }

      else if(step == 1 && posInStep == 0){//give city id
        city = atoi(id)-93000;
        cities[city].id = city;
        cities[city].exist = 1;
      }

      else if(step == 2){//give city nbRelated
        cities[city].nbRelated = (int) c - (int) '0';
      }

      else if(step == 3){
        if(!vecMalloc){//init related
          i = 0;
          cities[city].related = malloc(sizeof(int)*cities[city].nbRelated);
          assert(cities[city].related);
          vecMalloc = 1;
        }
        else if(c == '|'){//put one related in vec
          cities[city].related[i] = atoi(id)-93000;
          i++;
          posInStep = 0;
          continue;
        }
        id[posInStep] = c;//load id with char
      }

      else if(step == 5){
        if(posInStep < 6){
          distance[posInStep] = c;
        }
        else if(posInStep == 6){
          cities[city].longitude = atoi(distance);
        }
        else if(posInStep > 6){
          distance[posInStep-7] = c;
        }
        if(posInStep == 12){
          cities[city].latitude = atoi(distance);
        }
      }

      ++posInStep;
    }
  }
  fclose(file);
}

void display_cities(cities cities){//display cities structure

  int i, j;

  printf("\nCITIES :\n");
  for (i = 0; i < 80; i++){
    if(cities[i].exist){
      printf("id : %d", cities[i].id);
      printf(" | nbRelated : %d ", cities[i].nbRelated);
      for(j = 0; j < cities[i].nbRelated; j++){
        printf("|%d", cities[i].related[j]);
      }
      printf(" | longitude : %d", cities[i].longitude);
      printf(" | latitude : %d", cities[i].latitude);
      printf("\n");
    }
  }
}

//GRAMACO FUNCTIONS///////////////////////////////////////////////////////////////////

gramaco_t create_gramaco(cities cities){//make gramaco structure from cities structure

  gramaco_t gramaco;
  unsigned short i, j, k;
  unsigned short nba = 0, nbs = 0;

  for (i=0; i<80; i++){
    if(cities[i].exist){
      nbs++;
      for (j=0; j<cities[i].nbRelated; j++){
        if(cities[i].related[j] > i){
          nba++;
        }
      }
    }
  }

  gramaco.nbs = nbs;
  gramaco.nba = nba;
  gramaco.ares = malloc(sizeof(triple_t)*nba);
  assert(gramaco.ares);
  k=0;
  triple_t current;

  for(i=0; i<80; i++){
    if(cities[i].exist){
      for(j=0; j<cities[i].nbRelated; j++){
        if(cities[i].related[j] > i){
          current.i = i;
          current.j = cities[i].related[j];
          current.poids = euclidian_distance(cities[i].latitude, cities[current.j].latitude,
                                             cities[i].longitude, cities[current.j].longitude);
          gramaco.ares[k] = current;
          k++;
        }
      }
    }
  }
  return gramaco;
}

void display_gramaco(gramaco_t gramaco){//display the gramaco_t structure

  int i;
  printf("\nGRAMACO :\nNBS : %d, NBA : %d\nARES :\n", gramaco.nbs, gramaco.nba);
  for (i=0; i<gramaco.nba; i++){
    printf("ares[%d] == i:%d | j:%d | weight:%f\n",i, gramaco.ares[i].i, gramaco.ares[i].j, gramaco.ares[i].poids);
  }
}

//         TSP SEARCH
void gramaco_tsp_deep_aux(gramaco_t * gramaco, unsigned short * visitedCities, unsigned short * resultCities, long double * weight, unsigned short currentPos, unsigned short option, clock_t t){

  long double currentWeight;
  unsigned short i, j, nbRelated;
  unsigned short * relatedCities;
  clock_t elapsed;

  for(i=0, nbRelated=0; i<gramaco->nba; i++){
    if(gramaco->ares[i].i == visitedCities[currentPos] || gramaco->ares[i].j == visitedCities[currentPos])
      nbRelated++;
  }
  relatedCities = malloc(sizeof(unsigned short)*nbRelated);

  for(i=0, j=0; j<nbRelated; i++){
    if(gramaco->ares[i].i == visitedCities[currentPos]){
      relatedCities[j]=gramaco->ares[i].j;
      j++;
    }
    else if(gramaco->ares[i].j == visitedCities[currentPos]){
      relatedCities[j]=gramaco->ares[i].i;
      j++;
    }
  }

  if(is_full_vec(visitedCities, gramaco->nbs) && is_in_vec(visitedCities[0], relatedCities, nbRelated)){
    currentWeight = gramaco_calculate_weight(gramaco, visitedCities, gramaco->nbs);
    elapsed = clock() - t;
    printf("\nTime : %ld s | Solution found : weight -> %LF | road -> ", elapsed/CLOCKS_PER_SEC, currentWeight);
    for(i=0; i<gramaco->nbs; i++){
      printf("%d|", visitedCities[i]);
    }
    if(currentWeight<*weight){
      printf(" -> NEW OPTIMAL\n");
      *weight=currentWeight;
      copy_vec(visitedCities, resultCities, gramaco->nbs);
    }
    return;
  }

  for(i=0; i<nbRelated; i++){//remove already visited cities from related
    if(is_in_vec(relatedCities[i], visitedCities, gramaco->nbs))
      relatedCities[i] = 0;
  }

  if(!is_empty_vec(relatedCities, nbRelated)){//no related cities not visited available (blocked)

    if(option == 1)//shuffle related cities (pseudo-random)
      shuffle_vec(relatedCities, nbRelated);
    else if(option == 2)
      sort_by_weight(gramaco, relatedCities, nbRelated, visitedCities[currentPos]);

    for(i=0; i<nbRelated; i++){
      if(relatedCities[i]){
        visitedCities[currentPos+1]=relatedCities[i];
        gramaco_tsp_deep_aux(gramaco, visitedCities, resultCities, weight, currentPos+1, option, t);
      }
    }
  }
  visitedCities[currentPos]=0;
  free(relatedCities);
  return;
}

void gramaco_tsp_deep(gramaco_t * gramaco, unsigned short * resultCities, long double * weight, unsigned short option){

  clock_t t;
  t = clock();

  unsigned short i,j;
  unsigned short *startCities, *visitedCities;
  startCities = calloc(gramaco->nbs, sizeof(unsigned short));
  assert(startCities);
  visitedCities = calloc(gramaco->nbs, sizeof(unsigned short));
  assert(visitedCities);



  for(i=0, j=0; i<gramaco->nba; i++){
    if(!is_in_vec(gramaco->ares[i].i, startCities,gramaco->nbs)){
      startCities[j]=gramaco->ares[i].i;
      j++;
    }
    if(!is_in_vec(gramaco->ares[i].j, startCities,gramaco->nbs)){
      startCities[j]=gramaco->ares[i].j;
      j++;
    }
  }
  shuffle_vec(startCities, gramaco->nbs);

  for(i=0; i<gramaco->nbs; i++){
    clear_vec(visitedCities, gramaco->nbs);
    visitedCities[0]=startCities[i];
    gramaco_tsp_deep_aux(gramaco, visitedCities, resultCities, weight, 0, option, t);
  }

  free(startCities);
  free(visitedCities);
}

void gramaco_tsp_random_aux(gramaco_t * gramaco, unsigned short * visitedCities, unsigned short * resultCities, long double * weight, unsigned short currentPos, clock_t t){

  long double currentWeight;
  unsigned short i, j, nbRelated, nbRelatedPossible, choice;
  unsigned short * relatedCities;
  clock_t elapsed;

  for(i=0, nbRelated=0; i<gramaco->nba; i++){
    if(gramaco->ares[i].i == visitedCities[currentPos] || gramaco->ares[i].j == visitedCities[currentPos])
      nbRelated++;
  }
  relatedCities = malloc(sizeof(unsigned short)*nbRelated);

  for(i=0, j=0; j<nbRelated; i++){
    if(gramaco->ares[i].i == visitedCities[currentPos]){
      relatedCities[j]=gramaco->ares[i].j;
      j++;
    }
    else if(gramaco->ares[i].j == visitedCities[currentPos]){
      relatedCities[j]=gramaco->ares[i].i;
      j++;
    }
  }

  if(is_full_vec(visitedCities, gramaco->nbs) && is_in_vec(visitedCities[0], relatedCities, nbRelated)){
    currentWeight = gramaco_calculate_weight(gramaco, visitedCities, gramaco->nbs);
    elapsed = clock() - t;
    printf("\nTime : %ld s | Solution found : Weight : %LF\n", elapsed/CLOCKS_PER_SEC, currentWeight);
    for(i=0; i<gramaco->nbs; i++){
      printf("%d|",visitedCities[i]);
    }
    if(currentWeight<*weight){
      printf(" -> NEW OPTIMAL\n");
      *weight=currentWeight;
      copy_vec(visitedCities, resultCities, gramaco->nbs);
    }
    return;
  }

  nbRelatedPossible = nbRelated;
  for(i=0; i<nbRelated; i++){//remove already visited cities from related
    if(is_in_vec(relatedCities[i], visitedCities, gramaco->nbs)){
      relatedCities[i] = 0;
      nbRelatedPossible--;
    }
  }

  if(nbRelatedPossible>0){
    choice = rand()%nbRelatedPossible+1;
    for(i=0, j=0; j<choice; i++){
      if(relatedCities[i]) j++;
    }
    visitedCities[currentPos+1]=relatedCities[i];
    free(relatedCities);
    return gramaco_tsp_random_aux(gramaco, visitedCities, resultCities, weight, currentPos+1, t);
  }

  return;
}

void gramaco_tsp_random(gramaco_t * gramaco, unsigned short * resultCities, long double * weight){

  clock_t t;
  t = clock();

  unsigned short choice, choice2;
  unsigned short * visitedCities;
  visitedCities = calloc(gramaco->nbs, sizeof(unsigned short));
  assert(visitedCities);

  srand(time(NULL));
  while(1){
    clear_vec(visitedCities, gramaco->nbs);
    choice = rand()%gramaco->nba;
    choice2 = rand()%2;
    if(!choice2)
      visitedCities[0] = gramaco->ares[choice].i;
    else
      visitedCities[0] = gramaco->ares[choice].j;
    gramaco_tsp_random_aux(gramaco, visitedCities, resultCities, weight, 0, t);
  }

  free(visitedCities);

}

//LEGRAPHE FUNCTIONS//////////////////////////////////////////////////////////////////

void create_graphe(cities cities, legraphe graphe){//make the graphe structure from the cities structure

  int i, j, k;
  ptnoeud current;

  for (i=0; i<80; i++){
    if(!cities[i].exist){
      graphe[i].ne = NULL;
    }
    else{
      graphe[i].vu = 0;
      current = malloc(sizeof(noeud));
      current->num = i;
      current->nbs = cities[i].nbRelated;
      current->succ = malloc(sizeof(ptnoeud)*current->nbs);
      current->poids = malloc(sizeof(float)*current->nbs);
      graphe[i].ne = current;
    }
  }
  for(i=0; i<80; i++){
    if(graphe[i].ne != NULL){
      for(j=0; j<graphe[i].ne->nbs; j++){
        for(k=0; k<80; k++){
          if(graphe[k].ne && graphe[k].ne->num == cities[i].related[j]){
            graphe[i].ne->succ[j] = graphe[k].ne;
            break;
          }
        }
        graphe[i].ne->poids[j] = euclidian_distance(cities[i].latitude, cities[graphe[i].ne->succ[j]->num].latitude,
                                                    cities[i].longitude, cities[graphe[i].ne->succ[j]->num].longitude);
      }
    }
  }
}

void display_graphe(legraphe graphe){//display the lesnoeuds structure

  int i,j;

  printf("\nLEGRAPHE : \n");
  for (i=0; i<80; i++){
    if(graphe[i].ne){
      printf("[%d] -> ", i);
      printf("vu : %d | ", graphe[i].vu);
      printf("num : %d / ", graphe[i].ne->num);
      printf("nbs : %d / ", graphe[i].ne->nbs);
      printf("succ -> ");
      for(j=0; j<graphe[i].ne->nbs; j++){
        printf("[%d, %f] ", graphe[i].ne->succ[j]->num, graphe[i].ne->poids[j]);
      }
      printf("\n");
    }
  }
}

//         TSP SEARCH

void legraphe_tsp_deep_aux(legraphe graphe, ptnoeud node, unsigned short * visitedCities, unsigned short * resultCities, long double * weight, unsigned short currentPos, unsigned short option, clock_t t){

  long double currentWeight;
  //float choiceWeight = FLT_MAX;
  //unsigned short choice;
  unsigned short i, j, k;
  unsigned short * choiceVec;
  clock_t elapsed;


  //printf("%d\n", node->num);accéder au node
  //printf("%d\n", graphe[node->num].ne->num); accéder au lesnoeuds correspondant
  visitedCities[currentPos] = node->num;
  graphe[node->num].vu = 1;

/*  printf("\n");
  for(i=0; i<40; i++){
    if(visitedCities[i]){
      printf("%d|", visitedCities[i]);
    clock_t t;
  t = clock();}
  }*/

  if(is_full_vec(visitedCities, 40)){
    for(i=0; i<node->nbs; i++){
      if(node->succ[i]->num == visitedCities[0]){
        currentWeight = legraphe_calculate_weight(graphe, visitedCities, 40);
        elapsed = clock() - t;
        printf("\nTime : %ld s | Solution found : Weight : %LF\n", elapsed/CLOCKS_PER_SEC, currentWeight);
        for(i=0 ; i<40; i++){
          printf("%d|",visitedCities[i]);
        }
        if(currentWeight<*weight){
          *weight = currentWeight;
          copy_vec(visitedCities, resultCities, 40);
          printf(" -> NEW OPTIMAL\n");
          return;
        }
        break;
      }
    }
  }

  if(!option){
    for(i=0; i<node->nbs; i++){
      if(!graphe[node->succ[i]->num].vu)
        legraphe_tsp_deep_aux(graphe, node->succ[i], visitedCities, resultCities, weight, currentPos+1, option, t);
    }
  }

  else{
    for(i=0, j=0; i<node->nbs; i++){
      if(!graphe[node->succ[i]->num].vu)
        j++;
    }
    if(j){
      choiceVec = malloc(sizeof(unsigned short)*j);

      if(option == 1){
        for(i=0, k=0; k<j; i++){
          if(!graphe[node->succ[i]->num].vu){
            choiceVec[k]=i;
            k++;
          }
        }
      }

      //if option == 2

      for(i=0; i<j; i++){
        legraphe_tsp_deep_aux(graphe, node->succ[choiceVec[i]], visitedCities, resultCities, weight, currentPos+1, option, t);
      }
      free(choiceVec);
    }
  }

  graphe[node->num].vu = 0;
  visitedCities[currentPos] = 0;
  return;
}

void legraphe_tsp_deep(legraphe graphe, unsigned short * resultCities, long double * weight, unsigned short option){

  unsigned short * startCities, * visitedCities;
  unsigned short i, j;

  clock_t t;
  t = clock();

  visitedCities = calloc(40, sizeof(unsigned short));
  assert(visitedCities);
  startCities = malloc(sizeof(unsigned short)*40);
  assert(startCities);

  for(i=0, j=0; i<80; i++){
    if(graphe[i].ne){
      startCities[j] = i;
      j++;
    }
  }
  shuffle_vec(startCities, 40);

  for(i=0; i<40; i++){
    legraphe_tsp_deep_aux(graphe, graphe[startCities[i]].ne, visitedCities, resultCities, weight, 0, option, t);
  }

  free(visitedCities);
  free(startCities);
}

void legraphe_tsp_random_aux(legraphe graphe, ptnoeud node, unsigned short * visitedCities, unsigned short * resultCities, long double * weight, unsigned short currentPos, clock_t t){

  long double currentWeight;
  unsigned short i,j;
  clock_t elapsed;

  /*printf("\n");
  for(i=0; i<40; i++){
    if(visitedCities[i])
    printf("%d|", visitedCities[i]);
  }*/

  visitedCities[currentPos] = node->num;
  graphe[node->num].vu = 1;

  if(is_full_vec(visitedCities, 40)){
    for(i=0; i<node->nbs; i++){
      if(node->succ[i]->num == visitedCities[0]){
        currentWeight = legraphe_calculate_weight(graphe, visitedCities, 40);
        elapsed = clock() - t;
        printf("\nTime : %ld s | Solution found : Weight : %LF\n", elapsed/CLOCKS_PER_SEC, currentWeight);
        for(i=0 ; i<40; i++){
          printf("%d|",visitedCities[i]);
        }
        if(currentWeight<*weight){
          *weight = currentWeight;
          copy_vec(visitedCities, resultCities, 40);
          printf(" -> NEW OPTIMAL\n");
        }
      }
    }
    return;
  }

  for(i=0, j=0; i<node->nbs; i++){
    if(!graphe[node->succ[i]->num].vu)
      j++;
  }
  if(j){
    j = rand()%j+1;
    for(i=0; j>0; i++){
      if(!graphe[node->succ[i]->num].vu){
        j--;
      }
    }
    return legraphe_tsp_random_aux(graphe, node->succ[i-1], visitedCities, resultCities, weight, currentPos+1, t);
  }
}

void legraphe_tsp_random(legraphe graphe, unsigned short * resultCities, long double * weight){

  unsigned short * visitedCities;
  unsigned short i, j, choice;
  clock_t t;
  t = clock();

  visitedCities = calloc(40, sizeof(unsigned short));
  assert(visitedCities);

  srand(time(NULL));
  while(1){
    clear_vec(visitedCities, 40);
    for(i=0; i<80; i++){
      if(graphe[i].ne)
        graphe[i].vu = 0;
    }
    choice = rand()%40+1;
    for(i=0, j=0; j<choice; i++){
      if(graphe[i].ne)
        j++;
    }
    legraphe_tsp_random_aux(graphe, graphe[i-1].ne, visitedCities, resultCities, weight, 0, t);
  }

  free(visitedCities);
}


//MAIN////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv){

  int in;//stdin
  int step, choice, random, sec;//for choice purpose
  long double weight = LDBL_MAX;//best path weight reference
  unsigned short * resultCities;//best path cities reference
  resultCities = calloc(40, sizeof(unsigned short));
  assert(resultCities);

  //file parsing and creation of cities element
  cities cities;
  parse_cities(cities);
  display_cities(cities);

  //User choices
  step = 0;
  printf("\n\n\n");
  while(step<3){
    if(step == 0){
      printf("Veuillez choisir entre l'utilisation de la matrice compacte(tapez '0') ou celle du vecteur de successeur(taper '1') : ");
      scanf("%d", &in);
      choice = in;
      step++;
    }
    if(step == 1){
      printf("Veuillez choisir entre la recherche exhaustive en profondeur(tapez '0') ou la recherche aléatoire(taper '1') : ");
      scanf("%d", &in);
      random = in;
      step++;
    }
    else if(step == 2){
      printf("Veuillez choisir le temps d'éxécution -> 0 : infini | x : temps en secondes : ");
      scanf("%d", &in);
      sec = in;
      step++;
    }
  }

  if(!choice){//gramaco resolution
    //creation of gramaco element from cities
    gramaco_t gramaco = create_gramaco(cities);
    display_gramaco(gramaco);

    if(sec){//time before program stop
      signal(SIGALRM, exitfunc);
      alarm(sec);
    }

    if(!random){//deep search
      gramaco_tsp_deep(&gramaco, resultCities, &weight, 0);
    }
    else{//random search
      gramaco_tsp_random(&gramaco, resultCities, &weight);
    }
  }

  else{//graph resolution
    //creation of legraphe element from cities
    legraphe graphe;
    create_graphe(cities,graphe);
    display_graphe(graphe);

    if(sec){//time before program stop
      signal(SIGALRM, exitfunc);
      alarm(sec);
    }

    if(!random){//deep search
      legraphe_tsp_deep(graphe, resultCities, &weight, 0);
    }
    else{//random search
      legraphe_tsp_random(graphe, resultCities, &weight);
    }
  }
  return 0;
}
