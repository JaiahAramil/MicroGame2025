/*Project Credits - 
This project was inspired by a video by Volos Projects on YouTube "Arduino Star Wars Game - how to make"- https://www.youtube.com/watch?app=desktop&v=lOz_GuME63E
The code is built upon the linked sketch here - https://drive.google.com/file/d/1tQQKNb3TkO4WgnDnp7REAYJviCnHli0Y/view
Code translation, organization, pixel art, and animations by Jason A. Naquin.
*/

#include <Arduino.h>
#include <MicroGame.h>
#include <SpaceChase.h>

MicroGame mg;
SpaceChase spaceChase; 

/*Base values that change as the game runs*/
int preGame = 2;
int gameOver = 0;

int playerShotX = 0;
int playerShotY = 0;
int playerShot = 0;
int playerShotCooldown = 0;

int enemyY = 8;
int direction = 0;
int rx = 95;
int ry = 0;
int rx2 = 95;
int ry2 = 0;
int rx3 = 95;
int ry3 = 0;
int points = 0;
int bombCoolDown = 0;
int enemyBulletSpeed = 3;
int bossSpeed = 1;

int enemyHeightMod = 10;
int enemyWidthMod = 10;

int rx4 = 95;
int ry4 = 0;
int lives = 5;
int poc = 0;
int fired = 0;
int levelCount = 1;
int enemyX = 95;
unsigned long initially = 0;
unsigned long selected = 0;
unsigned long currentTime = 0;

unsigned long sessionStart = 0;
unsigned long sessionTime = 0;
String pointString;
String pointStringMod = "";

/*Starting location of the player ship - plrX and plrY are meant to dynamically change throughout gameplay*/
int defaultPlrY = 30;
int defaultPlrX = 4;
int plrX = defaultPlrX;
int plrY = defaultPlrY;

/*Set the default difficulty here - 0 is easy, 1 is normal, 2 is hard*/
int difficulty = 0;
int enemyHitCoolDown = 0;
/*Counts up to 6 with one frame bmp for every count of 2*/
int enemyFrame = 0;

int shipSelection = 0;
int playerDmgMod = 0;
int playerSpdMod = 0;

/*Restarts the game*/
void StartGame()
{
  sessionStart = millis();
  sessionTime = 0;

  playerShotX = 0;
  playerShotY = 0;
  playerShot = 0;
  playerShotCooldown = 0;
  enemyY = 8;
  direction = 0;

  gameOver = 0;
  preGame = 0;

  rx = 95;
  ry = 0;
  rx2 = 95;
  ry2 = 0;
  rx3 = 95;
  ry3 = 0;
  points = 0;
  pointString = "";
  pointStringMod = "";

  enemyBulletSpeed = 3;
  bossSpeed = 1;
  
  rx4 = 95;
  ry4 = 0;
  lives = 5;
  poc = 0;
  fired = 0;
  levelCount = 1;
  initially = 0;
  selected = 0;
  currentTime = 0;
  bombCoolDown = 0;

  /*Reset some defaults*/  
  plrX = defaultPlrX;
  plrY = defaultPlrY;
  
}

void beep(int note, int duration)
{
  /*Play tone on buzzerPin*/
  tone(mg.buzPin, note, duration);
  delay(duration);
  noTone(mg.buzPin);
  delay(50);
}

//TODO 080825 - The bomb just clears the enemy shots from the screen but maybe it can be made more useful later...
void UseBomb()
{
  if (bombCoolDown == 0)
  {
    tone(mg.buzPin, 80, 750);
    mg.display.invertDisplay(true);
    delay(100);
    mg.display.invertDisplay(false);
    delay(100);
    mg.display.invertDisplay(true);
    delay(100);
    mg.display.invertDisplay(false);
    fired = 0;
    bombCoolDown = 300;
  }
}
int ambientXVals[] = {50, 30, 60, 55, 25, 100, 117, 14, 24, 78, 80, 107, 150, 5, 8, 70, 10, 70};
int ambientYVals[] = {30, 17, 18, 16, 43,  52,  43, 49, 24, 36, 57,  11,  11, 5, 7, 12, 56, 25};
void RenderAmbience()
{
  //TODO 2021- I'm not sure why I can't use sizeof() on the arrays instead of a hardcoded 18...need better understanding of sizeof() method...
  for (int i = 0; i < 18; i++)
  {
    if (ambientXVals[i] < 0)
    {
      ambientXVals[i] = 127;
    }
    mg.display.drawPixel(ambientXVals[i], ambientYVals[i], WHITE);
    ambientXVals[i] -= 2;
  }
}

