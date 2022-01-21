/*Made by : Debbah Rabah / Rouichi Adil
*/
//BACKGROUNDS
let imageGrass = new Image();
imageGrass.src = "./assets/scenery/grass.png"
let imageDry= new Image();
imageDry.src = "./assets/scenery/drygrass.png"
let imageSand = new Image();
imageSand.src = "./assets/scenery/bigsand.png"
let imageGround = new Image();
imageGround.src = "./assets/scenery/ground.png"
//SCENERY
let imageBush1 = new Image();
imageBush1.src = "./assets/scenery/Bush_01.png"
let imageBush2 = new Image();
imageBush1.src = "./assets/scenery/Bush_02.png"
let imageTree1 = new Image();
imageTree1.src = "./assets/scenery/Tree_01.png"
let imageTree2 = new Image();
imageTree2.src = "./assets/scenery/Tree_02.png"
let imageStart = new Image();
imageStart.src = "./assets/scenery/Start.png"
let imageRoofConcrete = new Image();
imageRoofConcrete.src = "./assets/scenery/roof_concrete.png"
let imageRoofDust = new Image();
imageRoofDust.src = "./assets/scenery/roof_dust.png"
//FONTS
let fontAdil = new Image();
fontAdil.src = "./assets/fonts/adil.png"
let fontRabah = new Image();
fontRabah.src = "./assets/fonts/rabah.png"
let fontStart = new Image();
fontStart.src = "./assets/fonts/start.png"
let fontTitle = new Image();
fontTitle.src = "./assets/fonts/title.png"
let fontPause = new Image();
fontPause.src = "./assets/fonts/pause.png"
let fontGenerate = new Image();
fontGenerate.src = "./assets/fonts/generate.png"
//ROADS
let imageRoadHidden = new Image();
imageRoadHidden.src = "./assets/track/road_hidden.png";
let imageRoadConcrete = new Image();
imageRoadConcrete.src = "./assets/track/road_concrete.png";
let imageRoadDust = new Image();
imageRoadDust.src = "./assets/track/road_dust.png";

////////////////////////////////////////////////////CLASS CAR///////////////////////////////////////////////////
class Car{

  constructor(x, y, angle, type){


    this.x = x;
    this.y = y;
    this.angle = angle;
    this.steeringAngle;
    this.speed = 0;
    this.startSpeed;
    this.maxSpeed;
    this.acceleration;
    this.brakeSpeed;
    this.collision = false;

    this.imageCar = new Image();
    switch (type){
      case 0 :
        this.imageCar.src = "./assets/cars/car.png";//red car
        this.acceleration = 1.13;
        this.maxSpeed = 4.45;
        this.brakeSpeed = 1.07;
        this.startSpeed = 0.43;
        this.steeringAngle = Math.PI / 29;
        break;
      case 1 :
        this.imageCar.src = "./assets/cars/car1.png";//green car
        this.acceleration = 1.12;
        this.maxSpeed = 4.7;
        this.brakeSpeed = 1.09;
        this.startSpeed = 0.45;
        this.steeringAngle = Math.PI / 31;
        break;
      case 2 :
        this.imageCar.src = "./assets/cars/car2.png";//yellow car
        this.acceleration = 1.12;
        this.maxSpeed = 5.3;
        this.brakeSpeed = 1.085;
        this.startSpeed = 0.5;
        this.steeringAngle = Math.PI / 31.5;

        break;
      case 3 :
        this.imageCar.src = "./assets/cars/car3.png";//black car
        this.acceleration = 1.15;
        this.maxSpeed = 4.6;
        this.brakeSpeed = 1.09;
        this.startSpeed = 0.5;
        this.steeringAngle = Math.PI / 31;
        break;
    }

  }

  behavior(){

    var left = this.visibility(game.ctx, -(Math.PI/4), this.x, this.y);
    var right = this.visibility(game.ctx, Math.PI/4, this.x, this.y);
    var front = this.visibility(game.ctx, 0, this.x, this.y);

    if((right-left)*this.steeringAngle > 2) this.steering(true);
    else if ((left-right)*this.steeringAngle > 2) this.steering(false);

    if((this.speed*15)*this.acceleration < front/2) this.accelerate();
    else if((this.speed*8)*this.brakeSpeed > front) this.deccelerate();

  }

