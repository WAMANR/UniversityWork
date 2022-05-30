from asyncio.windows_events import NULL
import datetime
from random import Random, random
from statistics import median_grouped
import requests
import re
import csv
from bs4 import BeautifulSoup, ResultSet
from soupsieve import select
import random as rand

#PRE-REQUIRED LIBRAIRIES INSTALLATIONS:
#         ->pip install requests
#         ->pip install bs4


#GLOBALS VARIABLES:


# A template for the player_info dictionary.
player_info_template = {
        'Id' : '',
        'Name' : '',
        'Surname' : '',
        'Number' : '0',
        'Position' : '',
        'Ppg' : '0',
        'Rpg' : '0',
        'Apg' : '0',
        'Height' : '0',
        'Weight' : '0',
        'Country' : '',
        'College' : '',
        'Birthdate' : '00/00/0000',
        'Experience' : '0'  
    }

# A list of positions.

parameter_position_list = ['Guard','Forward','Center']

# A list of countries.
parameter_country_list = ['Angola','Argentina','Australia','Austria','Bahamas','Bosnia and Herzegovina','Brazil','Cameroon','Canada','Colombia',
                          'Croatia','Czech Republic','DRC','Democratic Republic of the Congo','Denmark','Dominican Republic', 'Egypt', 'Finland',
                          'France','Gabon','Georgia','Germany','Greece','Guinea','Israel','Italy','Jamaica','Japan','Latvia','Lithuania','Mali',
                          'Montenegro','New Zealand','Nigeria','Portugal','Republic of the Congo','Saint Lucia','Senegal','Serbia','Slovenia',
                          'South Sudan','Spain','Sudan','Switzerland','Turkey','USA','Ukraine','United Kingdom']

# A list of colleges.
parameter_college_list = ['Alabama','Anadolu Efes','Arizona','Arizona State','Arkansas','Arkansas-Little Rock','Asvel','Auburn','Austin Peay',
                          'Baloncesto Fuenlabrada','Baskonia','Bayern Munich','Baylor','Belmont','Besiktas','Bilbao','Boise State','Boston College',
                          'Bowling Green','Braunschweig','Brose Bamberg','Bucknell','Butler','Cajasol Sevilla','Cal Poly','Cal State-Long Beach',
                          'Cal-Santa Barbara','California','California-Santa Barbara','Cedevita','Central Florida','Chicago St.','Cholet','Cincinnati',
                          'Colorado','Connecticut','Creighton','Crvena zvezda','Davidson','Dayton','DePaul','Denver','Detroit Mercy','Drexel','Duke',
                          'Edgewater HS (FL)','Elan Chalon','Estudiantes','FC Barcelona','Fenerbahce','Filathlitikos','Florida','Florida Gulf Coast',
                          'Florida State','France','Fresno State','George Washington','Georgetown','Georgia','Georgia Tech','Gonzaga','Houston','IMG Academy (FL)',
                          'Illawarra','Illinois','Indiana','Indiana-Purdue Fort Wayne','Indiana-Purdue Indianapolis','Iona','Iowa','Iowa State','John A. Logan',
                          'Kansas','Kansas State','Kentucky','La Salle','Le Mans','Lehigh','Lietuvos rytas Vilnius','Lincoln Memorial','Lipscomb','Louisana-Lafayette',
                          'Louisiana State','Louisiana Tech','Louisville','Loyola-Maryland','Maccabi Tel Aviv','Marquette','Marshall','Maryland','Mega Basket','Memphis',
                          'Miami','Michigan','Michigan State','Minnesota','Mississippi','Mississippi State','Missouri','Missouri State','Molloy','Montana State','Morehead State',
                          'Murcia','Murray State','NBA G League Ignite','NBA Global Academy','Nebraska','Nebraska-Lincoln','Nevada','Nevada-Reno','New Mexico','New Mexico State',
                          'New Zealand Breakers','No College','North Carolina','North Carolina State','North Carolina-Wilmington','Notre Dame','Oakland','Ohio','Ohio State',
                          'Oklahoma','Oklahoma State','Old Dominion','Olimpia Milano','Olympiacos','Oregon','Oregon State','Panathinaikos','Penn State','Pepperdine','Pittsburgh',
                          'Poitiers Basket 86','Princeton','Princeton HS (OH)','Providence','Purdue','Radford','Ratiopharm Ulm','Real Madrid','Rhode Island','Ricoh Manresa',
                          'SW Atlanta Christian Academy (GA)','San Diego State','San Pablo Burgos','Seton Hall','Skyliners Frankfurt','South Carolina','South Carolina Upstate',
                          'South Gwinnett HS (GA)','Southern California','Southern Methodist','St. John\'s', 'St. Joseph\'s (PA)','St. Louis','St. Mary\'s College',
                          'St. Vincent-St. Mary HS (OH)','St.Mary\'s College of California','Stanford','Strasbourg IG','Sydney Kings','Syracuse','TCU','Tennessee',
                          'Tennessee State','Tennessee-Chattanooga','Texas A&M','Texas Tech','Texas-Austin','Truman State','Tulane','Tulsa','UCLA','UNLV','Union Olimpija',
                          'University of Colorado Boulder','Utah','Utah State','Vanderbilt','Vermont','Villanova','Virginia','Virginia Commonwealth','Virginia Tech',
                          'Wake Forest','Washington','Washington State','Weber State','West Virginia','Western Kentucky','Wheeling Jesuit','Wichita State','William & Mary',
                          'Wisc.-Green Bay','Wisconsin','Wyoming','Xavier','Yale','Zalgiris','Zaragoza']