void PlayIntroTune(){
  beep(294, 200);
  beep(200, 75);
  beep(392, 200);
  beep(250, 75);
  beep(450, 200);
  beep(300, 75);
  beep(600, 500);
  delay(500);
}

void RenderTitleScreen(){
  mg.display.clearDisplay();
  mg.display.drawBitmap(0, 0, spaceChase.titleBg, 128, 64, WHITE);
  mg.display.setTextSize(2);
  mg.display.setTextColor(BLACK);
  mg.display.setCursor(3, 2);
  mg.display.println("SPACE");
  mg.display.setCursor(3, 22);
  mg.display.println("CHASE");
  mg.display.setTextColor(WHITE);
  mg.display.setCursor(0, 0);
  mg.display.println("SPACE");
  mg.display.setCursor(0, 20);
  mg.display.println("CHASE");
  mg.display.setCursor(65, 20);
  mg.display.display();
}

void RenderGameOver(){ 
  /*only set the session time if it = 0. this will prevent it from continuing to count since this method is run in the loop*/  
  if(sessionTime == 0) { sessionTime = millis() - sessionStart; }   
  
  String diffString;
  switch(difficulty){
    case 0: diffString = "easy"; break;
    case 1: diffString = "normal"; break;
    case 2: diffString = "hard"; break;
  }
  String endMessage = "";
  mg.display.clearDisplay();     
  if(levelCount == 3){
    mg.display.drawBitmap(0, 0, spaceChase.winBg, 128, 64, WHITE);
    endMessage = "VICTORY!";
    }
    else{
      mg.display.drawBitmap(0, 0, spaceChase.failBg, 128, 64, WHITE);
      endMessage = "GAME OVER!";
    }
  mg.display.drawRect(0, 0, 128, 64, WHITE);
  mg.display.setFont();
  mg.display.setTextSize(2);
  mg.display.setTextColor(WHITE);
  mg.display.setCursor(7, 10);    
  mg.display.println(endMessage);
  mg.display.setTextSize(1);
  mg.display.setCursor(7, 30);
  mg.display.println("score:");
  mg.display.setCursor(44, 30);
  mg.display.println(pointString);
  mg.display.setCursor(7, 40);
  mg.display.println("diff :");
  mg.display.setCursor(44, 40);    
  mg.display.println(diffString);
  mg.display.setCursor(7, 50);
  mg.display.println("time :");
  mg.display.setCursor(44, 50);  
  mg.display.println(sessionTime / 1000);
  
  mg.display.display();
}

