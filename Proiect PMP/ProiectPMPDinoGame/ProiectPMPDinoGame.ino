//include biblioteca pentru lucrul cu LCD
#include <LiquidCrystal.h>
//inițializează lcd-ul la valorile stabilite ale pinilor
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//initializez matrici de byte-uri pentru reprezentarea caracterelor
byte standingDinoPart1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00001};
byte standingDinoPart2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B01000};
byte rightFootDinoPart1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00001};
byte rightFootDinoPart2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B00000};
byte leftFootDinoPart1[8] = {B00000, B00000, B00010, B00010, B00011, B00011, B00001, B00000};
byte leftFootDinoPart2[8] = {B00111, B00111, B00111, B00100, B11100, B11100, B11000, B01000};
byte twoBranchesPart1[8] = {B00000, B00100, B00100, B10100, B10100, B11100, B00100, B00100};
byte twoBranchesPart2[8] = {B00100, B00101, B00101, B10101, B11111, B00100, B00100, B00100};
byte birdWingsPart1[8] = {B00001, B00001, B00001, B00001, B01001, B11111, B00000, B00000};
byte birdWingsPart2[8] = {B00000, B10000, B11000, B11100, B11110, B11111, B00000, B00000};

int dinoColumn1 = 1,dinoColumn2 = 2,dinoRow = 3; //poz dino pe ecran
unsigned long timer = 0; //sincronizare
int period = 100;  //intervalul de timp intre schimbarile de stare
int flag = 1; //alterneaza intre diferite parti ale dinozaurului
int branchRow = 0; //pentru cactus
int branchColumn = 17; // Ajustat pentru 20 de coloane
int branchPeriod = 100; //pentru cactus
unsigned long branchTimer = 0; //pentru cactus
int a = 0,b = 1,c = 2,d = 0; //gestionarea logica a afisajului+sunet in functie de actiunile utilizatorului
unsigned long scoreTimer = 0;//urmareste timpul si actualizeaza scorul 
int scorePeriod = 100,score = 0,score2 = 0; //pt controlul si afisarea scorului
int randomNumber = 0; //utilizat in gestionarea tipului de obstacol
int birdColumn = 17; // Ajustat pentru 20 de coloane
int birdRow = 1; //pentru pasare
int currentSignal = 0,previousSignal = 0; //detecteaza starea curenta si anterioara
int f = 17; //Ajustat pentru 20 de coloane (controleaza afisarea cactusilor pe ecran)
int acceleration = 1; //ajustarea aparitiei cactusilor
unsigned long soundTimer = 0; //contor pentru gestionarea efectelor de sunet
int soundPeriod = 800; //controleaza durata sunetelor emise de buzzer
int jumpButton = 8; //pinul la care e conectat butonul de jump
int buzzer = 6;  //pinul la care e conectat buzzer-ul
int backend_score=0,highest_score=0; //scorul si cel mai mare scor
int restartButton = 7; //pinul la care e conectat butonul de restart 
bool gameIsOver = false; //daca jocul s-a incheiat
unsigned long lastDebounceTime = 0;  // ultima data cand a fost comutat pinul de iesire


void setup() {
  for (int i = 2; i <= 17; i++) //configurarea pinilor 2-17 ca iesiri
    pinMode(i, OUTPUT);
  pinMode(restartButton, INPUT);
  pinMode(jumpButton, INPUT);
  pinMode(buzzer, OUTPUT);
  
  lcd.begin(20, 4); //setează numărul de rânduri și coloane ale LCD-ului,20x4 display
  //crearea caracterelor
  lcd.createChar(0, standingDinoPart1);
  lcd.createChar(1, standingDinoPart2);
  lcd.createChar(2, rightFootDinoPart1);
  lcd.createChar(3, rightFootDinoPart2);
  lcd.createChar(4, leftFootDinoPart1);
  lcd.createChar(5, leftFootDinoPart2);
  lcd.createChar(6, twoBranchesPart1);
  lcd.createChar(7, twoBranchesPart2);
  lcd.clear();
  lcd.setCursor(5,1);
  lcd.print("Good luck !");
  lcd.setCursor(0,0);
  lcd.print("Welcome to Dino Game");
  delay(2000);
  lcd.clear();
}
void loop() {
  if (digitalRead(restartButton) == HIGH ) {
   gameIsOver = false;
   branchColumn = 19;
   branchPeriod = 100;
   score = 0;
   score2 = 0;
   branchPeriod = 100;
   timer = millis();
   branchTimer = millis();
   scoreTimer = millis();
   soundTimer = millis();
   lcd.clear();
  } 
 if (gameIsOver==false)
 { 
  Game_Code();
 }
}

void game_over(){
    lcd.clear();
    if(score>highest_score)
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("Your Score is");
      lcd.setCursor(2, 1);
      lcd.print("High Score : ");
      lcd.print(score);
    }
    else
    {
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("High Score : ");
      lcd.print(highest_score);
      lcd.setCursor(2, 2);
      lcd.print("Your Score : ");
      lcd.print(score);
    }
      lcd.setCursor(6, 3);
      lcd.print("GAME OVER");
    gameIsOver=true;
}