### URL FILTERS

print("\n-----------BIENVENUE, VEUILLEZ BIEN LIRE LE README AFIN DE COMPRENDRE L'UTILISATION DE CE PROGRAMME-----------")
print("\nVeuillez ajouter des paramètres à votre requête ou la laisser vide.\nRequest : ")

request_input = input()
### FIND COUNTRY ARGUMENT
country = re.search('country=(.[^\|]+)\|', request_input)
if(country is not None):
    country = country.group(1).strip()
if(country is None or country not in parameter_country_list):
    country = ""

### FIND COLLEGE ARGUMENT
college = re.search('college=(.[^\|]+)\|', request_input)
if(college is not None):
    college = college.group(1).strip()
if(college is None or college not in parameter_college_list):
    college = ""
    
### FIND DRAFT YEAR ARGUMENT
draftYear = re.search('draftyear=([0-9]{4})', request_input)
if(draftYear is not None):
    draftYear = draftYear.group(1).strip()
if(draftYear is None or not (int(draftYear)>1946 and int(draftYear)<2022)):
    draftYear = ""
    
### FIND HISTORICAL ARGUMENT
historical = re.search('historical=([0-1]{1})', request_input)
if(historical is not None):
    historical = historical.group(1)
else:
    historical = ""
    
### FIND LIMIT ARGUMENT
limit_number = re.search('limit=([0-9]+)', request_input)
if(limit_number is not None):
    limit_number = int(limit_number.group(1).strip())
    limit_number_cp = limit_number
    limit = True
else:
    limit = False
    limit_number_cp = ""


### ERROR RAISED
if(country == college == draftYear == historical == "" and not limit and request_input != ""):
    raise ValueError("Please type valid arguments : \"parameter=value|\" \nParameters list : \"country\", \"college\", \"draftyear\", \"historical\"\nExemple : country=France| historical=1 | college=Asvel| \nAlso check the valid values of parameters.")

if (historical == "0"):
    historical = ""


player_list_url = "https://stats.nba.com/stats/playerindex?College="+college+"&Country="+country+"&DraftPick=&DraftRound=&DraftYear="+draftYear+"&Height=&Historical="+historical+"&LeagueID=00&Season=2021-22&SeasonType=RegularSeason&TeamID=0&Weight=215"


### URL FILTERS


headers  = {
    'Connection': 'keep-alive',
    'Accept': 'application/json, text/plain, */*',
    'x-nba-stats-token': 'true',
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36',
    'x-nba-stats-origin': 'stats',
    'Sec-Fetch-Site': 'same-origin',
    'Sec-Fetch-Mode': 'cors',
    'Referer': 'https://stats.nba.com/',
    'Accept-Encoding': 'gzip, deflate, br',
    'Accept-Language': 'en-US,en;q=0.9',
}