int flameFrameCounter = 0;
void RenderEnemy(int xVal, int yVal, int enemyLevel, int color)
{ 
  /*Each case represents a phase of animation. Increasing the number of cases for each phase slows the animation.
  Some of this should be simplified in the future.*/
  int sizeL0[] = {20, 20};
  int sizeL1[] = {16, 14};
  if (enemyFrame > 5) { enemyFrame = 0; }
    switch (enemyFrame)
    {
      case 0:
      case 1:
        if(enemyLevel == 1) {           
          if(enemyHitCoolDown == 0 ) {mg.display.drawBitmap(xVal, yVal, spaceChase.bossShell0, sizeL0[0], sizeL0[1], color);}
          else{mg.display.drawBitmap(xVal, yVal, spaceChase.bossShellHit, sizeL0[0], sizeL0[1], color);}         
        }
        else {
          if(enemyHitCoolDown == 0) {mg.display.drawBitmap(xVal, yVal, spaceChase.bossPod0, sizeL1[0], sizeL1[1], color);}
          else{mg.display.drawBitmap(xVal, yVal, spaceChase.bossPodHit, sizeL1[0], sizeL1[1], color);}
        }
        break;      
      case 2:
      case 3:
        if(enemyLevel == 1) {
          if(enemyHitCoolDown == 0){mg.display.drawBitmap(xVal, yVal, spaceChase.bossShell1, sizeL0[0], sizeL0[1], color);} 
          else{mg.display.drawBitmap(xVal, yVal, spaceChase.bossShellHit, sizeL0[0], sizeL0[1], color);}
          }
        else{
          if(enemyHitCoolDown == 0) {mg.display.drawBitmap(xVal, yVal, spaceChase.bossPod1, sizeL1[0], sizeL1[1], color);}
          else{mg.display.drawBitmap(xVal, yVal, spaceChase.bossPodHit, sizeL1[0], sizeL1[1], color);}
          }
        break;
      case 4:
      case 5:
        if(enemyLevel == 1) { 
          if(enemyHitCoolDown == 0){mg.display.drawBitmap(xVal, yVal, spaceChase.bossShell2, sizeL0[0], sizeL0[1], color); }
          else{mg.display.drawBitmap(xVal, yVal, spaceChase.bossShellHit, sizeL0[0], sizeL0[1], color);}
          }
        else {
          if(enemyHitCoolDown == 0) {mg.display.drawBitmap(xVal, yVal, spaceChase.bossPod2, sizeL1[0], sizeL1[1], color);}
          else{mg.display.drawBitmap(xVal, yVal, spaceChase.bossPodHit, sizeL1[0], sizeL1[1], color);}
          }
        break;
    }
    
    /*Height and width mod are used for determining travel range and hit detection*/
    if(enemyLevel == 1){
      enemyHeightMod = sizeL0[0] / 2;
      enemyWidthMod = sizeL0[1] / 2;
    }
    if(enemyLevel == 2){
      enemyHeightMod = sizeL1[0] / 2;
      enemyWidthMod = sizeL1[1] / 2;
    }

    /*Render the enemy exhaust flame*/
    if(flameFrameCounter > 3){flameFrameCounter = 0;}
    switch (flameFrameCounter)
    {
    case 0:
    case 1:
      mg.display.drawBitmap(xVal - 6, yVal + (enemyHeightMod - 3), spaceChase.largeFlame0, 6, 6, WHITE);      
      break;
    case 2:
    case 3:
      mg.display.drawBitmap(xVal - 6, yVal + (enemyHeightMod - 3), spaceChase.largeFlame1, 6, 6, WHITE);      
      break;
    }
    flameFrameCounter++;
    enemyFrame++;
}

int explosionFrame = 0;
void RenderExplosion(int xVal, int yVal, int color)
{ 
  /*When the Level 2 pointThreshold is met the boss explodes and the game ends.*/
  if (explosionFrame > 19) {
    explosionFrame = 0; 
    gameOver = 1;       
    beep(294, 200);
    beep(200, 75);
    beep(600, 500);
    delay(500);
    return;
    
    }
    switch (explosionFrame)
    {
      case 0:
      case 1:
      case 4:
      case 5:
      case 8:
      case 9:
      case 12:
      case 13:
      case 16:
      case 17:
        mg.display.drawBitmap(xVal, yVal, spaceChase.explode0, 20, 20, color);
        break;
      case 2:
      case 3:
      case 6:
      case 7:
      case 10:
      case 11:
      case 14:
      case 15:
      case 18:
      case 19:
        mg.display.drawBitmap(xVal, yVal, spaceChase.explode1, 20, 20, color);        
        break;
    }
    tone(mg.buzPin, 80, 650);    
    explosionFrame++;
}