void Game_Code(){
  if (millis() > timer + period) 
  {
    timer = millis();
    if (flag == 1) 
    {
      flag = 2;
    } 
    else if (flag == 2) 
    {
      flag = 1;
    }
  }
  if (millis() > branchTimer + branchPeriod) {
      branchTimer = millis();
      branchColumn = branchColumn - 1;
      if (branchColumn <0) {
        branchColumn = 17;
        branchPeriod = branchPeriod - acceleration;
        randomNumber = random(0, 3);
    }
     if(branchColumn<0 || branchRow>3 )
     {
      branchColumn=0;
      branchRow=0;
     }
  f = branchColumn +1;
  lcd.setCursor(f, 3);
  lcd.print(" ");
  f = branchColumn+1;
  lcd.setCursor(f, 2);
  lcd.print(" ");
  lcd.setCursor(0, 3);
  lcd.print(" ");
  lcd.setCursor(0, 2);
  lcd.print(" ");
  a = 1;
}
  // Actualizarea pozitiei cursorului pentru ultimele doua linii(afisarea dinozaurului)
  if (d == 0) {  
    if (flag == 1) {
      lcd.setCursor(dinoColumn1, dinoRow);
      lcd.write(byte(2));
      lcd.setCursor(dinoColumn2, dinoRow);
      lcd.write(byte(3));
    }
    if (flag == 2) {
      lcd.setCursor(dinoColumn1, dinoRow);
      lcd.write(byte(4));
      lcd.setCursor(dinoColumn2, dinoRow);
      lcd.write(byte(5));
    }
  }
  if (a == 1) {
    if (randomNumber == 1) 
    {
      branchRow = 3;
      lcd.createChar(6, twoBranchesPart1);
      lcd.setCursor(branchColumn, branchRow);
      lcd.write(byte(6));
    } 
    else if (randomNumber == 2) 
    {
      branchRow = 3;
      lcd.createChar(7, twoBranchesPart2);
      lcd.setCursor(branchColumn, branchRow);
      lcd.write(byte(7));
    } 
    else 
    {
      birdColumn = branchColumn;
      birdColumn = birdColumn - 1;
      branchRow = 2;
      if(birdColumn>0)
      {lcd.createChar(6, birdWingsPart1);
      lcd.setCursor(birdColumn, branchRow);
      lcd.write(byte(6));}
      lcd.createChar(7, birdWingsPart2);
      lcd.setCursor(branchColumn, branchRow);
      lcd.write(byte(7));
    }
    a = 0;
  }
  if (digitalRead(jumpButton) == HIGH) 
  {
    b = 50;
    c = 50;

    //nu am afisat inca personajul sarind
    if (d == 0) 
    { 
      lcd.setCursor(0, 3);
      lcd.print("    ");
    }
    d = 1;

    lcd.setCursor(dinoColumn1, 2);
    lcd.write(byte(2));
    lcd.setCursor(dinoColumn2, 2);
    lcd.write(byte(3));
    if (millis() > soundTimer + soundPeriod) 
    {
      soundTimer = millis();
      int note[] = {600};
      for (int i = 0; i < 1; i++) 
      {
        tone(buzzer, note[i], 150);
        delay(20);
      }
    }
  } 
  else 
  {
    b = 1;
    c = 2;
    d = 0;
  }
  if(score>highest_score){
    highest_score = score;
  }
  if (millis() > scoreTimer + scorePeriod) 
  {
    scoreTimer = millis();
    score = score + 1;
  if (backend_score+100 == score) 
  {
    int note[] = {800, 900};
    for (int i = 0; i < 2; i++) 
    {
      tone(buzzer, note[i], 150);
      delay(150); 
    }
      backend_score = score;
      score2 = score2 + 1;
      if (score2 == 100) 
      {
        score2 = 0;
      }
  }
    lcd.setCursor(16, 1);
    lcd.print(score);
    lcd.setCursor(16, 0);
    lcd.print(score2);

    currentSignal = digitalRead(jumpButton);
    if (currentSignal != previousSignal) 
    {
      lcd.setCursor(1, 2);
      lcd.print("  ");
    }
    previousSignal = currentSignal;
  }

    // Conditii de coliziune pentru ultimele doua linii
  if (digitalRead(jumpButton) == HIGH && (branchColumn == 1 || branchColumn == 2 || birdColumn == 1 || birdColumn == 2) && branchRow == 2) 
  {
    int note[] = {200, 150};
    for (int i = 0; i < 2; i++) 
    {
      tone(buzzer, note[i], 250);
      delay(200);
    }
    game_over();
  }
  if ((branchColumn == b || branchColumn == c) && branchRow == 3) 
  {
    int note[] = {200, 150};
    for (int i = 0; i < 2; i++) 
    {
      tone(buzzer, note[i], 250);
      delay(200);
    }
    game_over();
  }
}