  visibility(ctx, angle, x, y){

    var fx = x;
    var fy = y;
    var acc = 0;

    while(this.onTheRoad(fx,fy)){

        acc++;
        fx = fx + Math.cos(-this.angle - angle);
        fy = fy + Math.sin(-this.angle - angle);
    }
    return acc;

  }

  apply_friction(){
    this.speed *= 0.98;
  }

  accelerate(){
    if (this.speed > 0) {
      this.speed *= this.acceleration; // Accelerate
    } else if (this.speed < 0) {
      this.speed = this.startSpeed;
    } else {
      this.speed = this.startSpeed;
    }
  }

  deccelerate(){

		if (this.speed < 0) {
			this.speed *= this.acceleration;
		} else if (this.speed > 0) {
			this.speed *= 1 - (this.brakeSpeed - 1) * 5;
		} else {
			this.speed = -1 * this.startSpeed;
		}
  }

  speed_limit(){
		if (this.speed > 0) {
			if (this.speed > this.maxSpeed) this.speed = this.maxSpeed;
		} else if (this.speed < 0) {
			if (this.speed < -1 / 2 * this.maxSpeed) this.speed = -1 / 2 * this.maxSpeed;
		}
  }

  steering(left){

	  if (this.speed > 0) {
			if (left) this.angle += this.steeringAngle;
      else if (!left) this.angle -= this.steeringAngle;
		}
		if (this.angle > 2 * Math.PI) this.angle %= 2 * Math.PI;
  }

  detect_collision(ctx){

    var front = [this.x + Math.cos(-this.angle) * 6, this.y + Math.sin(-this.angle) * 3];
    var back = [this.x - Math.cos(-this.angle) * 6, this.y - Math.sin(-this.angle) * 3];
    var right = [this.x - Math.cos(-this.angle-Math.PI/2) * 6, this.y - Math.sin(-this.angle-Math.PI/2) * 3];
    var left = [this.x - Math.cos(-this.angle+Math.PI/2) * 6, this.y - Math.sin(-this.angle+Math.PI/2) * 3];
    var cfr = [(front[0] - this.x) + right[0], (front[1] - this.y) + right[1]];
    var cfl = [(front[0] - this.x) + left[0], (front[1] - this.y) + left[1]];
    var cbr = [(back[0] - this.x) + right[0], (back[1] - this.y) + right[1]];
    var cbl = [(back[0] - this.x) + left[0], (back[1] - this.y) + left[1]];

    if(!this.onTheRoad(cfr[0], cfr[1])){
      this.collision = true;
      return;
    }
    if(!this.onTheRoad(cfl[0], cfl[1])){
      this.collision = true;
      return;
    }
    if(!this.onTheRoad(cbr[0], cbr[1])){
      this.collision = true;
      return;
    }
    if(!this.onTheRoad(cbl[0], cbl[1])){
      this.collision = true;
      return;
    }

    this.collision = false;

  }

  onTheRoad (x, y) {
    if(x<0 || y<0 || x>1024 || y>768)
      return false;
  	var i = Math.round(y) * 1024 + Math.round(x);
  	return game.track.dataArray[i];
  }

  update_pos(){

    var xTmp = this.x;
    var yTmp = this.y;
    this.x += this.speed * Math.cos(this.angle);
  	this.y -= this.speed * Math.sin(this.angle);

    this.detect_collision();
    if(this.collision){
      this.x = xTmp;
      this.y = yTmp;
      this.speed = -1 * this.speed / 1.5;
    }
  }

  draw(ctx){

    ctx.save();
    ctx.translate(this.x, this.y);
    ctx.rotate(-this.angle);
    ctx.translate(-8,-4);
    ctx.drawImage(this.imageCar, 0, 0, 16, 8);
    ctx.restore();
  }
}
////////////////////////////////////////////////////CLASS TRACK/////////////////////////////////////////////////

class Track{ // Track class object

  constructor(w, h, ratio, tileSize){

    this.height = h;
    this.width = w;
    this.trackRatio = ratio;
    this.mapArray = new Array(h*w);
    this.dataArray = [];
    this.start;
    this.tileSize = tileSize;
    this.roadType = Math.floor(Math.random()*2+1);

  }

  pushInto_dataArray(){

    this.white_canva(game.ctx);
    this.draw(game.ctx, 0);
		var k = game.ctx.getImageData(0, 0, 1024, 768).data;
    var j;
		for (var i = 0; i < k.length / 4; i++) {
			j = 4 * i;
			if (k[j] === 0 && k[j + 1] === 0 && k[j + 2] === 0) {
				this.dataArray.push(true);
			} else {
				this.dataArray.push(false);
			}
		}
  }