void RenderPlayerShip(int xVal, int yVal, int shipChoice)
{
  switch (shipChoice)
  {
  case 0:
    mg.display.drawBitmap(xVal, yVal, spaceChase.shipOp0, 16, 16, WHITE);
    break;
  case 1:
    mg.display.drawBitmap(xVal, yVal, spaceChase.shipOp1, 16, 16, WHITE);
    break;
  case 2:
    mg.display.drawBitmap(xVal, yVal, spaceChase.shipOp2, 16, 16, WHITE);
    break;
  case 3:
    mg.display.drawBitmap(xVal, yVal, spaceChase.shipOp3, 16, 16, WHITE);
    break;
  }
  if(preGame == 0){
    if(flameFrameCounter > 3){flameFrameCounter = 0;}
    switch (flameFrameCounter)
    {
    case 0:
    case 1:
      mg.display.drawBitmap(xVal - 4, yVal + 6, spaceChase.smallFlame0, 4, 4, WHITE);      
      break;
    case 2:
    case 3:
      mg.display.drawBitmap(xVal - 4, yVal + 6, spaceChase.smallFlame1, 4, 4, WHITE);      
      break;
    }
    flameFrameCounter++;
  }
}

//TODO 072925 - this renders the ship selection along with the rest of the ship select screen, maybe handle those separately?
void RenderSelectedShip()
{
  mg.display.clearDisplay();
  mg.display.drawBitmap(0, 0, spaceChase.titleBg, 128, 64, WHITE);
  mg.display.drawRect(0, 0, 128, 64, WHITE);
  mg.display.fillRoundRect(15, -8, 98, 20, 8, WHITE);
  mg.display.setTextSize(1);
  mg.display.setTextColor(BLACK);
  mg.display.setCursor(19, 2);
  mg.display.println("- Ship Select -");
  /*this is the selection box for the ship*/
  mg.display.fillRect(20, 24, 32, 32, BLACK);
  mg.display.drawRect(20, 24, 32, 32, WHITE);  
  RenderPlayerShip(28,32, shipSelection);

  /*this is the selection box for the difficulty*/
  mg.display.fillRect(64, 17, 50, 44, BLACK);
  mg.display.setTextColor(WHITE);  
  mg.display.setCursor(78, 21);
  mg.display.println("easy");
  mg.display.setCursor(72, 35);
  mg.display.println("normal");
  mg.display.setCursor(78, 49);
  mg.display.println("hard");
  
  switch(difficulty){
    case 0: mg.display.drawRoundRect(69, 20, 40, 13, 4, WHITE);
            break;
    case 1: mg.display.drawRoundRect(69, 33, 40, 13, 4, WHITE);
            break;
    case 2: mg.display.drawRoundRect(69, 46, 40, 13, 4, WHITE);
            break;             
  }

  switch(shipSelection){
    case 0: playerDmgMod = 3;
            playerSpdMod = 0;
            break;
    case 1: playerDmgMod = 2;
            playerSpdMod = 1;
            break;
    case 2: playerDmgMod = 1;
            playerSpdMod = 2;
            break;
    case 3: playerDmgMod = 0;
            playerSpdMod = 3;
            break;
  }
   
  mg.display.display();
}

void RenderHUD(){  
    /*Score segment*/
    mg.display.setTextSize(1);
    mg.display.setTextColor(WHITE);
    mg.display.setCursor(70, 0);
    pointString = String(points);
    for (int x = pointString.length(); x < 5; x++){ pointString = "0" + pointString; }
    mg.display.println("Scr:" + pointString);    
    /*Health, bomb and level counter*/
    mg.display.fillRoundRect(-40, -6, 80, 16, 8, WHITE);
    mg.display.fillRect(0, 1, 27, 7, BLACK);
    mg.display.fillCircle(32, 4, 4, BLACK);    
    /*Life bar - player can take five shots*/
    mg.display.fillRect(0, 3, lives * 5, 3, WHITE);
    /*Bomb ready*/
    if (bombCoolDown == 0) { mg.display.fillCircle(32, 4, 2, WHITE); }
    else { mg.display.fillCircle(32, 4, 2, BLACK); }    
    mg.display.display();    
}