######GET THE PLAYERS URL LIST

request = requests.get(url=player_list_url, headers=headers)

if not request.ok:
    raise ValueError("Exception HTTP player_list_request error.")

else:
    print("HTTP GET player_list_request successful.")
    
    player_list_request = request.json()
    player_list_index = []
    player_list = []

    for p in player_list_request["resultSets"][0]["rowSet"]:
        
        ### request_input LIMIT
        if(limit):
            if(not limit_number):
                break
            limit_number -= 1
            
        player_list_index.append("https://www.nba.com/stats/player/" + str(p[0]) + "/career")
        
    #######FOR EACH PLAYERS URL -> EXTRACT PLAYER STATS
    
    id = 0
        
    for p in player_list_index:
        
        ###PLAYER GENERAL INFORMATIONS
        
        player_info = player_info_template

        player_request = requests.get(url=p, headers=headers)
        player_soup = BeautifulSoup(player_request.text, "html.parser")
        
        player_info["Id"] = id
        
        nameAndSurname = player_soup.findAll("p", {"class" : "PlayerSummary_playerNameText__K7ZXO"})
        if(len(nameAndSurname)):
            player_info["Name"] = nameAndSurname[0].getText()
            player_info["Surname"] = nameAndSurname[1].getText()
        
        
        numberAndPostion = player_soup.find("p", {"class": "t11 md:t2"})
        if(numberAndPostion is not None):
            numberVerification = re.search('#([0-9]+)', numberAndPostion.getText())
            if(numberVerification is not None):
                player_info["Number"] = numberVerification.group(1)
                
            positionVerification = re.search('([^\s][a-zA-Z-]+)$',numberAndPostion.getText())#CAN BE IMPROVE
            if(positionVerification is not None):
                player_info["Position"] = positionVerification.group(1)
            
        stats = player_soup.findAll("p", {"class" : "PlayerSummary_playerStatValue__3hvQY"})
        if(len(stats)):
            if(stats[0].getText() == '--'):
                player_info["Ppg"] = '0'
            else:    
                player_info["Ppg"] = stats[0].getText()
            if(stats[1].getText() == '--'):
                player_info["Rpg"] = '0'
            else:    
                player_info["Rpg"] = stats[1].getText()
            if(stats[2].getText() == '--'):
                player_info["Apg"] = '0'
            else:    
                player_info["Apg"] = stats[2].getText()
        
        extras = player_soup.findAll("p", {"class" : "PlayerSummary_playerInfoValue__mSfou"})
        if(len(extras)):
            height = re.search('^[0-9]\'[0-9]{1,2}"\s\(([0-9]{1}[.][0-9]{1,2})m\)$', extras[0].getText())
            weight = re.search('^[0-9]{3}lb\s\(([0-9]{2,3})kg\)', extras[1].getText())
            if(height is not None):
                player_info["Height"] = height.group(1).strip()
            if(weight is not None):
                player_info["Weight"] = weight.group(1).strip()
            player_info["Country"] = extras[2].getText()
            player_info["College"] = extras[3].getText()
            
            
            if(re.search('(,)',extras[4].getText())):
                date = re.findall('([^\s,]+)', extras[4].getText())
                player_info_experience_tmp = [int(s) for s in extras[6].getText().split() if s.isdigit()]
            else:
                date = re.findall('([^\s,]+)', extras[5].getText())
                player_info_experience_tmp = [int(s) for s in extras[7].getText().split() if s.isdigit()]
                
            if(len(player_info_experience_tmp) == 0):
                player_info_experience_tmp = 0
            else:
                player_info_experience_tmp = player_info_experience_tmp[0]
            
            player_info["Experience"] = player_info_experience_tmp
                
            datetime_object = datetime.datetime.strptime(date[0], "%B")
            
            birthdate = datetime.datetime(year=int(date[2]), month=int(datetime_object.month), day=int(date[1]))
            
            player_info["Birthdate"] = birthdate
        
       # print(player_info)
        
        player_list.append(player_info.copy())
        print(id)
        
        id += 1
        
    #####DATA SET IS READY
    
    print("\nLe dataset est prêt à être utilisé.\n")
    
    result = []
    
    while(1): 
        
        print("\nVeuillez ajouter des paramètres de selection ou ne pas en ajouter pour choisir toute la table.\nSelection : ")
        
        selection_input = input()
        
        ### Request construction
        
        #Select Request ->   
        
        clause_list= []
        
        ####COLLECT INPUT REQUESTS
        #number
        select_number = re.findall('(Number)([=]{2}|[<>!]|!=)([0-9]+)', selection_input)
        if(len(select_number)):
            clause_list.append(select_number)
        #Ppg
        select_ppg = re.findall('(Ppg)([=]{2}|[<>!]|!=)([0-9]{1}[.]?[0-9]?)', selection_input)
        if(len(select_ppg)):
            clause_list.append(select_ppg)
        #Rpg
        select_rpg = re.findall('(Rpg)([=]{2}|[<>!]|!=)([0-9]{1}[.]?[0-9]?)', selection_input)
        if(len(select_rpg)):
            clause_list.append(select_rpg)
        #Ppg
        select_apg = re.findall('(Apg)([=]{2}|[<>!]|!=)([0-9]{1}[.]?[0-9]?)', selection_input)
        if(len(select_apg)):
            clause_list.append(select_apg)
        #Rpg
        select_height = re.findall('(Height)([=]{2}|[<>!]|!=)([0-9]{1}[.]{1}[0-9]{1,2}|[0-9]{1})', selection_input)
        if(len(select_height)):
            clause_list.append(select_height)
        #Ppg
        select_weight = re.findall('(Weight)([=]{2}|[<>!]|!=)([0-9]{2,3})', selection_input)
        if(len(select_weight)):
            clause_list.append(select_weight)
        #birthdate
        select_birthdate = re.findall('(Birthdate)([=]{2}|[<>!]|!=)([0-9]{2}\/[0-9]{2}\/[0-9]{4})', selection_input)
        if(len(select_birthdate)):
            clause_list.append(select_birthdate)
        #experience
        select_experience = re.findall('(Experience)([=]{2}|[<>!]|!=)([0-9]{1,2})', selection_input)
        if(len(select_experience)):
            clause_list.append(select_experience)
        #name
        select_name = re.findall('(Name)([([=]{2}|!=)(.[^|]+)\|', selection_input)
        if(len(select_name)):
            clause_list.append(select_name)
        #surname
        select_surname = re.findall('(Surname)([([=]{2}|!=)(.[^|]+)\|', selection_input)
        if(len(select_surname)):
            clause_list.append(select_surname)
        #position
        select_position = []
        for pos in parameter_position_list:
            select_position_tmp = re.findall("(Position)([=]{2}|!=)("+pos+")", selection_input)
            if(len(select_position_tmp)):
                select_position.extend(select_position_tmp)
                clause_list.append(select_position)
        #country
        select_country = []
        if(re.match("(Country)([=]{2}|!=)([a-zA-Z][^|]+)", selection_input)):
            for pos in parameter_country_list:
                select_country_tmp = re.findall("(Country)([=]{2}|!=)("+pos+")", selection_input)
                if(len(select_country_tmp)):
                    select_country.extend(select_country_tmp)
                    clause_list.append(select_country)
        #college
        select_college = []
        if(re.match("(College)([=]{2}|!=)([a-zA-Z][^|]+)", selection_input)):
            for pos in parameter_college_list:
                select_college_tmp = re.findall("(College)([=]{2}|!=)("+pos+")", selection_input)
                if(len(select_college_tmp)):
                    select_college.extend(select_college_tmp)
                    clause_list.append(select_college)
                
        if(not len(clause_list) and selection_input != ""):
            
            print("\nVeuillez entrer des paramètres valides.\n")
            
        ### Do the selection
        else: 
            
            print("\nArguments valides. Collection en cours de création.\n")
            
            result_clause = {"request" : "Country-"+country+"-College-"+college+"-DraftYear-"+draftYear+"-Historical-"+historical+"-Limit-"+str(limit_number_cp), "clause" : clause_list, "rowset" : []}   
            
            for player in player_list:
                
                format_str = '%d/%m/%Y'
                
                alreadyIn = False
                if(len(result)):
                    for r in result_clause["rowset"]:
                        if(player["Id"] == r["Id"]):
                            alreadyIn = True
                if(alreadyIn):
                    continue
                
                noMatch=False                
                for clause_type in clause_list:
                    for clause in clause_type:
                        if(clause[0] == "Position"):
                            if(clause[1] == "==" and clause[2] not in player[clause[0]]):
                                noMatch=True
                                break
                            elif(clause[1] == "!=" and clause[2] in player[clause[0]]):
                                noMatch=True
                                break
                            
                        elif(clause[0] == "Birthdate"):
                            dateTimeCompare = datetime.datetime.strptime(clause[2], format_str)
                            if(clause[1] == "==" and dateTimeCompare != player["Birthdate"]):
                                noMatch=True
                                break
                            elif(clause[1] == "!=" and dateTimeCompare == player["Birthdate"]):
                                noMatch=True
                                break
                            elif(clause[1] == ">" and dateTimeCompare > player["Birthdate"]):
                                noMatch=True
                                break
                            elif(clause[1] == "<" and dateTimeCompare < player["Birthdate"]):
                                noMatch=True
                                break
                        
                        elif(clause[0] == "Name" or clause[0] == "Surname" or clause[0] == "Country" or clause[0] == "College"):
                            if(clause[1] == "==" and clause[2] != player[clause[0]]):
                                noMatch=True
                                break
                            elif(clause[1] == "!=" and clause[2] == player[clause[0]]):
                                noMatch=True
                                break
                        
                        else:   
                            if not eval(player[clause[0]] + clause[1] + clause[2]):
                                noMatch=True
                                break
                    if(noMatch):
                        break
                if(noMatch):
                    continue
                                                 
                result_clause["rowset"].append(player)
                
            exist=0
                
            if(len(result_clause["rowset"])):
                for i in range(len(result)):
                    if(result[i]["request"] == result_clause["request"] and result[i]["clause"] == result_clause["clause"]):
                        exist = 1
                        break
                    
                if(not exist):
                    result.append(result_clause.copy()) 
                    print("\nCollection crée.\n")
                else:
                    print("\nLa collection éxiste déjà.\n")
                    continue
            else:
                print("\nPas de correspondance avec un joueur trouvée.\n")
                continue
            
            
            while(1):
            
                print("\nVoulez vous ajouter une autre collection? Tapez y ou n\nRéponse:")
                add_input = input()
                if(add_input == "y" or add_input == "n"):
                    break
            
            if(add_input == 'n'):
                break
            
    ####EXPORT TABLES CSV - COLLECTIONS
    
    while(1):
            
        print("\nVoulez vous exportez vos collections en fichiers CSV? Entrez y ou n\nRéponse:")
        save_input = input()
        if(save_input == "y" or save_input == "n"):
            break
        print("Veuillez entrer une réponse valide.")
        
    if(save_input == "y"):
    
        for table in result:
            player_info_csv = ['Id','Name','Surname','Number','Position','Ppg','Rpg','Apg','Height','Weight','Country','College','Birthdate','Experience']   
            
            table_request = ""
            for i in table["clause"]:
                for j in i:
                    for k in range(len(j)):
                        if(k==1):
                            if(j[k] == "=="):
                                table_request += "EQUAL"
                            elif(j[k] == "!="):
                                table_request += "DIFF"
                            elif(j[k] == ">"):
                                table_request += "SUP"
                            elif(j[k] == "<"):
                                table_request += "INF"
                            else:
                                table_request += j[k]
                        elif(j[0] == "Birthdate" and k==2):
                            birthdate_table_request = re.findall('^([0-9]{2})\/([0-9]{2})\/([0-9]{4})$', j[k])
                            table_request += birthdate_table_request[0][0]+"-"
                            table_request += birthdate_table_request[0][1]+"-"
                            table_request += birthdate_table_request[0][2]
                        else:
                            table_request += j[k]
                    table_request+="-"
                    
            
            with open("./collections/"+"CLAUSES-"+table_request+"REQUEST-"+table["request"]+".csv", 'w') as csvfile:
                writer = csv.DictWriter(csvfile, delimiter=';',fieldnames=player_info_csv)
                writer.writeheader()
                writer.writerows(table["rowset"])  

   
    while(1):
            
        print("\nVoulez vous créer une statistique selon un des attributs des joueurs? Entrez y ou n\nRéponse:")
        stats_input = input()
        if(stats_input == "y" or stats_input == "n"):
            break
        print("Veuillez entrer une réponse valide.")
        
    #### DO SOME STATS
        
    if(stats_input == "y"):
                     
        result_s = []
                
        while(1):
            
            print("\nVeuillez entrer l'attribut selon lequel vous voulez créer une statistique :")
        
            while(1):
                
                stats_attribute = input()
                
                if(stats_attribute == "Ppg" or stats_attribute == "Rpg" or stats_attribute == "Apg" or stats_attribute == "Height" or stats_attribute == "Weight" or stats_attribute == "Birthdate" or stats_attribute == "Experience"):
                    break
                    
                elif(stats_attribute == "Position" or stats_attribute == "Country" or stats_attribute == "College"):
                    break
                    
                else:
                    print("Veuillez entrer un attribut valide.")
                    print("Liste : Ppg, Rpg, Apg, Height, Weight, Birthdate, Experience, Position, Country, College")
                
            result_stats = {"stats" : stats_attribute , "rowset" : []}
            
            for table in result:
                
                table_request = ""
                for i in table["clause"]:
                    for j in i:
                        for k in j:
                                table_request += k
                        table_request+="|"
                
                if(stats_attribute == "Ppg" or stats_attribute == "Rpg" or stats_attribute == "Apg" or stats_attribute == "Height" or stats_attribute == "Weight" or stats_attribute == "Birthdate" or stats_attribute == "Experience"):
                    result_stats_table = {"Name" : "Stats->"+stats_attribute+"|CLAUSES->"+table_request+"|REQUEST->"+table["request"] , "Min" : "", "Max" : "", "Quartile 1" : "", "Mediane" : "", "Quartile 3" : "", "Moyenne" : ""}
                    #MIN
                    if(stats_attribute == "Birthdate"):
                        min = datetime.date(3000,1,1)
                    else:
                        min = 100000
                    for player in table["rowset"]:
                        if(stats_attribute == "Birthdate"):
                            if(player[stats_attribute].date()<min):
                                min = player[stats_attribute].date()
                        else:
                            if(float(player[stats_attribute])<min):
                                min = float(player[stats_attribute])
                    result_stats_table["Min"] = min
                    #Max
                    if(stats_attribute == "Birthdate"):
                        max = datetime.date(1800,1,1)
                    else:
                        max = 0
                    for player in table["rowset"]:
                        if(stats_attribute == "Birthdate"):
                            if(player[stats_attribute].date()>max):
                                max = player[stats_attribute].date()
                        else:
                            if(float(player[stats_attribute])>max):
                                max = float(player[stats_attribute])
                    result_stats_table["Max"] = max
                    #Quartiles and mediane
                    player_stats_list = []
                    for i in table["rowset"]:
                        if(stats_attribute == "Birthdate"):
                            player_stats_list.append(i[stats_attribute].date())
                        else:
                            player_stats_list.append(float(i[stats_attribute]))
                    player_stats_list.sort()
                    
                    if(stats_attribute == "Birthdate"):
                        quartile1 = quartile3 = mediane = datetime.date(1800,1,1)
                    else:
                        quartile1 = quartile3 = mediane = 0

                    for i in range(len(player_stats_list)):
                        if(stats_attribute == "Birthdate"):
                            if(quartile1 == datetime.date(1800,1,1) and i>= len(player_stats_list)/4):
                                quartile1 = player_stats_list[i]
                            if(mediane == datetime.date(1800,1,1) and i>= len(player_stats_list)/2):
                                mediane = player_stats_list[i]
                            if(quartile3 == datetime.date(1800,1,1) and i>= len(player_stats_list)*0.75):
                                quartile3 = player_stats_list[i]
                        else:
                            if(not quartile1 and i>= len(player_stats_list)/4):
                                quartile1 = player_stats_list[i]
                            if(not mediane and i>= len(player_stats_list)/2):
                                mediane = player_stats_list[i]
                            if(not quartile3 and i>= len(player_stats_list)*0.75):
                                quartile3 = player_stats_list[i]
                    result_stats_table["Quartile 1"] = quartile1
                    result_stats_table["Mediane"] = mediane
                    result_stats_table["Quartile 3"] = quartile3
                    #Average
                    if(stats_attribute == "Birthdate"):
                        result_stats_table["Moyenne"] = 0
                    else:
                        average = 0
                        for player in table["rowset"]:
                            average += float(player[stats_attribute])
                        average = average / len(table["rowset"])
                        result_stats_table["Moyenne"] = average
                        
                    result_stats["rowset"].append(result_stats_table)
                        
                elif(stats_attribute == "Position"):
                    result_stats_table = {"Name" : "Stats->"+stats_attribute+"CLAUSES->"+table_request+"REQUEST->"+table["request"] , "Guard" : "", "Guard%" : "", "Forward" : "", "Forward%" : "", "Center" : "", "Center%" : ""}
                    #Guard
                    guard=0
                    for player in table["rowset"]:
                        if("Guard" in player["Position"]):
                            guard += 1
                    result_stats_table["Guard"] = guard
                    result_stats_table["Guard%"] = (guard*10)/len(table)
                    #Forward
                    forward=0
                    for player in table["rowset"]:
                        if("Forward" in player["Position"]):
                            forward += 1
                    result_stats_table["Forward"] = forward
                    result_stats_table["Forward%"] = (forward*10)/len(table)
                    #Center
                    center=0
                    for player in table["rowset"]:
                        if("Center" in player["Position"]):
                            center += 1
                    result_stats_table["Center"] = center
                    result_stats_table["Center%"] = (center*10)/len(table)
                    result_stats["rowset"].append(result_stats_table)
                    
                    
                else:
                    result_stats_table = {"Name" : "Stats->"+stats_attribute+"CLAUSES->"+table_request+"REQUEST->"+table["request"]}
                    visited_name = []
                    for player in table["rowset"]:
                        if(player[stats_attribute] not in visited_name):
                            count = 0
                            visited_name.append(player[stats_attribute])
                            for p in table["rowset"]:
                                if p[stats_attribute] == player[stats_attribute]:
                                    count += 1
                        result_stats_table[player[stats_attribute]] = count
                    result_stats_table["Element"] = visited_name
                    result_stats["rowset"].append(result_stats_table)
                                
                    
            print("\nLa statistique à été crée.\n")
            result_s.append(result_stats)
                
            while(1):
                    
                print("\nVoulez vous faire une statistique sur un autre attribut? Tapez y ou n\nRéponse:")
                add_input = input()
                if(add_input == "y" or add_input == "n"):
                    break
                    
            if(add_input == 'n'):
                break
            
    ####EXPORT TABLES CSV - STATS
    
    while(1):
            
        print("\nVoulez vous exportez vos statistiques en fichiers CSV? Entrez y ou n\nRéponse:")
        save_input = input()
        if(save_input == "y" or save_input == "n"):
            break
        print("Veuillez entrer une réponse valide.")
        
    if(save_input == "y"):
        
        for table in result_s:
            
            if(table["stats"] == "Ppg" or table["stats"] == "Rpg" or table["stats"] == "Apg" or table["stats"] == "Height" or table["stats"] == "Weight" or table["stats"] == "Birthdate" or table["stats"] == "Experience"):
                stats_info_csv = ["Name", "Min", "Max", "Quartile 1", "Mediane", "Quartile 3", "Moyenne"]
            
            elif(table["stats"] == "Position"):
                stats_info_csv = ["Name", "Guard", "Guard%", "Forward", "Forward%", "Center", "Center%"]
                
            else:
                stats_info_csv = ["Name", "Element"]
                for t in table["rowset"]:
                    for elem in t["Element"]:
                        if(elem not in stats_info_csv):
                            stats_info_csv.append(elem)
                        
            
            fileId = str(rand.randint(0, 1000))                
            with open("./stats/"+"Stats-"+table["stats"]+fileId+".csv", 'w') as csvfile:
                writer = csv.DictWriter(csvfile, delimiter=';', fieldnames=stats_info_csv)
                writer.writeheader()
                writer.writerows(table["rowset"])  
            
                    
            
        
                
                
        
        
        
        
        
    print("Thanks. Bye!")