  init_mapArray(){
    this.zero_mapArray();
    this.create_road();
    this.draw_road();
    this.pushInto_dataArray();
    this.choose_start();
  }

  choose_start(){
    for (var i = 0; i < this.mapArray.length; i++){
      if (this.mapArray[i] == 1){
        this.start[0] = i%this.width;
        this.start[1] = Math.floor(i/this.width);
        return;
      }
    }
  }

  zero_mapArray(){
    for (var i = 0; i < this.mapArray.length; i++){
      this.mapArray[i] = 0;
    }
  }

  create_road(){

    var x = Math.floor(Math.random() * this.width);
    var y = Math.floor(Math.random() * this.height);
    this.put_array_value(x,y,1);
    this.start = [x,y];
    this.next_road(x,y,1);

    var roadNbr = 0;
    for (var i = 0 ; i < this.mapArray.length; i++){
      if(this.mapArray[i] > 0) roadNbr++;
    }
    if(roadNbr < (this.height*this.width)/this.trackRatio) {
      this.zero_mapArray();
      this.create_road();
    }
    else{
    }
  }

  draw_road(){

    var val1, val2, valCurrent;
    var valMax = this.get_array_max_value();
    var tmpArray = new Array(this.height*this.width);

    for(var y=0; y<this.height; y++){
      for(var x=0; x<this.width; x++){
        valCurrent = this.get_array_value(x,y);

        if(valCurrent < 1) tmpArray[x + (y*this.width)] = -1;

        else{
          if(valCurrent == valMax){
            val1 = 1;
            val2 = valCurrent-1;
          }
          else if(valCurrent == 1){
            val1 = 2;
            val2 = valMax;
          }
          else{
            val1 = valCurrent-1;
            val2 = valCurrent+1;
          }

          if(y-1 >= 0 && y+1 <= this.height-1){ // straight vertical
            if(
              (val1 == this.get_array_value(x,y-1) && val2 == this.get_array_value(x,y+1)) ||
              (val2 == this.get_array_value(x,y-1) && val1 == this.get_array_value(x,y+1))){
                tmpArray[x + (y*this.width)] = 0;
                continue;
              }
          }
          if(x-1 >= 0 && x+1 <= this.width-1){ //straight horizontal
            if(
              (val1 == this.get_array_value(x-1,y) && val2 == this.get_array_value(x+1,y)) ||
              (val2 == this.get_array_value(x-1,y) && val1 == this.get_array_value(x+1,y))){
                tmpArray[x + (y*this.width)] = 1;
                continue;
              }
          }
          if(y+1 <= this.height-1 && x+1 <= this.width-1){ // corner top left
            if(
              (val1 == this.get_array_value(x+1,y) && val2 == this.get_array_value(x,y+1)) ||
              (val2 == this.get_array_value(x+1,y) && val1 == this.get_array_value(x,y+1))){
                tmpArray[x + (y*this.width)] = 2;
                continue;
              }
          }
          if(x-1 >= 0 && y+1 <= this.height-1){ // corner top right
            if(
              (val1 == this.get_array_value(x-1,y) && val2 == this.get_array_value(x,y+1)) ||
              (val2 == this.get_array_value(x-1,y) && val1 == this.get_array_value(x,y+1))){
                tmpArray[x + (y*this.width)] = 3;
                continue;
              }
          }
          if(x+1 <= this.width-1 && y-1 >= 0){ // corner bottom left
            if(
              (val1 == this.get_array_value(x+1,y) && val2 == this.get_array_value(x,y-1)) ||
              (val2 == this.get_array_value(x+1,y) && val1 == this.get_array_value(x,y-1))){
                tmpArray[x + (y*this.width)] = 4;
                continue;
              }
          }
          if(x-1 >= 0 && y-1 >= 0){ // corner bottom right
            if(
              (val1 == this.get_array_value(x-1,y) && val2 == this.get_array_value(x,y-1)) ||
              (val2 == this.get_array_value(x-1,y) && val1 == this.get_array_value(x,y-1))){
                tmpArray[x + (y*this.width)] = 5;
                continue;
              }
          }
        }
      }
    }
    this.mapArray = tmpArray;
  }