//TODO 072825 - ButtonTester would probably be a better name for this method. Revisit... 
void RenderModeSelect(){  
  mg.display.clearDisplay();
  //mg.display.drawBitmap(0, 0, mg.splashScreen, 128, 64, WHITE);
  mg.display.drawRect(0, 0, 128, 64, WHITE);
  //mg.display.fillRoundRect(15, -5, 98, 20, 8, BLACK);
  mg.display.fillRoundRect(15, -8, 98, 20, 8, WHITE);
  mg.display.setTextSize(1);
  mg.display.setTextColor(BLACK);
  mg.display.setCursor(19, 2);
    
  mg.display.println("- Button Test -");  
  mg.display.drawCircle(18, 20, 4, WHITE);
  mg.display.drawCircle(18, 40, 4, WHITE);
  mg.display.drawCircle(8, 30, 4, WHITE);
  mg.display.drawCircle(28, 30, 4, WHITE);

  mg.display.drawCircle(109, 20, 4, WHITE);
  mg.display.drawCircle(109, 40, 4, WHITE);

  // mg.display.drawRect(0, 0, 128, 64, WHITE);
  //mg.display.fillRoundRect(15, 53, 98, 69, 8, BLACK);
  mg.display.fillRoundRect(15, 53, 98, 72, 8, WHITE);
  mg.display.setTextSize(1);
  mg.display.setTextColor(BLACK);  
  mg.display.setCursor(19, 55);
  mg.display.println("- 1+2 = Start -");
  
  if(mg.PressLUp()){
    //mg.display.println("LUp");
    mg.display.fillCircle(18, 20, 4, WHITE);
    beep(294, 75);
    }
  if(mg.PressLDn()){
    //mg.display.println("LDn");
    mg.display.fillCircle(18, 40, 4, WHITE);
    beep(200, 75);
    }
  if(mg.PressLLt()){
    //mg.display.println("LLt");
    mg.display.fillCircle(8, 30, 4, WHITE);
    beep(392, 75);
    }
  if(mg.PressLRt()){
    //mg.display.println("LRt");
    mg.display.fillCircle(28, 30, 4, WHITE);
    beep(250, 75);
    }
  if(mg.PressRUp()){
    //mg.display.println("RUp");
    mg.display.fillCircle(109, 20, 4, WHITE);
    beep(450, 75);
    }
  if(mg.PressRDn()){
    //mg.display.println("RDn");
    mg.display.fillCircle(109, 40, 4, WHITE);
    beep(300, 75);
    }
    
  mg.display.display();
    
  if(mg.PressRUp() && mg.PressRDn()){    
    preGame = 1;
    mg.display.clearDisplay();
    RenderTitleScreen();
    delay(1500);
  }
}

void ShipSelect() {
  RenderSelectedShip();
  //select up and re-render
  if (mg.PressLUp()) {
    shipSelection++;
    if (shipSelection > 3) {
      shipSelection = 0;
    }
    RenderSelectedShip();
    delay(500);
  }
  //select down and re-render
  if (mg.PressLDn()) {
    shipSelection--;
    if (shipSelection < 0) {
      shipSelection = 3;
    }
    RenderSelectedShip();
    delay(500);
  }
  
  //TODO 080825 - it would be more intuitive to use the d-pad to make this selection. Need logic to toggle the side of the selection to make that work...
  if (mg.PressRUp()) {
    difficulty++;
    if (difficulty > 2) {
      difficulty = 0;
    }
    delay(500);
  }
  
  if (mg.PressRDn()) {
    tone(mg.buzPin, 370, 300);
    delay(300);
    StartGame();
  }
}

void TakePlayerDamage(){
  lives = lives - 1;
}

void setup()
{
  Serial.begin(9600);
  
  mg.Begin();
  mg.RenderSplashScreen();
    
}

