from pysc2.agents import base_agent
from pysc2.env import run_loop
from pysc2.env import sc2_env
from pysc2.tests import utils
from absl.testing import absltest
from pysc2.lib import actions, features, units
import numpy as np
import empty as emp
import random as rand
from fysom import Fysom

#GLOBAL VARIABLES
MISSION = "Simple64"
VISUAL = True

class SimpleAgent(base_agent.BaseAgent):

    #select_request & move_camera
    camera_placed = 0
    select = 0
    tag = 0

    #workerTag
    scv = 0

    #fill_gaz
    gaz_filled = 0
    order = 0

    #Build order management
    current_id = 0
    building_id = [18,22,23,25,130,20,19,21,24,26,27,132,28,30,29,37]
    build_order = [21, 21, 21]
    gaz = False
    boStep = 0

    #FSM
    FSM = None

    #attack
    attack = False

    #########################################FUNCTIONS#############################################

    #GET INFORMATIONS FUNCTIONS

    #Units informations

    def get_unit(self, num, option, obs):

        for u in obs.observation.raw_units:
            if (u.alliance == features.PlayerRelative.SELF):
                if(option == "tag"):
                    if(u.tag == num):
                        return u
                else:
                    unit = [u for u in obs.observation.raw_units if u.unit_type == num]
                    if(len(unit)):
                        return rand.choice(unit)
        return []#get a unit (option : tag or type)

    def selected_unit(self, type, obs):

        for u in obs.observation.raw_units:
            if (u.is_selected):
                if(not type):
                    return u
                else:
                    if(u.unit_type == type):
                        return u
        return []#return selected unit or []

    def is_harvesting(self, unit, obs):#return true or false if scv is harvesting

        if(unit.order_id_0 == 359 or unit.order_id_0 == 362):
            return True
        return False

    def is_idle(self, tag, obs):

        for units in obs.observation.raw_units:
            if (units.tag == tag and not units.order_length):
                return True
        return False#return true if idle, false if not

    #Dependancies and actions id
    def unitDependancies(self, type, obs):

        dep = 0

        if(type == 50):
            dep = 26
        if(type == 52):
            dep = 29
        if(type == 57):
            dep = 30
        if (type == 22):
            dep = 18
        elif (type == 23 or type == 25 or type == 130):
            dep = 22
        elif (type == 21):
            dep = 19
        elif (type == 24 or type == 26 or  type == 27 or type == 132):
            dep = 21
        elif (type == 28 or type == 29):
            dep = 27
        elif (type == 30):
            dep = 28

        if not dep :
            return type

        else :
            current_dep = [building for building in obs.observation.raw_units if building.alliance == features.PlayerRelative.SELF and building.unit_type == dep]
            if current_dep :
                return type
            else :
                return self.unitDependancies(dep, obs)#return unit dependancies or himself if already fulfilled

    def get_required_building(self):

        if (self.current_id == 45 or self.current_id == 130 or self.current_id == 132):
            return 18
        elif (self.current_id == 48 or self.current_id == 49 or self.current_id == 211 or self.current_id == 37):
            return 21
        elif (self.current_id == 51 or self.current_id == 50):
            return 211
        elif (self.current_id == 53 or self.current_id == 498 or self.current_id == 271):
            return 27
        elif (self.current_id == 33 or self.current_id == 692 or self.current_id == 52):
            return 271
        elif (self.current_id == 35 or self.current_id == 54 or self.current_id == 689 or self.current_id == 281):
            return 28
        elif (self.current_id == 56 or self.current_id == 55 or self.current_id == 57):
            return 281#get the required building to build or to upgrade

    def getActionId(self):#get the action linked to the unit or the building wanted to create

        #TRAINING UNITS
        if (self.current_id == 45):
            return actions.FUNCTIONS.Train_SCV_quick
        elif (self.current_id == 48):
            return actions.FUNCTIONS.Train_Marine_quick
        elif (self.current_id == 49):
            return actions.FUNCTIONS.Train_Reaper_quick
        elif (self.current_id == 51):
            return actions.FUNCTIONS.Train_Marauder_quick
        elif (self.current_id == 50):
            return actions.FUNCTIONS.Train_Ghost_quick
        elif (self.current_id == 53):
            return actions.FUNCTIONS.Train_Hellion_quick
        elif (self.current_id == 33):
            return actions.FUNCTIONS.Train_SiegeTank_quick
        elif (self.current_id == 52):
            return actions.FUNCTIONS.Train_Thor_quick
        elif (self.current_id == 692):
            return actions.FUNCTIONS.Train_Cyclone_quick
        elif (self.current_id == 498):
            return actions.FUNCTIONS.Train_WidowMine_quick
        elif (self.current_id == 35):
            return actions.FUNCTIONS.Train_VikingFighter_quick
        elif (self.current_id == 54):
            return actions.FUNCTIONS.Train_Medivac_quick
        elif (self.current_id == 689):
            return actions.FUNCTIONS.Train_Liberator_quick
        elif (self.current_id == 56):
            return actions.FUNCTIONS.Train_Raven_quick
        elif (self.current_id == 55):
            return actions.FUNCTIONS.Train_Banshee_quick
        elif (self.current_id == 57):
            return actions.FUNCTIONS.Train_Battlecruiser_quick
        #BUILDING UNIT
        elif (self.current_id == 18):
            return actions.FUNCTIONS.Build_CommandCenter_screen
        elif (self.current_id == 22):
            return actions.FUNCTIONS.Build_EngineeringBay_screen
        elif (self.current_id == 23):
            return actions.FUNCTIONS.Build_MissileTurret_screen
        elif (self.current_id == 25):
            return actions.FUNCTIONS.Build_SensorTower_screen
        elif (self.current_id == 20):
            return actions.FUNCTIONS.Build_Refinery_screen
        elif (self.current_id == 19):
            return actions.FUNCTIONS.Build_SupplyDepot_screen
        elif (self.current_id == 21):
            return actions.FUNCTIONS.Build_Barracks_screen
        elif (self.current_id == 24):
            return actions.FUNCTIONS.Build_Bunker_screen
        elif (self.current_id == 26):
            return actions.FUNCTIONS.Build_GhostAcademy_screen
        elif (self.current_id == 27):
            return actions.FUNCTIONS.Build_Factory_screen
        elif (self.current_id == 28):
            return actions.FUNCTIONS.Build_Starport_screen
        elif (self.current_id == 29):
            return actions.FUNCTIONS.Build_Armory_screen
        elif (self.current_id == 30):
            return actions.FUNCTIONS.Build_FusionCore_screen
        #Upgrade buildings
        elif (self.current_id == 130):
            return actions.FUNCTIONS.Morph_PlanetaryFortress_quick
        elif (self.current_id == 132):
            return actions.FUNCTIONS.Morph_OrbitalCommand_quick

    def getBuildActionId(self):

        if (self.current_id == 19):
            return 222
        if (self.current_id == 18):
            return 187
        if (self.current_id == 20):
            return 214
        if (self.current_id == 22):
            return 191
        if (self.current_id == 23):
            return 202
        if (self.current_id == 25):
            return 216
        if (self.current_id == 21):
            return 185
        if (self.current_id == 24):
            return 186
        if (self.current_id == 26):
            return 196
        if (self.current_id == 27):
            return 194
        if (self.current_id == 28):
            return 221
        if (self.current_id == 30):
            return 195
        if (self.current_id == 29):
            return 183
        if (self.current_id == 37):
            return 225
        if (self.current_id == 39):
            return 227
        if (self.current_id == 41):
            return 229#get the id of a build action

    def camera_CC(self, obs):

        for u in obs.observation.raw_units:
            if(u.unit_type == 18 or u.unit_type == 130 or u.unit_type == 132):
                self.tag = u.tag
        raw = self.get_raw_coord(obs)
        if(not len(raw)):
            return actions.FUNCTIONS.no_op()
        self.tag = 0
        return self.move_camera(raw[0], raw[1])#move camera on command center

    def canDo(self, action, obs):
      return action in obs.observation.available_actions#can i do this action or not?

    def need_gaz(self, type):

        if(type == 130 or type == 25 or type == 26 or type == 27 or type == 28 or type == 30 or type == 29 or type == 49
           or type == 51 or type == 50 or type == 33 or type == 692 or type == 498 or type == 52 or type == 35 or type == 54
           or type == 689 or type == 56 or type == 55 or type == 57):
            return True
        else:
            return False#does this unit_type need gaz?

    #SELECT FUNCTIONS
    def select_unit(self, obs):

        for u in obs.observation.raw_units:
            if(self.tag == u.tag):
                if(u.unit_type == 45):
                    if(obs.observation.player.idle_worker_count > 0):
                        self.FSM.done()
                        self.camera_placed = 0
                        self.select = 0
                        return actions.FUNCTIONS.select_idle_worker("select")
                else:
                    break

        raw = self.get_raw_coord(obs)
        if(not len(raw)):
            return actions.FUNCTIONS.no_op()
        coord = self.get_coord(raw[0], raw[1])

        if(not self.camera_placed):
            return self.move_camera(coord[0], coord[1])

        elif(self.camera_placed == 1 and not self.select):
            return self.select_on_screen(coord[2], coord[3])

        else:
            for u in obs.observation.raw_units:
                if(u.is_selected and u.tag == self.tag):
                    self.tag = 0
                    self.FSM.done()
            self.camera_placed = 0
            self.select = 0
            return actions.FUNCTIONS.no_op()#do all the step of selection (move camera, select_on_screen...)

    def get_raw_coord(self, obs):

        for u in obs.observation.raw_units:
            if(u.tag == self.tag):
                return [u.x, u.y]
        return []#use tag global to get raw coordonates

    def get_coord(self, raw_x, raw_y):#[minimap_x, y, screen_x, y]

        if(16 <= raw_x and raw_x <= 47):
            minimap_x = 31
            screen_x = raw_x - 16
        elif(48 <= raw_x and raw_x <= 79):
            minimap_x = 63
            screen_x = raw_x - 47
        else:
            minimap_x = 85
            screen_x = raw_x - 69
        if(16 <= raw_y and raw_y <= 39):
            minimap_y = 31
            screen_y = raw_y - 15
        elif(40 <= raw_y and raw_y <= 63):
            minimap_y = 55
            screen_y = raw_y - 39
        elif(64 <= raw_y and raw_y <= 87):
            minimap_y = 80
            screen_y = raw_y - 64
        else:
            minimap_y = 103
            screen_y = raw_y - 88

        return [minimap_x, minimap_y, screen_x, screen_y]

    def move_camera(self, minimap_x, minimap_y):

        self.camera_placed = 1
        return actions.FUNCTIONS.move_camera([minimap_x, minimap_y])#move_camera to [x,y] position

    def select_on_screen(self, screen_x, screen_y):

        self.select = 1
        return actions.FUNCTIONS.select_point("select", (screen_x, screen_y))#select on screen the point [screen_x, screen_y]

    #HARVEST GAZ FUNCTIONS
    def harvest_gaz(self, obs):

        refinery = self.get_unit(20, "type", obs)
        if(len(refinery)):
            if(refinery.build_progress == 100):
                return self.fill_gaz(obs)
            return actions.FUNCTIONS.no_op()
        else:
            self.FSM.build_request()
            self.current_id = 20
            return actions.FUNCTIONS.no_op()#manage gaz harvest

    def fill_gaz(self, obs):

        refinery = self.not_full_refinery(obs)
        selected_unit = self.selected_unit(45,obs)

        if(not len(refinery)):
            self.gaz = True
            self.gaz_filled = 0
            self.FSM.done()
            self.tag = 0
            self.order = 0
            return actions.FUNCTIONS.no_op()

        else:
            if (len(selected_unit)):
                if(selected_unit.tag != self.scv and self.order == 1):
                    self.tag = refinery.tag
                    raw = self.get_raw_coord(obs)
                    if(not len(raw)):
                        return actions.FUNCTIONS.no_op()
                    return self.harvest(raw[0], raw[1], obs)

            harvesting_scv = self.get_harvesting_scv(obs)
            if (len(harvesting_scv)):
                self.tag = harvesting_scv.tag
                self.FSM.select_request()
                self.order = 1
                return actions.FUNCTIONS.no_op()

            return actions.FUNCTIONS.no_op()#fill not full gaz refinery

    def get_harvesting_scv(self, obs):

        for u in obs.observation.raw_units:
            if(u.unit_type == 45 and (u.order_id_0 == 359 or u.order_id_0 == 362)):
                return u
        return []#get harvesting scv

    def not_full_refinery(self, obs):

        for u in obs.observation.raw_units:
            if(u.unit_type == 20):
                if(u.assigned_harvesters < 3):
                    return u
        return []#return not full refinery or []

    def harvest(self, raw_x, raw_y, obs):

        coord = self.get_coord(raw_x, raw_y)

        if (not self.camera_placed):
            return self.move_camera(coord[0], coord[1])

        else :
            self.camera_placed = 0
            self.order = 0
            return actions.FUNCTIONS.Harvest_Gather_screen("now", [coord[2], coord[3]])#move unit to a raw position

    #BUILD UNITS
    def build_techlab(self, obs):

        selected_unit = self.selected_unit(0, obs)

        barrack_flying = [u for u in obs.observation.raw_units if u.unit_type == 46]
        if(len(barrack_flying)):
            self.FSM.done()
            return actions.FUNCTIONS.no_op()

        if(selected_unit.unit_type == 21):
            if(self.canDo(actions.FUNCTIONS.Build_TechLab_quick.id, obs)):
                self.FSM.done()
                return actions.FUNCTIONS.Build_TechLab_quick("now")
            elif(self.canDo(actions.FUNCTIONS.Build_TechLab_screen.id, obs)):
                coord = self.random_location(obs)
                return actions.FUNCTIONS.Build_TechLab_screen("now", (coord[0], coord[1]))

        else:
            barrack = [u for u in obs.observation.raw_units if u.unit_type == 21]
            if(len(barrack)):
                self.FSM.select_request()
                self.tag = barrack[0].tag
                return actions.FUNCTIONS.no_op()
            else:
                self.FSM.build_request()
                self.current_id = 21
                return actions.FUNCTIONS.no_op()

        return actions.FUNCTIONS.no_op()

    def research_shield(self, obs):

        selected_unit = self.selected_unit(0, obs)
        if(selected_unit.unit_type == 37):
            print("ZOUZOU")
            if(self.canDo(361, obs)):
                return actions.FUNCTIONS.Research_CombatShield_quick("now")
            else :
                return actions.FUNCTIONS.no_op()

        else:
            techLab = [u for u in obs.observation.raw_units if u.unit_type == 37]
            print("TECHLAB :", techLab)
            self.FSM.select_request()
            self.tag = techLab[0].tag
            return actions.FUNCTIONS.no_op()

    def build_unit(self, obs):#use to build according to all the dependancies

        if(self.current_id == 37):
            return self.build_techlab(obs)

        if(self.current_id == 130 or self.current_id == 132):
            self.FSM.morph_request()
            return actions.FUNCTIONS.no_op()

        action = self.getActionId()
        build_action_id = self.getBuildActionId()
        selected_unit = self.selected_unit(45, obs)

        if(len(selected_unit)):
            if(selected_unit.order_id_0 == build_action_id):
                self.FSM.done()
                if(self.build_order):
                    if(self.build_order[0] == self.current_id):
                        del self.build_order[0]
                if(self.build_order):
                    self.current_id = self.build_order[0]
                return actions.FUNCTIONS.no_op()

            if(self.canDo(action.id, obs)):
                if(self.is_idle(selected_unit.tag, obs) or self.is_harvesting(selected_unit, obs)):
                    if(not self.camera_placed):
                        return self.camera_CC(obs)
                    if(self.current_id == 20):
                        coord = self.find_vespine_screen(obs)
                        if(not len(coord)):
                            return actions.FUNCTIONS.no_op()
                    else:
                        coord = self.random_location(obs)
                    return action("now", (coord[0], coord[1]))
                return actions.FUNCTIONS.no_op()

            else:

                dep = self.unitDependancies(self.current_id, obs)

                if(dep != self.current_id):
                    self.current_id = dep
                    return actions.FUNCTIONS.no_op()

                elif(self.need_gaz(self.current_id)):
                    self.gaz_filled = 0
                    return actions.FUNCTIONS.no_op()

                else:
                    return actions.FUNCTIONS.no_op()

        else:
            if(self.scv):
                self.FSM.select_request()
                self.tag = self.scv
                return actions.FUNCTIONS.no_op()

            else:
                new_scv = self.get_unit(45, "type", obs)
                if(len(new_scv)):
                    self.scv = new_scv.tag
                    return actions.FUNCTIONS.no_op()

                else:
                    self.FSM.train_request()
                    self.current_id = 45
                    return actions.FUNCTIONS.no_op()

    def random_location(self, obs):

        x = rand.randint(4,28)
        y = rand.randint(4,28)

        for u in obs.observation.feature_units:
            if((u.alliance == features.PlayerRelative.NEUTRAL or u.alliance == features.PlayerRelative.SELF) and self.get_distance(u.x, u.y, x, y) < 4):
                return self.random_location(obs)

        return [x,y]#return random point on the screen

    def get_distance(self, x1, y1, x2, y2):
        return np.sqrt(((x1-x2)**2) + ((y1-y2)**2))#get distance between two points

    def find_vespine_screen(self, obs):#return vespine geyser [x, y] or [] if no vespine geyser

        for u in obs.observation.feature_units:
            if(u.unit_type == 342):
                return [u.x, u.y]
        return []

    #MORPH UNITS
    def morph_unit(self, obs):

        action = self.getActionId()
        required_building = self.get_required_building()
        selected_unit = self.selected_unit(required_building, obs)

        if(len(selected_unit)):

            print(obs.observation.available_actions)

            if(self.canDo(action.id, obs)):
                self.FSM.done()
                if(len(self.build_order)):
                    if(self.build_order[0] == self.current_id):
                        del self.build_order[0]
                if(self.build_order):
                    self.current_id = self.build_order[0]
                return action("now")

            elif(selected_unit.unit_type == required_building):

                dep = self.unitDependancies(self.current_id, obs)

                if(dep != self.current_id):
                    self.current_id = dep
                    self.FSM.build_request()
                    return actions.FUNCTIONS.no_op()

                elif(self.need_gaz(self.current_id)):
                    self.gaz_filled = 1
                    return actions.FUNCTIONS.no_op()

                else:
                    return actions.FUNCTIONS.no_op()

        else:
            morph_building = self.get_unit(required_building, "type", obs)
            if(len(morph_building)):
                self.FSM.select_request()
                self.tag = morph_building.tag
                return actions.FUNCTIONS.no_op()
            else:
                self.FSM.build_request()
                self.current_id = required_building
                return actions.FUNCTIONS.no_op()#use to morph unit according to all the dependancies(upgrade for example)

    #TRAIN UNITS
    def train_unit(self, obs):#use to train unit according to all the dependancies

        action = self.getActionId()
        required_building = self.get_required_building()
        if(self.current_id == 45):
            for u in obs.observation.raw_units:
                if(u.unit_type == 132):
                    required_building = 132

        selected_unit = self.selected_unit(required_building, obs)

        if(len(selected_unit)):

            if(self.canDo(action.id,obs)):

                if (self.selected_unit(0, obs).order_length >= 4):
                    return actions.FUNCTIONS.no_op()
                self.FSM.done()
                if(self.build_order):
                    if(self.current_id == self.build_order[0]):
                        del self.build_order[0]
                if(self.build_order):
                    self.current_id = self.build_order[0]
                return action("now")

            dep = self.unitDependancies(self.current_id, obs)

            if(dep != self.current_id):
                self.FSM.build_request()
                self.current_id = dep
                return actions.FUNCTIONS.no_op()

            elif(self.need_gaz(self.current_id)):
                self.gaz_filled = 1
                return actions.FUNCTIONS.no_op()

            else:
                return actions.FUNCTIONS.no_op()

        else:
            training_building = self.get_unit(required_building, "type", obs)
            if(len(training_building)):

                self.FSM.select_request()
                self.tag = training_building.tag
                return actions.FUNCTIONS.no_op()
            else:
                self.FSM.build_request()
                self.current_id = required_building
                return actions.FUNCTIONS.no_op()

    #Attack
    def attack_base(self, obs):

        if(not self.attack):
            self.attack = True
            if(self.canDo(actions.FUNCTIONS.select_army.id,obs)):
                return actions.FUNCTIONS.select_army("select")
            return actions.FUNCTIONS.no_op()

        else:
            for u in obs.observation.raw_units:
                if (u.alliance == features.PlayerRelative.ENEMY and (u.unit_type == 86 or u.unit_type == 101 or u.unit_type == 100)):
                    self.attack = False
                    if(self.canDo(actions.FUNCTIONS.Attack_minimap.id,obs)):
                        return actions.FUNCTIONS.Attack_minimap("now", (u.x, u.y))
                    return actions.FUNCTIONS.no_op()

    ########################################RESET AND GAME LOOP##############################################

    def reset(self):
        super(SimpleAgent, self).reset()
        self.FSM = Fysom({'initial': 'READY',
        'events': [
        {'name': 'done', 'src': '*', 'dst': 'READY'},
        {'name': 'train_request', 'src': '*', 'dst': 'TRAIN_UNIT'},
        {'name': 'build_request', 'src': '*', 'dst': 'BUILD_UNIT'},
        {'name': 'morph_request', 'src': '*', 'dst': 'MORPH_UNIT'},
        {'name': 'select_request', 'src': '*', 'dst': 'SELECT'},
        {'name': 'gaz_request', 'src': '*', 'dst': 'HARVEST_GAZ'}]})
        self.current_id = self.build_order[0]


  # game Loop
    def step(self, obs):
        super(SimpleAgent, self).step(obs)

        print("CURRENT ID : ", self.current_id)
        unit = [u for u in obs.observation.raw_units if u.alliance == features.PlayerRelative.SELF]
        for i in unit :
            print (i.unit_type)

        if(self.FSM.current == 'READY'):
            print("STATE : READY")

            if(not self.boStep):


                if(obs.observation.player.food_used < 17):
                    print("STEP 0 < 17")
                    CC = self.get_unit(18, "id", obs)
                    if(not CC.order_length):
                        self.FSM.train_request()
                        self.current_id = 45
                    elif(obs.observation.player.food_used == 14):
                        self.FSM.build_request()
                        self.current_id = 19
                    elif(obs.observation.player.food_used == 16 and not len(self.get_unit(21,"id",obs))):
                        self.FSM.build_request()
                        self.current_id = 21
                    elif(obs.observation.player.food_used == 16 and not len(self.get_unit(20,"id",obs))):
                        self.FSM.build_request()
                        self.current_id = 20

                elif(obs.observation.player.food_used < 24):
                    print("STEP 0 < 24")

                    if(len(self.not_full_refinery(obs))):
                        self.FSM.gaz_request()

                    else:
                        barracks = [u for u in obs.observation.raw_units if u.unit_type == 21]
                        barrack = rand.choice(barracks)
                        if(not barrack.order_length and obs.observation.player.food_cap - obs.observation.player.food_used > 2):
                            self.FSM.train_request()
                            self.current_id = 48
                        elif(obs.observation.player.food_used == 17):
                            self.FSM.build_request()
                            self.current_id = 19
                        elif(obs.observation.player.food_used == 19 and len(barracks) < 3):
                            self.FSM.build_request()
                            self.current_id = 21
                        elif(not len(self.get_unit(132, "id", obs)) and obs.observation.player.food_used >= 20):
                            self.FSM.build_request()
                            self.current_id = 132
                        CC = self.get_unit(18, "id", obs)
                        orbital = self.get_unit(132, "id", obs)
                        print("CC : ", CC)
                        if (len(orbital) or CC.order_id_0 == actions.FUNCTIONS.Morph_OrbitalCommand_quick.id):
                            self.boStep = 1

            elif(self.boStep == 1):
                print("STEP 1")
                techlab = self.get_unit(37, "id", obs)
                if(len(techlab)):
                    if(techlab.order_id_0 == 433):
                        self.boStep = 2
                    else:
                        return self.research_shield(obs)
                else:
                    self.FSM.build_request()
                    self.current_id = 37

            elif(self.boStep == 2):
                barracks = [u for u in obs.observation.raw_units if u.unit_type == 21]
                if(len(barracks) < 3):
                    self.FSM.build_request()
                    self.current_id = 21
                else:
                    self.boStep = 3

            else:
                barracks = [u for u in obs.observation.raw_units if u.unit_type == 21]
                barrack = rand.choice(barracks)
                if(obs.observation.player.food_cap - obs.observation.player.food_used < 10):
                    self.FSM.build_request()
                    self.current_id = 19
                elif(not barrack.order_length):
                    self.FSM.train_request()
                    self.current_id = 48
                elif(len(barracks)<4):
                    self.FSM.build_request()
                    self.current_id = 21
                else:
                    return self.attack_base(obs)



        elif(self.FSM.current == 'BUILD_UNIT'):
            print("STATE : BUILD_UNIT")
            return self.build_unit(obs)

        elif(self.FSM.current == 'TRAIN_UNIT'):
            print("STATE : TRAIN_UNIT")
            return self.train_unit(obs)

        elif(self.FSM.current == 'MORPH_UNIT'):
            print("STATE : MORPH_UNIT")
            return self.morph_unit(obs)

        elif(self.FSM.current == 'HARVEST_GAZ'):
            print("STATE : FILL_GAZ")
            return self.harvest_gaz(obs)

        elif(self.FSM.current == 'SELECT'):
            print("STATE : SELECT_UNIT")
            return self.select_unit(obs)

        return actions.FUNCTIONS.no_op()


class TestBuildAll(utils.TestCase):
  step_mul = 10
  game_steps_per_episode = 10000000
  nb_episodes = 1

  def setUp(self):
    super(TestBuildAll, self).setUp()
    sc2_env.stopwatch.sw.disable()

  def test(self):
    AGENT_INTERFACE_FORMAT = sc2_env.AgentInterfaceFormat(
      feature_dimensions=sc2_env.Dimensions(screen=32, minimap=128), use_feature_units=True, use_raw_units=True, raw_resolution=128)
    with sc2_env.SC2Env(
      map_name=MISSION, players=[sc2_env.Agent(sc2_env.Race.terran), sc2_env.Agent(sc2_env.Race.zerg)],
      step_mul=self.step_mul, visualize=VISUAL,
      agent_interface_format=AGENT_INTERFACE_FORMAT,
        disable_fog=1) as env:
      agent = SimpleAgent()
      agent2 = emp.SimpleAgent()
      run_loop.run_loop([agent, agent2], env, self.game_steps_per_episode,
        self.nb_episodes)

if(__name__ == "__main__"):
  absltest.main()