  next_road(x, y, acc){

    if(x != this.start[0] || y != this.start[1])
      this.put_array_value(x,y,acc);

    for (var i = 0; i < this.mapArray.length; i++) {
      if(this.mapArray[i] < 1) this.mapArray[i] = -2;
    }
    this.give_score_to_case(this.start[0], this.start[1]);

    var direction = [];
    if(y-1 >= 0 && this.get_array_value(x, y-1) == -1){
      direction.push(0);
    }
    if(x-1 >= 0 && this.get_array_value(x-1, y) == -1){
      direction.push(1);
    }
    if(y+1 <= this.height-1 && this.get_array_value(x, y+1) == -1){
      direction.push(2);
    }
    if(x+1 <= this.width-1 && this.get_array_value(x+1, y) == -1){
      direction.push(3);
    }
    if(!direction.length) return;

    var ranDir = Math.floor(Math.random() * direction.length);

    if(!direction[ranDir]){
      this.next_road(x,y-1,++acc);
    }
    else if(direction[ranDir] == 1){
      this.next_road(x-1,y,++acc);
    }
    else if(direction[ranDir] == 2){
      this.next_road(x,y+1,++acc);
    }
    else{
      this.next_road(x+1, y,++acc);
    }
  }

  get_array_max_value(){
    var valMax = 0;
    for(var i = 0; i < this.mapArray.length; i++){
      if(this.mapArray[i] > valMax) valMax = this.mapArray[i];
    }
    return valMax;
  }

  get_array_value(x, y){
    return this.mapArray[x + (y*this.width)];
  }

  put_array_value(x, y, value){
    this.mapArray[x + (y*this.width)] = value;
  }

  give_score_to_case(x, y){

    if(y-1 >= 0 && (this.get_array_value(x,y-1) <= 0 && this.get_array_value(x,y-1) != -1)){
      this.put_array_value(x,y-1,-1);
      this.give_score_to_case(x, y-1);
    }
    if(x-1 >= 0 && (this.get_array_value(x-1,y) <= 0 && this.get_array_value(x-1,y) != -1)){
      this.put_array_value(x-1,y,-1);
      this.give_score_to_case(x-1, y);
    }
    if(y+1 <= this.height-1 && (this.get_array_value(x,y+1) <= 0 && this.get_array_value(x,y+1) != -1)){
      this.put_array_value(x,y+1,-1);
      this.give_score_to_case(x, y+1);
    }
    if(x+1 <= this.width-1 && (this.get_array_value(x+1,y) <= 0 && this.get_array_value(x+1,y) != -1)){
      this.put_array_value(x+1,y,-1);
      this.give_score_to_case(x+1, y);
    }
  }

  white_canva(ctx){

  ctx.fillStyle = "#FFFFFF";
  ctx.fillRect(0, 0, 1024, 768);
  }

  draw(ctx, type){


    var sx, sy;
    var sourceRoad = {"straightVertical" : [256,256] , "straightHorizontal" : [256,0], "cornerTopLeft" : [128,128] ,
                  "cornerTopRight" : [256,128], "cornerBottomLeft" : [128,256], "cornerBottomRight" : [0,256]};

    for (var i = 0; i < this.mapArray.length; i++){
      switch (this.mapArray[i]){
        case 0:
          sx = sourceRoad["straightVertical"][0];
          sy = sourceRoad["straightVertical"][1];
          break;
        case 1:
          sx = sourceRoad["straightHorizontal"][0];
          sy = sourceRoad["straightHorizontal"][1];
          break;
        case 2:
          sx = sourceRoad["cornerTopLeft"][0];
          sy = sourceRoad["cornerTopLeft"][1];
          break;
        case 3:
          sx = sourceRoad["cornerTopRight"][0];
          sy = sourceRoad["cornerTopRight"][1];
          break;
        case 4:
          sx = sourceRoad["cornerBottomLeft"][0];
          sy = sourceRoad["cornerBottomLeft"][1];
          break;
        case 5:
          sx = sourceRoad["cornerBottomRight"][0];
          sy = sourceRoad["cornerBottomRight"][1];
          break;
        default:
          continue;
      }
      if(!type)
        ctx.drawImage(imageRoadHidden, sx, sy, 128, 128, (i%this.width)*this.tileSize, Math.floor(i/this.width)*this.tileSize, this.tileSize, this.tileSize);
      else{
        if(this.roadType == 1)
          ctx.drawImage(imageRoadConcrete, sx, sy, 128, 128, (i%this.width)*this.tileSize, Math.floor(i/this.width)*this.tileSize, this.tileSize, this.tileSize);
        else
          ctx.drawImage(imageRoadDust, sx, sy, 128, 128, (i%this.width)*this.tileSize, Math.floor(i/this.width)*this.tileSize, this.tileSize, this.tileSize);
      }
    }
  }