void loop()
{
  if(preGame == 2){
    RenderModeSelect();
  }
    
  /*Pre-game setup*/
  if (preGame == 1)
  {
    ShipSelect();    
  }

  /*If not game over and not pregame do all the things...*/
  if (gameOver == 0 && preGame == 0) {
    mg.display.clearDisplay();    
    /*stars in the background*/
    RenderAmbience();
    if (poc == 0)
    {
      initially = millis();
      selected = random(400, 1200);
      poc = 1;
    }
    
    currentTime = millis();

    /*level stuff*/
    
    /*Point threshhold should be the same for easy and normal - the only difference for easy is no damage*/
    int pointThreshold = difficulty == 2 ? 75 : 50;
    if (points > pointThreshold && levelCount == 1)
    {
      //TODO 080325 - i dont think lvl is actually used
      //lvl = currentTime;
      levelCount++;

      //TODO 072925 - maybe only increase bullet speed on normal and hard
      enemyBulletSpeed = enemyBulletSpeed + 1; //Enemy bullet logic
      if(difficulty == 2){ bossSpeed ++;}
      
    }
    /*If points exceed the threshold and the level is 3 then stop the boss. This triggers the explosion and end game.*/
    if (points > pointThreshold * 2 && levelCount == 2) {
      levelCount++;
      bossSpeed = 0;
      }

    if ((selected + initially) < currentTime) {
      // Serial.println(selected / 1000);
      // Serial.println(initially / 1000);
      poc = 0;
      fired = fired + 1;
      if (fired == 1)
      {
        rx = 95;
        ry = enemyY;
      }
      if (fired == 2)
      {
        rx2 = 95;
        ry2 = enemyY;
      }
      if (fired == 3)
      {
        rx3 = 95;
        ry3 = enemyY;
      }

      if (fired == 4)
      {
        rx4 = 95;
        ry4 = enemyY;
      }
    }
    
    /*this handles the shot variety from the enemy*/
    if (fired > 0){
      mg.display.fillCircle(rx, ry, 2, 1);
      rx = rx - enemyBulletSpeed;
    }    
    if (fired > 1){
      mg.display.fillCircle(rx2, ry2, 3, 1);
      rx2 = rx2 - enemyBulletSpeed;
    }
    if (fired > 2){
      mg.display.fillCircle(rx3, ry3, 4, 1);
      rx3 = rx3 - enemyBulletSpeed;
    }
    if (fired > 3){
      mg.display.fillCircle(rx4, ry4, 2, 1);
      rx4 = rx4 - enemyBulletSpeed;
    }
    
    //TODO 080825 - i need to work on the movement modifiers some because things dont feel quite right here. The right direction needs +1 or else the player ship cannot advance, but greater than 1 feels too fast.
    /*Player Movement*/
    if(mg.PressLUp() && plrY >= 2){ plrY -= (1 + playerSpdMod); }
    if(mg.PressLDn() && plrY <= 46) { plrY += (1 + playerSpdMod); }    
    if(mg.PressLRt() && plrX <= 70) { plrX += 1; }
    if(mg.PressLLt() && plrX >= 4) {plrX -= (2 + playerSpdMod);}
    
    /*Player shot logic*/    
    if (mg.PressRDn() && playerShot == 0 && playerShotCooldown == 0)
    {
      //TODO 080325 - maybe get rid of this but it does flash the led which the kids might like too...
      //Serial.println("pew!");
      playerShot = 1;
      /*The cool down ensures shots dont speed up the closer the player gets to the enemy*/
      playerShotCooldown = 20 - (playerSpdMod * 4);

      playerShotX = plrX + 4;
      playerShotY = plrY + 8;
      /*dont use beep() here because it will slow down gameplay*/
      tone(mg.buzPin, 1300, 20);
    }
    if (playerShot == 1) {      
      playerShotX += 8;
      /*this handles the look of the player's shots based on ship selection*/
      switch(shipSelection){
        case 0: mg.display.drawLine(playerShotX, playerShotY + 4, playerShotX + 8, playerShotY + 4, WHITE);
                mg.display.drawLine(playerShotX, playerShotY - 4, playerShotX + 8, playerShotY - 4, WHITE);                
                break;
        case 1: mg.display.drawLine(playerShotX, playerShotY + 3, playerShotX + 4, playerShotY + 3, WHITE);
                mg.display.drawLine(playerShotX, playerShotY - 3, playerShotX + 4, playerShotY - 3, WHITE);
                break;
        case 2: 
                mg.display.drawLine(playerShotX, playerShotY, playerShotX + 10, playerShotY, WHITE);
                break;
        case 3: mg.display.drawLine(playerShotX, playerShotY, playerShotX + 6, playerShotY, WHITE);
                break;
      }
    }    
    
    if (mg.PressRUp()) { UseBomb(); }
    if (bombCoolDown > 0) { bombCoolDown--; }
    if(playerShotCooldown > 0){playerShotCooldown--;}

    /*Draw ships*/
    RenderPlayerShip(plrX, plrY, shipSelection);
    if(levelCount == 1){RenderEnemy(enemyX + 10, enemyY - 10, levelCount, WHITE);}
    else if (levelCount == 2){RenderEnemy(enemyX + 8, enemyY - 7, levelCount, WHITE);}
    else if (levelCount == 3){ RenderExplosion(enemyX + 10, enemyY - 10, WHITE); }
    
    if(enemyHitCoolDown > 0){enemyHitCoolDown--;}
    RenderHUD();

    /*If the player shot exceeds the width of the screen it disappears*/
    if (playerShotX > 128) { playerShot = 0; }
    
    //TODO 080125 - direction is either 0 for down or 1 for up. Since we're not using a boolean here we could possibly add more complex movement though
    if (direction == 0) { enemyY = enemyY + bossSpeed; }
    else { enemyY = enemyY - bossSpeed; }
    
    if (enemyY >= (64 - enemyHeightMod)) { direction = 1; }
    if (enemyY <= enemyHeightMod) { direction = 0; }

    /*This handles the enemy getting shot.*/
    if (playerShotY >= enemyY - enemyHeightMod && playerShotY <= enemyY + enemyHeightMod)
    {
      if (playerShotX > (enemyX) && playerShotX < (enemyX + enemyWidthMod * 2))
      {
        playerShotX = -20;
        tone(mg.buzPin, 500, 20);
        points += (1 + playerDmgMod);        
        playerShot = 0;        
        enemyHitCoolDown = 5;
      }
    }

    /*This handles player collision with the enemy bullets.
    Damage only happens on difficulties greater than easy so this can be less frustrating for small children to play.*/
    int position = plrY + 8;
    if (ry >= position - 8 && ry <= position + 8)
      if (rx < plrX + 8 && rx > plrX)
      {
        rx = 95;
        ry = -50;
        tone(mg.buzPin, 100, 100);        
        if(difficulty != 0) TakePlayerDamage();
      }

    if (ry2 >= position - 8 && ry2 <= position + 8)
      if (rx2 < plrX + 8 && rx2 > plrX)
      {
        rx2 = -50;
        ry2 = -50;
        tone(mg.buzPin, 100, 100);
        if(difficulty != 0) TakePlayerDamage();
      }

    if (ry3 >= position - 8 && ry3 <= position + 8)
      if (rx3 < plrX + 8 && rx3 > plrX)
      {
        rx3 = -50;
        ry3 = -50;
        tone(mg.buzPin, 100, 100);
        if(difficulty != 0) TakePlayerDamage();
      }

    if (ry4 >= position - 8 && ry4 <= position + 8)
      if (rx4 < plrX + 8 && rx4 > plrX)
      {
        rx4 = 200;
        ry4 = -50;
        fired = 0;
        tone(mg.buzPin, 100, 100);
        if(difficulty != 0) TakePlayerDamage();
      }

    if (rx4 < 1)
    {
      fired = 0;
      rx4 = 200;
    }
    if (lives == 0)
    {
      //TODO 080825 - add some logic here for the player to explode similar to the enemy.
      gameOver = 1;
    }
  }

  /*Game Over screen*/
  if (gameOver == 1)
  {
    if (lives == 0)
    {
      tone(mg.buzPin, 200, 300);
      delay(300);
      tone(mg.buzPin, 250, 200);
      delay(200);
      tone(mg.buzPin, 300, 300);
      delay(300);      
    }
    lives = 5;
    RenderGameOver();

    /*restarts game when button is pressed*/
    if (mg.PressRDn())
    {      
      tone(mg.buzPin, 280, 300);
      delay(300);
      tone(mg.buzPin, 250, 200);
      delay(200);
      tone(mg.buzPin, 370, 300);
      delay(300);
      StartGame();
      preGame = 1;
    }
  }
}
