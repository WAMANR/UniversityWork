trace=False
#############################CASE#########################################

class CaseLab :
    
    def __init__(self, a, b):
        #coordonnées
        self.x=a
        self.y=b
    
        #accessibilitée
        self.N=1
        self.O=1
        self.S=1
        self.E=1
        
        #heurisitique
        self.coup=0
        self.heuristique=0

        
#############################LABYRINTHE####################################


class Labyrinthe:

    

    def __init__(self):

        self.chemin=[]
        self.i=0
        self.fileEtat=[]
        self.etatVisite=[]
        self.start=[0,0]
        self.stop=[1,1]
        self.labyrinthe=[]
        self.height=10
        self.length=10
        for y in range(self.height):
            for x in range (self.length):
                self.labyrinthe.append(CaseLab(x, y))


    def afficher(self):
        
        for y in range(self.height):
            line = "|"
            for x in range (self.length):
                case = self.labyrinthe[x + y*self.length]
                line+= " "+str(case.x)+"/"+str(case.y)#+"/"+str(case.S)+"/"+str(case.E)
            line += " |"
            print(line)


    def bordure(self):

        for x in range(len(self.labyrinthe)):
            
            if(x%self.length==0):
                self.labyrinthe[x].O=0
                
            if(x%self.length==self.length-1):
                self.labyrinthe[x].E=0
                
            if(x-self.length<0):
                self.labyrinthe[x].N=0
                
            if(x-(self.length*(self.height-1))>=0):
                self.labyrinthe[x].S=0