  /*draw_data(ctx){
    for (var i = 0 ; i < this.dataArray.length; i++){
      if(this.dataArray[i] == true){
        ctx.fillStyle = "green";
      }
      else {
        ctx.fillStyle = "red";
      }
      ctx.fillRect(i%1024, Math.floor(i/1024), 1, 1);
    }
  }
*/
}

////////////////////////////////////////////////////CLASS GAME//////////////////////////////////////////////////

class Game {

  constructor(tileX, tileY, tileSize) {
    // Game context handler
    this.cnv = document.getElementById('myCanvas');
    this.ctx = this.cnv.getContext('2d');
    this.ctx.imageSmoothingEnabled= false;
    // Game running handler
    this.isPaused = true;
    this.isInGame = false;
    // Track
    this.track;
    // Cars
    this.cars;
    // Tiles
    this.tileSize = tileSize;
    this.width = tileX;
    this.height = tileY;
    // Background
    this.bgWidth = tileX*4;
    this.bgHeight = tileY*4;
    this.bgTileSize = Math.round(tileSize/4);
    this.bgArray = new Array(this.bgWidth*this.bgHeight);
    // Scenery
    this.scWidth = tileX*2;
    this.scHeight = tileY*2
    this.scTileSize = Math.round(tileSize/2);
    this.scArray = new Array(this.scWidth*this.scHeight);
    //Music
    this.music = new Audio('./assets/sounds/theme.mp3');

   }

  make_track(){ // make the track

    let newTrack = new Track(this.width, this.height, 3, this.tileSize);
    newTrack.init_mapArray();
    this.track = newTrack;
  }

  new_car(x, y, type){

    let newCar = new Car(x, y, 0, type);
    this.cars.push(newCar);
  }


  ////////////////BACKGROUND
  init_background(){
    this.bg_zero_mapArray();
    this.make_background();

  }

  bg_zero_mapArray(){
    for (var i = 0; i < this.bgArray.length; i++){
      this.bgArray[i] = 0;
    }
  }

  make_background(){
    var tileLeft = this.bgWidth*this.bgHeight;
    var x, y, acc, randNumb, ranDir, randStart, tileType = 1;

    while(tileLeft){


      acc = 0;
      for (var i = 0; i < this.bgArray.length; i++){
        if(!this.bgArray[i]) acc++;
      }
      randStart = Math.floor(Math.random() * acc + 1);
      for(i = 0; i < this.bgArray.length; i++){
        if(!randStart){
          x = i%this.bgWidth;
          y = Math.floor(i/this.bgWidth);
          break;
        }
        else if(!this.bgArray[i]) randStart--;
      }

      if(tileType == 1) tileType = 2;
      else if(tileType == 2) tileType = 3;
      else if(tileType == 3) tileType = 4;
      else if(tileType == 4) tileType = 1;

      randNumb = Math.floor(Math.random() * (tileLeft/2) + 1);

      while(randNumb){

        var direction = [];
        if(y-1 >= 0 && !this.bg_get_array_value(x, y-1)){
          direction.push(0);
        }
        if(x-1 >= 0 && !this.bg_get_array_value(x-1, y)){
          direction.push(1);
        }
        if(y+1 <= this.bgHeight-1 && !this.bg_get_array_value(x, y+1)){
          direction.push(2);
        }
        if(x+1 <= this.bgWidth-1 && !this.bg_get_array_value(x+1, y)){
          direction.push(3);
        }
        if(!direction.length) break;


        ranDir = Math.floor(Math.random() * direction.length);

        if(!direction[ranDir]){
          this.bg_put_array_value(x,y,tileType);
          randNumb--;
          tileLeft--;
          y = y-1;
        }
        else if(direction[ranDir] == 1){
          this.bg_put_array_value(x,y,tileType);
          randNumb--;
          tileLeft--;
          x = x-1;
        }
        else if(direction[ranDir] == 2){
          this.bg_put_array_value(x,y,tileType);
          randNumb--;
          tileLeft--;
          y = y+1;
        }
        else{
          this.bg_put_array_value(x,y,tileType);
          randNumb--;
          tileLeft--;
          x = x+1;

        }
      }
    }
  }

  bg_get_array_value(x, y){
    return this.bgArray[x + (y*this.bgWidth)];
  }

  bg_put_array_value(x, y, value){
    this.bgArray[x + (y*this.bgWidth)] = value;
  }

  draw_background(){

    for (var i = 0 ; i < this.bgArray.length; i++){

      if(this.bgArray[i] == 1){
        this.ctx.drawImage(imageGrass, (i%this.bgWidth)*this.bgTileSize, Math.floor(i/this.bgWidth)*this.bgTileSize, this.bgTileSize, this.bgTileSize);
      }
      else if(this.bgArray[i] == 2){
        this.ctx.drawImage(imageDry, (i%this.bgWidth)*this.bgTileSize, Math.floor(i/this.bgWidth)*this.bgTileSize, this.bgTileSize, this.bgTileSize);
      }
      else if(this.bgArray[i] == 3){
        this.ctx.drawImage(imageSand, (i%this.bgWidth)*this.bgTileSize, Math.floor(i/this.bgWidth)*this.bgTileSize, this.bgTileSize, this.bgTileSize);
      }
      else
        this.ctx.drawImage(imageGround, (i%this.bgWidth)*this.bgTileSize, Math.floor(i/this.bgWidth)*this.bgTileSize, this.bgTileSize, this.bgTileSize);
    }
  }

  ////////////////SCENERY
  init_scenery(){
    this.sc_zero_mapArray();
    this.make_scenery();
    this.make_tunnels();
  }

  sc_zero_mapArray(){
    for (var i = 0; i < this.scArray.length; i++){
      this.scArray[i] = 0;
    }
  }

  make_scenery(){

    var acc, tileCount, tileLeftCp, x, y, randTile, tileChoice;
    var tileLeft = this.scArray.length;

    for(var j = 0; j < this.scHeight ; j++){
      for(var i = 0; i < this.scWidth; i++){
        if(this.track.get_array_value(Math.floor(i/2), Math.floor(j/2)) >= 0){
          tileLeft--;
          this.sc_put_array_value(i,j,-1);
        }
      }
    }

    tileCount = Math.floor(Math.round(tileLeft/1.5));

    while(tileCount){

      randTile = Math.floor(Math.random() * tileLeft + 1);

      for(i = 0; i < this.scArray.length; i++){
        if(!randTile){
          x = i%this.scWidth;
          y = Math.floor(i/this.scWidth);
          break;
        }
      else if(this.scArray[i] == 0) randTile--;
      }

      tileChoice = Math.floor(Math.random() * 4 + 1);

      this.sc_put_array_value(x,y,tileChoice);
      tileLeft--;
      tileCount--;
    }
    this.sc_put_array_value(this.track.start[0]*2, this.track.start[1]*2, 5);
  }

  make_tunnels(){

    var j,i, x,y,randT,tNumbers;
    var straightTile = 0;

    for (var i = 0 ; i < this.track.mapArray.length; i++){
      if((i%this.track.width != this.track.start[0] && Math.floor(i/this.track.width) != this.track.start[1])
       && (this.track.mapArray[i] == 0 || this.track.mapArray[i] == 1))
        straightTile++;
    }

    tNumbers = Math.floor(straightTile/2);

    while(tNumbers){

      randT = Math.floor(Math.random() * straightTile);

      for(j = 0; j < this.track.height && randT; j++){
        for (i = 0; i < this.track.width && randT; i++){
          if((i != this.track.start[0] && j != this.track.start[1]) &&
            (this.track.get_array_value(i,j) == 0 || this.track.get_array_value(i,j) == 1)) randT--;
        }
      }
      this.sc_put_array_value(i*2-2,j*2-2,6);
      tNumbers--;
      straightTile--;
    }
  }

  sc_put_array_value(x, y, value){
    this.scArray[x + (y*this.scWidth)] = value;
  }

  sc_get_array_value(x, y){
    return this.scArray[x + (y*this.scWidth)];
  }