#d-->direction : N=0,O=1,S=2,E=3

    def consMur(self, x, y, d):
        
        if(d==0):
            self.labyrinthe[x+y*self.length].N=0
            if(y>0):
                self.labyrinthe[x+(y-1)*self.length].S=0
    
        if(d==2):
            self.labyrinthe[x+y*self.length].S=0
            if(y<self.height-1):
                self.labyrinthe[x+(y+1)*self.length].N=0

        if (d==1):
            self.labyrinthe[x+y*self.length].O=0
            if(x>0):
                self.labyrinthe[(x-1)+y*self.length].E=0

        if(d==3):
            self.labyrinthe[x+y*self.length].E=0
            if(x<self.length-1):
                self.labyrinthe[(x+1)+y*self.length].O=0


    def setStart(self,a,b):
        if a>self.length-1 or b>self.height-1 or a<0 or b<0:
            print('Coordonnées start hors du labyrinthe')

        else:
            self.start=[a,b]

    def setStop(self,a,b):
        if a>self.length-1 or b>self.height-1 or a<0 or b<0:
            print('Coordonnées stop hors du labyrinthe')

        else:
            self.stop=[a,b]

            

    def heuristique(self, a):
        
        if a.x>self.stop[0]:
            difference_x=a.x-self.stop[0]
        else:
            difference_x=self.stop[0]-a.x

        if a.y>self.stop[1]:
            difference_y=a.y-self.stop[1]
        else:
            difference_y=self.stop[1]-a.y

        return difference_x + difference_y
    

    def getCase(self,x,y):
        return self.labyrinthe[x+y*self.length]
        


    def resolution(self,case):


        if case == self.stop:
            print('CHEMIN TROUVE')
            self.etatVisite.append(case)
            if trace :
                print('ETAT VISITE FINAL', self.etatVisite)

            indice=len(self.etatVisite)-1
            coup_etat_suivant=self.getCase(self.etatVisite[indice][0],self.etatVisite[indice][1]).coup
            self.chemin.append([self.etatVisite[indice][0], self.etatVisite[indice][1]])
            self.chemin.append([self.etatVisite[indice-1][0], self.etatVisite[indice-1][1]])
            
            while(indice>=1):
                indice-=1
                if trace :
                    print('TEST :',self.getCase(self.etatVisite[indice][0],self.etatVisite[indice][1]).coup)
                if self.getCase(self.etatVisite[indice][0],self.etatVisite[indice][1]).coup==coup_etat_suivant  :
                    self.chemin.append([self.etatVisite[indice][0],self.etatVisite[indice][1]])
                    coup_etat_suivant-=1
                if trace :
                    print('indice :',indice,' coup :',coup_etat_suivant,' CHEMIN', self.chemin)

            self.chemin.reverse()
            print('CHEMIN :', self.chemin)
        
                
                
                
            
            return 1

        if trace:
            print('\nBOUCLE :\n')

        etat=self.getCase(case[0],case[1])
        if trace :
            print('case actuelle =',etat.x,'/',etat.y)

        if self.i != 0:
            if len(self.fileEtat)==0:
                print('AUCUNE CHEMIN POSSIBLE JUSQU\'A L\'ARRIVEE')
                return 0
            self.i=1

            if trace :
                print('fileEtat avant ajout (etat :',case,' )', self.fileEtat)
                print('len file etat = ', len(self.fileEtat))

        for indice in range(len(self.fileEtat)):
            if self.fileEtat[indice][0]==case[0] and self.fileEtat[indice][1]==case[1]:
                del self.fileEtat[indice]
                break
                        
        self.etatVisite.append(case)
        if trace :
            print('etatVisite =', self.etatVisite)

        #ajout file Etat en fonction accessibilité

        if etat.N==1:
            nouvelle_case=self.getCase(case[0],case[1]-1)
            nouvelle_case.coup=etat.coup+1
            nouvelle_case.heuristique=self.heuristique(nouvelle_case)+nouvelle_case.coup

            if [case[0],case[1]-1] not in self.etatVisite and [case[0],case[1]-1] not in self.fileEtat:
                self.fileEtat.append([case[0],case[1]-1])
            
            if trace :
                print('nouvelle case N : x=',nouvelle_case.x,' y=', nouvelle_case.y,' coup=', nouvelle_case.coup,' h=', nouvelle_case.heuristique)

        if etat.O==1:
            nouvelle_case=self.getCase(case[0]-1,case[1])
            nouvelle_case.coup=etat.coup+1
            nouvelle_case.heuristique=self.heuristique(nouvelle_case)+nouvelle_case.coup

            if [case[0]-1,case[1]] not in self.etatVisite and [case[0]-1,case[1]] not in self.fileEtat:
                self.fileEtat.append([case[0]-1,case[1]])
            
            if trace:
                print('nouvelle case O : x=',nouvelle_case.x,' y=', nouvelle_case.y,' coup=', nouvelle_case.coup,' h=', nouvelle_case.heuristique)

        if etat.S==1:
            nouvelle_case=self.getCase(case[0],case[1]+1)
            nouvelle_case.coup=etat.coup+1
            nouvelle_case.heuristique=self.heuristique(nouvelle_case)+nouvelle_case.coup

            if [case[0],case[1]+1] not in self.etatVisite and [case[0],case[1]+1] not in self.fileEtat:
                self.fileEtat.append([case[0],case[1]+1])
            
            if trace:
                print('nouvelle case S : x=',nouvelle_case.x,' y=', nouvelle_case.y,' coup=', nouvelle_case.coup,' h=', nouvelle_case.heuristique)

        if etat.E==1:
            nouvelle_case=self.getCase(case[0]+1,case[1])
            nouvelle_case.coup=etat.coup+1
            nouvelle_case.heuristique=self.heuristique(nouvelle_case)+nouvelle_case.coup

            if [case[0]+1,case[1]] not in self.etatVisite and [case[0]+1,case[1]] not in self.fileEtat:
                self.fileEtat.append([case[0]+1,case[1]])

            if trace:
                print('nouvelle case E : x=',nouvelle_case.x,' y=', nouvelle_case.y,' coup=', nouvelle_case.coup,' h=', nouvelle_case.heuristique)

        if trace:
            print('fileEtat après ajout (etat :',case,' )', self.fileEtat)

        #choix etat suivant


        if len(self.fileEtat)==1 and self:
            suivant=self.fileEtat[0]

        heuristique_min=self.getCase(self.fileEtat[0][0],self.fileEtat[0][1]).heuristique
        for indice in range(len(self.fileEtat)):
            if self.getCase(self.fileEtat[indice][0],self.fileEtat[indice][1]).heuristique<heuristique_min:
                heuristique_min=self.getCase(self.fileEtat[indice][0],self.fileEtat[indice][1]).heuristique

        if trace:
            print('heuristique_min',heuristique_min)

        liste_heuristique_min=[]
        for indice in range(len(self.fileEtat)):
            if self.getCase(self.fileEtat[indice][0],self.fileEtat[indice][1]).heuristique==heuristique_min:
                liste_heuristique_min.append([self.fileEtat[indice][0],self.fileEtat[indice][1]])

        if trace:
            print('Liste_heuristique_min=',liste_heuristique_min)

        if len(liste_heuristique_min)==1:
            suivant=liste_heuristique_min[0]
                

        coup_max=self.getCase(liste_heuristique_min[0][0],liste_heuristique_min[0][1]).coup
        for indice in range(len(liste_heuristique_min)):
            if self.getCase(liste_heuristique_min[indice][0],liste_heuristique_min[indice][1]).coup>coup_max:
                coup_max=self.getCase(liste_heuristique_min[indice][0],liste_heuristique_min[indice][1]).coup

        if trace:
            print('coup_max=',coup_max)

        for indice in range(len(liste_heuristique_min)):
            if self.getCase(liste_heuristique_min[indice][0],liste_heuristique_min[indice][1]).coup==coup_max:
                suivant=liste_heuristique_min[indice]

        self.resolution(suivant)

        

        
            
            
            
            
        
            
        

        

        

        

        

        