  draw_scenery(){

    for (var i = 0 ; i < this.scArray.length; i++){

      if(this.scArray[i] == 1){
        this.ctx.drawImage(imageBush1, ((i-1)%this.scWidth)*this.scTileSize, Math.floor(i/this.scWidth)*this.scTileSize, this.scTileSize, this.scTileSize);
      }
      else if(this.scArray[i] == 2){
        this.ctx.drawImage(imageBush2, ((i-1)%this.scWidth)*this.scTileSize, Math.floor(i/this.scWidth)*this.scTileSize, this.scTileSize, this.scTileSize);
      }
      if(this.scArray[i] == 3){
        this.ctx.drawImage(imageTree1, ((i-1)%this.scWidth)*this.scTileSize, Math.floor(i/this.scWidth)*this.scTileSize, this.scTileSize, this.scTileSize);
      }
      else if(this.scArray[i] == 4){
        this.ctx.drawImage(imageTree2, ((i-1)%this.scWidth)*this.scTileSize, Math.floor(i/this.scWidth)*this.scTileSize, this.scTileSize, this.scTileSize);
      }
      else if(this.scArray[i] == 5){
        this.ctx.drawImage(imageStart, ((i+1)%this.scWidth)*this.scTileSize+this.scTileSize/2, Math.floor(i/this.scWidth)*this.scTileSize+this.scTileSize/4, this.scTileSize-this.scTileSize/2, this.scTileSize*1.5);
      }
    }

  }

  draw_tunnels(){

    for (var i = 0 ; i < this.scArray.length; i++){

      if(this.scArray[i] == 6){
        if(this.track.roadType == 1)
          this.ctx.drawImage(imageRoofConcrete, (i%this.scWidth)*this.scTileSize, Math.floor(i/this.scWidth)*this.scTileSize, this.scTileSize*2, this.scTileSize*2);
        else
          this.ctx.drawImage(imageRoofDust, (i%this.scWidth)*this.scTileSize, Math.floor(i/this.scWidth)*this.scTileSize, this.scTileSize*2, this.scTileSize*2);
      }
    }
  }

}

////////////////////////////////////////////Utilities functions///////////////////////////////////////////////

function game_loop(){ // Game loop

  game.ctx.clearRect(0, 0, game.cnv.width, game.cnv.height);
  game.draw_background();
  game.track.draw(game.ctx, 1);
  game.draw_scenery();
  //game.track.draw_data(game.ctx);
  for(var i = 0; i < game.cars.length; i++){
    if(!game.isPaused){
      game.music.play();
      game.cars[i].behavior();
      game.cars[i].speed_limit();
      game.cars[i].update_pos();
    }
    game.cars[i].draw(game.ctx);
  }
  game.draw_tunnels();
  if(game.isPaused)
    draw_UI();

}

function game_init(){ // Game initialisation

  game.cars = new Array();
  game.make_track();
  game.init_background();
  game.init_scenery();
  game.new_car(game.track.start[0]*60+12, game.track.start[1]*60+22, 0);
  game.new_car(game.track.start[0]*60+12, game.track.start[1]*60+36, 1);
  game.new_car(game.track.start[0]*60+32, game.track.start[1]*60+22, 2);
  game.new_car(game.track.start[0]*60+32, game.track.start[1]*60+36, 3);
}

function draw_UI(){
  game.ctx.beginPath();
  game.ctx.globalAlpha = 0.6;
  game.ctx.fillStyle = "#000011"
  game.ctx.fillRect(0,0,1024,768);
  game.ctx.closePath();
  game.ctx.beginPath();
  game.ctx.fillRect(50, 400, 400,200);
  game.ctx.fillRect(560, 400, 400,200);
  game.ctx.closePath();
  game.ctx.globalAlpha = 1;
  game.ctx.drawImage(fontTitle, 220,50,800,500);
  game.ctx.drawImage(fontAdil, 10,10,200,125);
  game.ctx.drawImage(fontRabah, 825, 10, 200,125);
  game.ctx.drawImage(fontGenerate, 50,400,510,350);
  game.ctx.drawImage(fontStart, 590,410,700,500);

}

function clickButton(event) {

    var x = event.clientX;
    var y = event.clientY;

    if(x >= 50 && x <= 450 && y >= 400 && y <= 600)
      game_init();
    else if(x >= 560 && x <= 960 && y >= 400 && y <= 600)
      game.isPaused = false;
}

///////////////////////////////////////////Game execution/////////////////////////////////////////////////////
let game = new Game(17, 13, 60);
game_init();
document.addEventListener("click", clickButton);
setInterval(game_loop, 20);