################################   MAIN   ######################################




            
lab= Labyrinthe()
lab.bordure()
lab.afficher()

#############################################

#CONSTRUCTION LABYRINTHE

lab.setStart(4,0)
lab.setStop(5,9)

lab.consMur(0,3,3)
lab.consMur(0,4,2)
lab.consMur(0,6,3)
lab.consMur(0,7,2)
lab.consMur(1,0,2)
lab.consMur(1,1,2)
lab.consMur(0,1,3)
lab.consMur(1,2,2)
lab.consMur(1,4,2)
lab.consMur(1,4,3)
lab.consMur(1,5,3)
lab.consMur(1,6,2)
lab.consMur(1,7,3)
lab.consMur(1,8,3)
lab.consMur(1,8,2)
lab.consMur(2,0,3)
lab.consMur(2,1,3)
lab.consMur(2,1,2)
lab.consMur(2,3,3)
lab.consMur(2,3,2)
lab.consMur(0,1,3)
lab.consMur(2,5,2)
lab.consMur(2,6,2)
lab.consMur(3,0,3)
lab.consMur(3,1,2)
lab.consMur(3,3,3)
lab.consMur(3,4,2)
lab.consMur(3,5,3)
lab.consMur(3,6,3)
lab.consMur(3,6,2)
lab.consMur(3,7,3)
lab.consMur(3,8,3)
lab.consMur(3,8,2)
lab.consMur(4,0,2)
lab.consMur(4,1,3)
lab.consMur(4,2,3)
lab.consMur(4,3,3)
lab.consMur(4,3,2)
lab.consMur(4,4,3)
lab.consMur(4,5,2)
lab.consMur(4,6,3)
lab.consMur(4,7,3)
lab.consMur(4,7,2)
lab.consMur(4,9,3)
lab.consMur(5,0,3)
lab.consMur(5,1,2)
lab.consMur(5,3,2)
lab.consMur(5,4,2)
lab.consMur(5,5,3)
lab.consMur(5,6,2)
lab.consMur(5,7,2)
lab.consMur(5,8,3)
lab.consMur(6,0,2)
lab.consMur(6,1,2)
lab.consMur(6,2,2)
lab.consMur(6,3,2)
lab.consMur(6,4,2)
lab.consMur(6,6,2)
lab.consMur(6,6,3)
lab.consMur(6,7,3)
lab.consMur(6,7,3)
lab.consMur(6,9,3)
lab.consMur(7,0,3)
lab.consMur(7,1,3)
lab.consMur(7,2,3)
lab.consMur(7,2,2)
lab.consMur(7,3,2)
lab.consMur(7,4,3)
lab.consMur(7,5,2)
lab.consMur(7,5,3)
lab.consMur(7,7,2)
lab.consMur(8,1,3)
lab.consMur(8,1,2)
lab.consMur(8,3,3)
lab.consMur(8,4,2)
lab.consMur(8,5,3)
lab.consMur(8,6,2)
lab.consMur(8,7,2)
lab.consMur(8,8,3)
lab.consMur(8,8,2)
lab.consMur(9,2,2)
lab.consMur(9,6,2)


















lab.resolution(lab.start)

    
    
